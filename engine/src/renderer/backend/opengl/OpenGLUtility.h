#pragma once
#include "OpenGLInclude.h"
#include "ocf/base/Types.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <utility>

namespace ocf::backend {

namespace OpenGLUtility {

GLenum toGLPrimitive(backend::PrimitiveType primitiveType);
GLenum toGLFormat(PixelFormat format);
GLsizei getGLDataTypeSize(GLenum type);

constexpr GLenum getInternalFormat(TextureFormat format) {
    switch (format) {
    case ocf::backend::TextureFormat::R8:       return GL_R8;
    case ocf::backend::TextureFormat::RG8:      return GL_RG8;
    case ocf::backend::TextureFormat::RGB8:     return GL_RGB8;
    case ocf::backend::TextureFormat::RGBA8:    return GL_RGBA8;
    default:                                    return GL_NONE;
    }
}

constexpr GLenum getTextureTarget(SamplerType target) {
    switch (target) {
    case SamplerType::SAMPLER_2D:
        return GL_TEXTURE_2D;
    case SamplerType::SAMPLER_2D_ARRAY:
        return GL_TEXTURE_2D_ARRAY;
    case SamplerType::SAMPLER_CUBEMAP:
        return GL_TEXTURE_CUBE_MAP;
    case SamplerType::SAMPLER_CUBEMAP_ARRAY:
        return GL_TEXTURE_CUBE_MAP_ARRAY;
    default:
        return GL_NONE;
    }
}

constexpr std::pair<GLenum, GLenum> textureFormatToFormatAndType(TextureFormat format) {
    switch (format) {
    case ocf::backend::TextureFormat::R8:       return {GL_RED, GL_UNSIGNED_BYTE};
    case ocf::backend::TextureFormat::RG8:      return {GL_RG, GL_UNSIGNED_BYTE};
    case ocf::backend::TextureFormat::RGB8:     return {GL_RGB, GL_UNSIGNED_BYTE};
    case ocf::backend::TextureFormat::RGBA8:    return {GL_RGBA, GL_UNSIGNED_BYTE};
    default:                                    return {GL_NONE, GL_NONE};
    }
}

} // namespace OpenGLUtility
} // namespace ocf::backend
