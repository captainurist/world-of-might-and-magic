#include "LowercaseFileSystem.h"

#include <ranges>
#include <filesystem>

#include "Utility/MapAccess.h"
#include "Utility/Exception.h"
#include "Utility/String/Transformations.h"

// TODO(captainurist): move out all ascii functions to namespace ascii in Utility/Ascii.h
static bool asciiIsUpper(char c) {
    return c >= 'A' && c <= 'Z';
}

static bool asciiIsLower(std::string_view path) {
    return !std::ranges::any_of(path, &asciiIsUpper);
}

[[noreturn]] static void throwNotDirectory(std::string_view path) {
    throw Exception("Path '{}' is not a directory.", path);
}

[[noreturn]] static void throwNotFile(std::string_view path) {
    throw Exception("Path '{}' does not point to a file", path);
}

[[noreturn]] static void throwDoesntExist(std::string_view path) {
    throw Exception("Path '{}' doesn't exist.", path);
}

[[noreturn]] static void throwAmbiguous(std::string_view path) {
    throw Exception("Path '{}' maps to several different files in the underlying file system.", path);
}

LowercaseFileSystem::LowercaseFileSystem(FileSystem *base) : _base(base) {
    assert(base);
}

LowercaseFileSystem::~LowercaseFileSystem() = default;

bool LowercaseFileSystem::exists(std::string_view path) const {
    WalkResult result = walk(normalizePath(path));
    return result.status == PATH_OK;
}

FileStat LowercaseFileSystem::stat(std::string_view path) const {
    WalkResult result = walk(normalizePath(path));
    switch (result.status) {
    case PATH_OK:
        return _base->stat(result.entry->basePath);
    case PATH_LEAF_IS_AMBIGUOUS:
        return FileStat(FILE_REGULAR, 0, 0);
    case PATH_PARENT_IS_AMBIGUOUS:
    case PATH_DOESNT_EXIST:
        return {};
    }
}

std::vector<DirectoryEntry> LowercaseFileSystem::ls(std::string_view path) const {
    auto createLsResult = [](CachedDirectoryEntry *entry) {
        std::vector<DirectoryEntry> result;
        for (const CachedDirectoryEntry &cachedSubEntry : entry->dir->ls) {
            if (!result.empty() && result.back().name == cachedSubEntry.name) {
                result.back().type = FILE_REGULAR; // It's a conflict.
            } else {
                result.emplace_back(DirectoryEntry(cachedSubEntry.type, cachedSubEntry.name));
            }
        }
        return result;
    };

    WalkResult result = walk(normalizePath(path));
    switch (result.status) {
    case PATH_OK:
        if (result.entry->type != FILE_DIRECTORY) {
            throwNotDirectory(path);
        } if (CachedDirectoryEntry *entry = refreshDirectoryEntry(result.entry)) {
            return createLsResult(entry);
        }
        [[fallthrough]]; // Got here? This means that the dir was snatched right from under us.
    case PATH_DOESNT_EXIST:
        throwDoesntExist(path);
    case PATH_LEAF_IS_AMBIGUOUS:
    case PATH_PARENT_IS_AMBIGUOUS:
        throwAmbiguous(path);
    }
}

Blob LowercaseFileSystem::read(std::string_view path) const {
    WalkResult result = walk(normalizePath(path));
    switch (result.status) {
    case PATH_OK:
        if (result.entry->type != FILE_REGULAR) {
            throwNotFile(path);
        } else {
            return _base->read(result.entry->basePath);
        }
    case PATH_DOESNT_EXIST:
        throwDoesntExist(path);
    case PATH_LEAF_IS_AMBIGUOUS:
    case PATH_PARENT_IS_AMBIGUOUS:
        throwAmbiguous(path);
    }
}

void LowercaseFileSystem::write(std::string_view path, const Blob &data) {
    std::string normalPath = normalizePath(path);
    WalkResult result = walk(normalPath);
    switch (result.status) {
    case PATH_OK:
        if (result.entry->type != FILE_REGULAR) {
            throwNotFile(path);
        } else {
            return _base->write(result.entry->basePath, data);
        }
    case PATH_DOESNT_EXIST:
        return _base->write(result.entry->basePath + normalPath.substr(result.entry->basePath.size()), data);
    case PATH_LEAF_IS_AMBIGUOUS:
    case PATH_PARENT_IS_AMBIGUOUS:
        throwAmbiguous(path);
    }
}

std::string LowercaseFileSystem::normalizePath(std::string_view path) {
    if (!asciiIsLower(path))
        throw Exception("Path '{}' contains uppercase letters, this is not supported.", path);

    return FileSystem::normalizePath(path);
}

LowercaseFileSystem::WalkResult LowercaseFileSystem::walk(std::string_view path) const {
    assert(path == normalizePath(path));
    if (path.empty())
        return WalkResult(PATH_OK, refreshDirectoryEntry(&_cachedRoot));

    CachedDirectoryEntry *entry = &_cachedRoot;
    std::vector<std::string_view> chunks = split(path, '/');
    for (size_t i = 0, size = chunks.size(); i < size; i++) {
        CachedDirectoryEntry *newEntry = refreshDirectoryEntry(entry);
        if (!newEntry)
            return WalkResult(PATH_DOESNT_EXIST, entry); // Not a folder, can't walk into it.
        entry = newEntry;

        auto [lo, hi] = std::ranges::equal_range(entry->dir->ls, chunks[i], std::ranges::less(), &CachedDirectoryEntry::name);
        switch (hi - lo) {
        case 0:
            return WalkResult(PATH_DOESNT_EXIST, entry);
        case 1:
            entry = &*lo;
            continue;
        default:
            return WalkResult(i + 1 == size ? PATH_LEAF_IS_AMBIGUOUS : PATH_PARENT_IS_AMBIGUOUS, entry);
        }
    }

    assert(entry);
    return WalkResult(PATH_OK, entry);
}

std::string LowercaseFileSystem::walkForReading(std::string_view path) const {
    WalkResult result = walk(normalizePath(path));
    switch (result.status) {
        case PATH_OK:
            if (result.entry->type != FILE_REGULAR) {
                throwNotFile(path);
            } else {
                return result.entry->basePath;
            }
        case PATH_DOESNT_EXIST:
            throwDoesntExist(path);
        case PATH_LEAF_IS_AMBIGUOUS:
        case PATH_PARENT_IS_AMBIGUOUS:
            throwAmbiguous(path);
    }
}

std::string LowercaseFileSystem::walkForWriting(std::string_view path) const {

}

std::unique_ptr<InputStream> LowercaseFileSystem::openForReading(std::string_view path) const {
    throwIfUpper(path);

}

std::unique_ptr<OutputStream> LowercaseFileSystem::openForWriting(std::string_view path) {
    throwIfUpper(path);

}

std::string LowercaseFileSystem::mapForReading(std::string_view path) const {
    throwIfUpper(path);

    CachedDirectory *dir = _cachedRoot.get();

    for
}

std::string LowercaseFileSystem::mapForWriting(std::string_view path) const {
    throwIfUpper(path);

}

void LowercaseFileSystem::throwIfUpper(std::string_view path) const {
    if (!asciiIsLower(path))
        throw Exception("Path '{}' contains uppercase letters, this is not supported.", path);
}

void LowercaseFileSystem::throwFileDoesntExist(std::string_view path) const {
    throw Exception("File '{}' doesn't exist.", path);
}

void LowercaseFileSystem::throwFilesConflict(std::string_view path) const {
    throw Exception("File path '{}' maps to several different files in the underlying file system.", path);
}

std::string LowercaseFileSystem::mapPathForReading(std::string_view path) const {
    throwIfUpper(path);

    std::string preprocessedPath = preprocessPath(path);
    auto [parentPath, fileName] = splitPath(preprocessedPath);

    CachedDirectory *cachedParent = cachedDirectory(parentPath);
    if (!cachedParent)
        throw Exception("File '{}' doesn't exist.", path);

    auto [lo, hi] = std::ranges::equal_range(cachedParent->ls, fileName, std::ranges::less(), &CachedDirectoryEntry::name);
    switch(hi - lo) {
    case 0: throwFileDoesntExist(path);
    case 1: return cachedParent->basePath + '/' + lo->base.name;
    default: throwFilesConflict(path);
    }
}

std::string LowercaseFileSystem::mapPathForWriting(std::string_view path) const {
    throwIfUpper(path);

    std::string preprocessedPath = preprocessPath(path);
    auto [parentPath, fileName] = splitPath(preprocessedPath);

    CachedDirectory *cachedParent = cachedDirectory(parentPath);
    if (!cachedParent)
        throw Exception("File '{}' doesn't exist.", path);

    auto [lo, hi] = std::ranges::equal_range(cachedParent->ls, fileName, std::ranges::less(), &CachedDirectoryEntry::name);
    switch(hi - lo) {
        case 0: throwFileDoesntExist(path);
        case 1: return cachedParent->basePath + '/' + lo->base.name;
        default: throwFilesConflict(path);
    }
}

LowercaseFileSystem::CachedDirectory *LowercaseFileSystem::cachedDirectory(std::string_view preprocessedPath) const {
    CachedDirectory *cachedResult = valuePtr(_cache, preprocessedPath);
    if (cachedResult) {
        FileStat baseStat = _base->stat(cachedResult->basePath);
        if (baseStat.type != FILE_DIRECTORY)


            {
            if (baseStat.mtime == result->mtime)
                return result;

            // It's a dir, but cached listing might be outdated.
            result->ls.clear();
            for (DirectoryEntry &entry : _base->ls(result->basePath)) { // TODO: this can throw.

            }
        }
    } else {
        auto [parentPath, dirName] = splitPath(preprocessedPath);

        CachedDirectory *cachedParent = cachedDirectory(parentPath);
        if (!cachedParent)
            return nullptr; // Parent doesn't exist => we don't exist.

        cachedResult = &_cache.emplace(preprocessedPath, createCachedDirectory(cachedParent, dirName)).first->second;
    }

    FileStat baseStat = _base->stat(cachedResult->basePath);
    if (baseStat.type == FILE_DIRECTORY) {
        if (baseStat.mtime == result->mtime)
            return result;

        // It's a dir, but cached listing might be outdated.
        result->ls.clear();
        for (DirectoryEntry &entry : _base->ls(result->basePath)) { // TODO: this can throw.

        }
    }

}

LowercaseFileSystem::CachedDirectory LowercaseFileSystem::createCachedDirectory(CachedDirectory *parent, std::string_view name) {

}

LowercaseFileSystem::CachedDirectoryEntry *LowercaseFileSystem::refreshDirectoryEntry(CachedDirectoryEntry *entry) const {
    FileStat baseStat = _base->stat(entry->basePath);
    if (baseStat.type != FILE_DIRECTORY) {
        assert(entry != &_cachedRoot); // Root should always exist & be a dir in the underlying FileSystem.
        return nullptr;
    }

    if (!entry->dir || entry->dir->mtime != baseStat.mtime) {
        try {
            if (!entry->dir)
                entry->dir = std::make_unique<CachedDirectory>();
            entry->dir->mtime = baseStat.mtime;
            entry->dir->ls.clear();

            for (const DirectoryEntry &baseSubEntry : _base->ls(entry->basePath)) {
                CachedDirectoryEntry &subEntry = entry->dir->ls.emplace_back();
                subEntry.name = toLower(baseSubEntry.name);
                subEntry.basePath = entry->basePath + '/' + baseSubEntry.name;
                subEntry.type = baseSubEntry.type;
            }

            std::ranges::sort(entry->dir->ls, std::ranges::less(), &CachedDirectoryEntry::name);
        } catch (...) {
            // Check if the folder was snatched right from under us - we shouldn't throw in this case.
            if (!_base->exists(entry->basePath))
                return nullptr;

            // Otherwise just rethrow - this might be a permission error.
            throw;
        }
    }

    return entry;
}

void LowercaseFileSystem::doIt() const {
    CachedDirectoryEntry *entry = refreshDirectoryEntry(&_cachedRoot);
    if (!entry)
        return false; // Nothing exists if root doesn't exist.

    std::string normalPath = normalizePath(path);
    if (normalPath.empty())
        return true; // Empty path is root, and root exists - we've just checked that above.

    for (std::string_view name : split(normalPath, '/')) {
        if (!entry)
            return false; // Parent was ambiguous.

        auto [lo, hi] = std::ranges::equal_range(entry->dir->ls, name, std::ranges::less(), &CachedDirectoryEntry::name);
        switch (hi - lo) {
            case 0:
                return false;
            case 1:
                entry = refreshDirectoryEntry(&*lo);
            continue;
            default:
                entry = nullptr;
            continue;
        }
    }

    return true;
}
