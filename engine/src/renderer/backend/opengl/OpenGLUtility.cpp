#include "OpenGLUtility.h"
#include <assert.h>

namespace ocf::backend {

GLenum OpenGLUtility::toGLPrimitive(backend::PrimitiveType primitiveType)
{
    GLenum result = GL_TRIANGLES;
    switch (primitiveType) {
    case PrimitiveType::POINTS:
        result = GL_POINTS;
        break;
    case PrimitiveType::LINES:
        result = GL_LINES;
        break;
    case PrimitiveType::TRIANGLES:
        result = GL_TRIANGLES;
        break;
    default:
        break;
    }

    return result;
}

GLenum OpenGLUtility::toGLFormat(PixelFormat format)
{
    GLenum result = 0;

    switch (format) {
    case PixelFormat::GRAY:
        result = GL_RED;
        break;
    case PixelFormat::RGB:
        result = GL_RGB;
        break;
    case PixelFormat::RGBA:
        result = GL_RGBA;
        break;
    default:
        assert(false);
        break;
    }

    return result;
}

GLsizei OpenGLUtility::getGLDataTypeSize(GLenum type)
{
    GLsizei size = 0;
    switch (type) {
    case GL_BOOL:
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        size = sizeof(GLbyte);
        break;
    case GL_BOOL_VEC2:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
        size = sizeof(GLshort);
        break;
    case GL_BOOL_VEC3:
        size = sizeof(GLboolean);
        break;
    case GL_BOOL_VEC4:
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FLOAT:
        size = sizeof(GLfloat);
        break;
    case GL_FLOAT_VEC2:
    case GL_INT_VEC2:
        size = sizeof(GLfloat) * 2;
        break;
    case GL_FLOAT_VEC3:
    case GL_INT_VEC3:
        size = sizeof(GLfloat) * 3;
        break;
    case GL_FLOAT_MAT2:
    case GL_FLOAT_VEC4:
    case GL_INT_VEC4:
        size = sizeof(GLfloat) * 4;
        break;
    case GL_FLOAT_MAT3:
        size = sizeof(GLfloat) * 9;
        break;
    case GL_FLOAT_MAT4:
        size = sizeof(GLfloat) * 16;
        break;
    default:
        break;
    }
    return size;
}

} // namespace ocf::backend
