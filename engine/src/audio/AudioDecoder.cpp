#include "AudioDecoder.h"
#include <string.h>

namespace ocf {

uint32_t AudioDecoder::readFixedFrames(uint32_t framesToRead, char* pcmBuffer)
{
    uint32_t framesRead = 0;
    uint32_t framesReadOnce = 0;
    do
    {
        framesReadOnce = read(framesToRead - framesRead, pcmBuffer + framesToBytes((framesRead)));
        framesRead += framesReadOnce;
    } while (framesReadOnce != 0 && framesRead < framesToRead);

    if (framesRead < framesToRead)
    {
        memset(pcmBuffer + framesToBytes(framesRead), 0x0, framesToBytes(framesToRead - framesRead));
    }

    return framesRead;
}

bool AudioDecoder::isOpened() const
{
    return m_isOpened;
}

uint32_t AudioDecoder::getTotalFrames() const
{
    return m_totalFrames;
}

uint32_t AudioDecoder::framesToBytes(uint32_t frames) const
{
    return m_bytesPerBlock * frames;
}

uint32_t AudioDecoder::bytesToFrames(uint32_t bytes) const
{
    return bytes / m_bytesPerBlock;
}

uint32_t AudioDecoder::getSampleRate() const
{
    return m_sampleRate;
}

uint32_t AudioDecoder::getChannelCount() const
{
    return m_channelCount;
}

uint32_t AudioDecoder::getSamplesPerBlock() const
{
    return m_samplesPerBlock;
}

AudioSourceFormat AudioDecoder::getSourceFormat() const
{
    return m_sourceFormat;
}

AudioDecoder::AudioDecoder()
    : m_isOpened(false)
    , m_totalFrames(0)
    , m_bytesPerBlock(0)
    , m_samplesPerBlock(1)
    , m_sampleRate(0)
    , m_channelCount(0)
    , m_sourceFormat(AudioSourceFormat::PCM_16)
{
}

AudioDecoder::~AudioDecoder()
{
}

} // namespace ocf
