#include "ocf/core/StringUtils.h"

namespace ocf {

namespace StringUtils {

std::u32string convertUtf8ToUtf32(std::string_view utf8String)
{
    std::u32string result;
    result.reserve(utf8String.size());

    for (size_t i = 0, size = utf8String.size(); i < size; i++) {
        char p = utf8String.at(i);
        char32_t unicode = 0;

        int numBytes = 0;
        if ((p & 0x80) == 0x00) {
            numBytes = 1;
        }
        else if ((p & 0xE0) == 0xC0) {
            numBytes = 2;
        }
        else if ((p & 0xF0) == 0xE0) {
            numBytes = 3;
        }
        else if ((p & 0xF8) == 0xF0) {
            numBytes = 4;
        }

        switch (numBytes) {
        case 1:
            unicode = p;
            result.push_back(unicode);
            break;
        case 2:
            unicode = (p & 0x1F) << 6;
            unicode |= (utf8String[i + 1] & 0x3F);
            result.push_back(unicode);
            break;
        case 3:
            unicode = (p & 0x0F) << 12;
            unicode |= (utf8String[i + 1] & 0x3F) << 6;
            unicode |= (utf8String[i + 2] & 0x3F);
            result.push_back(unicode);
            break;
        case 4:
            unicode = (p & 0x07) << 18;
            unicode |= (utf8String[i + 1] & 0x3F) << 12;
            unicode |= (utf8String[i + 2] & 0x3F) << 6;
            unicode |= (utf8String[i + 3] & 0x3F) << 6;
            result.push_back(unicode);
            break;
        default:
            break;
        }
    }

    return result;
}

} // namespace StringUtils

} // namespace ocf
