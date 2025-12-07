#pragma once
#include <string>
#include "platform/PlatformMacros.h"

namespace ocf {

enum class AudioSourceFormat : uint16_t {
    PCM_UNKNOWN,
    PCM_U8,
    PCM_16,
    PCM_24,
};

class AudioDecoder {
public:
    virtual bool open(std::string_view filename) = 0;

    virtual void close() = 0;

    virtual uint32_t read(uint32_t framesToRead, char* pcmBuffer) = 0;

    virtual uint32_t readFixedFrames(uint32_t framesToRead, char* pcmBuffer);

    virtual bool seek(uint32_t frameOffset) = 0;

    virtual bool isOpened() const;

    virtual uint32_t getTotalFrames() const;

    virtual uint32_t framesToBytes(uint32_t frames) const;

    virtual uint32_t bytesToFrames(uint32_t bytes) const;

    virtual uint32_t getSampleRate() const;

    virtual uint32_t getChannelCount() const;

    uint32_t getSamplesPerBlock() const;

    AudioSourceFormat getSourceFormat() const;

protected:
    AudioDecoder();
    virtual ~AudioDecoder();

    bool m_isOpened;
    uint32_t m_totalFrames;
    uint32_t m_bytesPerBlock;
    uint32_t m_samplesPerBlock;
    uint32_t m_sampleRate;
    uint32_t m_channelCount;
    AudioSourceFormat m_sourceFormat;

    friend class AudioDecoderManager;
};

} // namespace ocf
