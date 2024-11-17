/*
    UTF-8 was devised by Ken Thompson in 1992, five years before I was born.alignas
    # Important Points
    1: Character numbers from U+0000 to U+007F are valid ASCI, which makes it inherently compatible with UTF-8
    2: The first octets indicates number of bytes for a character in the byte stream
    3: octets values can never be 0xC0, 0xC1, 0xF5, 0xFF
    4: Character values can be found in the stream easily
    5: Boyer-Moore fast search algorithm can be used on UTF-8 strings


    # Specifications
    1: UTF-8 characters range between U+0000 ..U+10FFFF, and encoded in 1 to 4 octets
    2: The character which are only one character sequence long, has higher order bit set to 0 such as 0XXXXXXX
    3: The characters with n character sequence has all higher n bits set to 1 and followed by the zero
    Example:
        110xxxxx 10xxxxxx
        1110xxxx 10xxxxxx 10xxxxxx
        11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/


class utf8_string {

}