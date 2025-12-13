#pragma once
#include <string>
#include "platform/PlatformMacros.h"

namespace ocf {

class AudioDecoder;

class AudioDecoderManager {
public:
    static bool init();
    static void destroy();
    static AudioDecoder* createDecoder(std::string_view filePath);
    static void destoroyDecoder(AudioDecoder* decoder);
};

} // namespace ocf
