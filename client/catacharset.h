#ifndef CATACHARSET_H
#define CATACHARSET_H

#include <stdint.h>
#include <string>
#define ANY_LENGTH 5
#define UNKNOWN_UNICODE 0xFFFD

class utf8_wrapper;

// get a unicode character from a utf8 string
uint32_t UTF8_getch(const char **src, int *srclen);
// from wcwidth.c, return "cell" width of a unicode char
int mk_wcwidth(uint32_t ucs);
// convert cursorx value to byte position
int cursorx_to_position(const char *line, int cursorx, int *prevppos = NULL, int maxlen = -1);
//erease for characters insertion
int erease_utf8_by_cw(char *t, int cw, int len, int maxlen);
int utf8_width(const char *s, const bool ignore_tags = false);
int utf8_width(const std::string &str, const bool ignore_tags = false);
int utf8_width(const utf8_wrapper &str, const bool ignore_tags = false);

/**
 * Center text inside whole line.
 * @param Text to be centered.
 * @param First printable position on line.
 * @param Last printable position on line.
 * @return First char position of centered text or start_pos if text is too big.
*/
int center_text_pos(const char *text, int start_pos, int end_pos);
int center_text_pos(const std::string &text, int start_pos, int end_pos);
int center_text_pos(const utf8_wrapper &text, int start_pos, int end_pos);
std::string utf8_substr(std::string s, int start, int size = -1);
std::string utf32_to_utf8(uint32_t ch);
std::string utf8_truncate(std::string s, size_t length);

std::string base64_encode(std::string str);
std::string base64_decode(std::string str);

/**
 * UTF8-Wrapper over std::string.
 * It looks and feels like a std::string, but uses code points counts
 * as index, not bytes.
 * A multi-byte Unicode character might by represented
 * as 3 bytes in UTF8, this class will see these 3 bytes as 1 character.
 * It will never separate them. It will however split between code points
 * which might be problematic when containing combination characters.
 * In this case use the *_display functions. They operate on the display width.
 * Code points with a zero width are considered to belong to the previous code
 * point and are not split from that.
 * Having a string with like [letter0][letter1][combination-mark][letter2]
 * (assuming each letter has a width of 1) will return a display_width of 3.
 * substr_display(1, 2) returns [letter1][combination-mark][letter2],
 * substr_display(1, 1) returns [letter1][combination-mark]
 * substr_display(2, 1) returns [letter2]
 *
 * Note: functions use code points, not bytes, for counting/indexing!
 * Functions with the _display suffix use display width for counting/indexing!
 * Protected functions might behave different
 *
 * For function documentation see std::string, the functions here
 * mimic the behavior of the equally named std::string function.
 */
class utf8_wrapper
{
public:
    utf8_wrapper() : _data(), _length(0), _display_width(0) { }
    utf8_wrapper(const std::string &d);
    utf8_wrapper(const char *d);

    void insert(size_t start, const utf8_wrapper &other);
    utf8_wrapper substr(size_t start, size_t length) const;
    utf8_wrapper substr(size_t start) const
    {
        return substr(start, _length - start);
    }
    void erase(size_t start, size_t length);
    void erase(size_t start)
    {
        erase(start, _length - start);
    }
    void append(const utf8_wrapper &other);
    utf8_wrapper &replace_all(const utf8_wrapper &search, const utf8_wrapper &replace);
    /**
     * Returns a substring based on the display width, not the number of
     * code points (as the other substr function does).
     * @param start Start the returned substring with the character that is
     * at that position when this string would be have been printed (rounded down).
     * E.g. a string "a´a´" (where a is a normal character, and ` is a combination
     * code point) would be displayed as two cells: "áá".
     * substr_display(0,2) would return the whole string, substr_display(0,1)
     * would return the first two code points, substr_display(1,1) would return the
     * last two code points.
     * @param length Display length of the returned string, the returned string can
     * have a shorter display length (especially if the last character is a multi-cell
     * character and including it would exceed the length parameter).
     */
    utf8_wrapper substr_display(size_t start, size_t length) const;
    utf8_wrapper substr_display(size_t start) const
    {
        return substr_display(start, _length - start);
    }

    utf8_wrapper &operator= (const std::string &d)
    {
        return *this = utf8_wrapper(d);
    }
    const std::string &str() const
    {
        return _data;
    }

    // Returns unicode character at position start
    long at(size_t start) const;

    // Returns number of unicode characters
    size_t size() const
    {
        return _length;
    }
    size_t length() const
    {
        return size();
    }
    bool empty() const
    {
        return size() == 0;
    }
    // Display size might be different from length, as some characters
    // are displayed as 2 chars in a terminal
    size_t display_width() const
    {
        return _display_width;
    }
    const char *c_str() const
    {
        return _data.c_str();
    }
    /**
     * Return a substring at most maxlength width (display width).
     * If the string had to shortened, an ellipsis (...) is added. The
     * string with the ellipsis will be exactly maxlength displayed
     * characters.
     */
    std::string shorten(size_t maxlength) const;
protected:
    std::string _data;
    size_t _length;
    size_t _display_width;
    // Byte offset into @ref _data for codepoint at index start.
    // bstart is a initial offset (in bytes!). The function operates on
    // _data.substr(bstart), it ignores everything before bstart.
    size_t byte_start(size_t bstart, size_t start) const;
    // Byte offset into @ref _date for the codepoint starting at displayed cell start,
    // if the first character occupies two cells, than byte_start_display(2)
    // would return the byte offset of the second codepoint
    // byte_start_display(1) and byte_start_display(0) would return 0
    size_t byte_start_display(size_t bstart, size_t start) const;
    // Same as @ref substr, but with a byte index as start
    utf8_wrapper substr_byte(size_t bytestart, size_t length, bool use_display_width) const;
    void init_utf8_wrapper();
};

#endif
