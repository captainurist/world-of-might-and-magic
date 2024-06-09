#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "Library/FileSystem/Interface/FileSystem.h"

#include "Utility/String/TransparentFunctors.h"

// Not thread-safe, even in const methods.

/**
 * Provides a lowercase view over another filesystem:
 * - Works only with lowercase names, passing names containing uppercase characters to any of the methods will throw.
 * - Provides a lowercase view over the existing files, effectively giving the user a case-insensitive view over
 *   a potentially case-sensitive filesystem.
 * - On conflicts, `exists` returns true, `stat` always returns `FILE_REGULAR`, even if it's two folders conflicting.
 *   `size` and `mtime` is always 0 in case of conflicts.
 * - Calling `ls` on conflicting folders will throw.
 * - Reading from / writing to conflicting files will throw.
 * - This class is NOT thread-safe, all const methods update the local cache.
 */
class LowercaseFileSystem : public FileSystem {
 public:
    LowercaseFileSystem(FileSystem *base);
    virtual ~LowercaseFileSystem();

    virtual bool exists(std::string_view path) const override;
    virtual FileStat stat(std::string_view path) const override;

    virtual std::vector<DirectoryEntry> ls(std::string_view path) const override;

    virtual Blob read(std::string_view path) const override;
    virtual void write(std::string_view path, const Blob &data) override;

    virtual std::unique_ptr<InputStream> openForReading(std::string_view path) const override;
    virtual std::unique_ptr<OutputStream> openForWriting(std::string_view path) override;

 private:
    struct CachedDirectory;

    struct CachedDirectoryEntry {
        std::string name; // Lowercase name.
        std::string basePath; // Full path in the base file system.
        FileType type = FILE_INVALID;
        std::unique_ptr<CachedDirectory> dir;
    };

    struct CachedDirectory {
        std::int64_t mtime = 0;
        std::vector<CachedDirectoryEntry> ls; // Sorted by lowercase name.
    };

    enum class PathStatus {
        PATH_DOESNT_EXIST = 0,
        PATH_PARENT_IS_AMBIGUOUS = 1,
        PATH_LEAF_IS_AMBIGUOUS = 2,
        PATH_OK = 3,
    };
    using enum PathStatus;

    struct WalkResult {
        WalkResult() = default;
        WalkResult(PathStatus status, CachedDirectoryEntry *entry): status(status), entry(entry) {}

        PathStatus status = PATH_DOESNT_EXIST;
        CachedDirectoryEntry *entry = nullptr; // Last existing entry, never null.
    };

 private:
    [[nodiscard]] static std::string normalizePath(std::string_view path);

    WalkResult walk(std::string_view path) const;
    std::string walkForReading(std::string_view path) const;
    std::string walkForWriting(std::string_view path) const;

    std::optional<std::string> mapForStat(std::string_view path) const;
    std::string mapForReading(std::string_view path) const;
    std::string mapForWriting(std::string_view path) const;

    void throwIfUpper(std::string_view path) const;
    [[noreturn]] void throwFileDoesntExist(std::string_view path) const;
    [[noreturn]] void throwFilesConflict(std::string_view path) const;

    CachedDirectory *cachedDirectory(std::string_view path, bool mustExist = false) const;
    CachedDirectory createCachedDirectory(CachedDirectory *parent, std::string_view name);

    CachedDirectoryEntry *refreshDirectoryEntry(CachedDirectoryEntry *entry) const;

 private:
    FileSystem *_base = nullptr;
    mutable CachedDirectoryEntry _cachedRoot;
};
