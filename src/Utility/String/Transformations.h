#pragma once

#include <cstring>
#include <algorithm> // For std::find.
#include <string>
#include <vector>

std::vector<char*> tokenize(char *input, const char separator);

/**
 * @param str                           String to unquote.
 * @return                              String with leading & trailing double quotes removed, if any.
 * @offset 0x00452C30
 */
inline std::string_view removeQuotes(std::string_view str) {
    std::string_view result = str;
    if (result.starts_with('"'))
        result = result.substr(1);
    if (result.ends_with('"'))
        result = result.substr(0, result.size() - 1);
    return result;
}

inline std::string trimRemoveQuotes(std::string str) {
    while (str.length() > 0 && (str.at(0) == ' ' || str.at(0) == '"')) {
        str.erase(0, 1);
    }
    while (str.length() > 0 && (str.at(str.length() - 1) == ' ' || str.at(str.length() - 1) == '"')) {
        str.pop_back();
    }

    return str;
}

inline std::string_view trim(std::string_view s) {
    size_t l = 0;
    size_t r = s.size();
    while (l < r && s[l] == ' ')
        l++;
    while (l < r && s[r - 1] == ' ')
        r--;
    return s.substr(l, r - l);
}

/**
 * `toHexDump("0123", 2)` is `"3132 3334"`.
 *
 * @param s                             String to convert.
 * @param groupSize                     Size of a single group, in bytes.
 * @return                              Hex dump of the provided string.
 */
std::string toHexDump(std::string_view s, size_t groupSize = 0);

std::string replaceAll(std::string_view text, std::string_view what, std::string_view replacement);

std::string replaceAll(std::string_view text, char what, char replacement);

/**
 * Splits the provided string `s` using separator `sep`, passing `std::string_view` chunks into `consumer`.
 *
 * This function doesn't discard empty chunks, so at least one chunk will always be passed to `consumer` - splitting an
 * empty string produces a single empty chunk.
 *
 * @param s                             String to split.
 * @param sep                           Separator character.
 * @param consumer                      Lambda to pass `std::string_view` chunks into.
 */
template<class Consumer>
void splitString(std::string_view s, char sep, Consumer &&consumer) {
    const char *pos = s.data();
    const char *end = s.data() + s.size();
    while (pos != end + 1) {
        const char *next = std::find(pos, end, sep);

        consumer(std::string_view(pos, next));
        pos = next + 1;
    }
}

void splitString(std::string_view s, char sep, std::vector<std::string_view> *result);

inline std::vector<std::string_view> splitString(std::string_view s, char sep) {
    std::vector<std::string_view> result;
    splitString(s, sep, &result);
    return result;
}

inline std::vector<std::string_view> splitString(const char *s, char sep) {
    return splitString(std::string_view(s), sep);
}

std::vector<std::string_view> splitString(std::string &&s, char sep) = delete; // Don't dangle!

namespace detail {

template<class T>
constexpr bool is_bounded_char_array_v = false;

template<std::size_t N>
constexpr bool is_bounded_char_array_v<char[N]> = true;

} // namespace detail

template<class T>
concept JoinableToString =
    std::is_same_v<std::remove_cvref_t<T>, char> ||
    std::is_same_v<std::remove_cvref_t<T>, std::string_view> ||
    std::is_same_v<std::remove_cvref_t<T>, std::string> ||
    std::is_same_v<std::remove_cvref_t<T>, const char *> ||
    detail::is_bounded_char_array_v<std::remove_cvref_t<T>>;

template<JoinableToString... Joinables> requires (sizeof...(Joinables) > 0)
std::string join(Joinables &&... joinables) {
    std::string result;
    int _[] = {
        ((result += joinables), 0)...
    };
    return result;
}

