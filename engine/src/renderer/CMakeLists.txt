# ==================================================================================================
# Renderer Module Sources and Headers
# ==================================================================================================

set(MODULE_PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/ocf/renderer/CustomCommand.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/IndexBuffer.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/Material.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/MaterialEnums.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/MeshCommand.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/Program.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/ProgramManager.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/QuadCommand.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/RenderCommand.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/Renderer.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/Texture.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/TextureManager.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/TextureSampler.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/TrianglesCommand.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/VertexBuffer.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/backend/BufferDescriptor.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/backend/Driver.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/backend/DriverEnums.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/backend/Handle.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/backend/PipelineState.h
    ${PUBLIC_HDR_DIR}/ocf/renderer/backend/PixelBufferDescriptor.h
)

set(MODULE_PRIVATE_HDRS
    src/renderer/RenderQueue.h
)

set(MODULE_SRCS
    src/renderer/CustomCommand.cpp
    src/renderer/IndexBuffer.cpp
    src/renderer/Material.cpp
    src/renderer/MeshCommand.cpp
    src/renderer/Program.cpp
    src/renderer/ProgramManager.cpp
    src/renderer/QuadCommand.cpp
    src/renderer/RenderCommand.cpp
    src/renderer/Renderer.cpp
    src/renderer/RenderQueue.cpp
    src/renderer/Texture.cpp
    src/renderer/TextureManager.cpp
    src/renderer/TrianglesCommand.cpp
    src/renderer/VertexBuffer.cpp
)

# Include backend sources (backend is nested, so process it inline)
set(BACKEND_PRIVATE_HDRS
    src/renderer/backend/DriverBase.h
    src/renderer/backend/HandleAllocator.h
)

set(BACKEND_SRCS
    src/renderer/backend/Driver.cpp
    src/renderer/backend/DriverBase.cpp
    src/renderer/backend/HandleAllocator.cpp
)

# OpenGL backend sources
if(OCF_USE_GL)
    list(APPEND BACKEND_PRIVATE_HDRS
        src/renderer/backend/opengl/OpenGLContext.h
        src/renderer/backend/opengl/OpenGLDriver.h
        src/renderer/backend/opengl/OpenGLInclude.h
        src/renderer/backend/opengl/OpenGLUtility.h
    )

    list(APPEND BACKEND_SRCS
        src/renderer/backend/opengl/OpenGLContext.cpp
        src/renderer/backend/opengl/OpenGLDriver.cpp
        src/renderer/backend/opengl/OpenGLUtility.cpp
    )
endif()

list(APPEND MODULE_PRIVATE_HDRS ${BACKEND_PRIVATE_HDRS})
list(APPEND MODULE_SRCS ${BACKEND_SRCS})

ocf_add_sources(PUBLIC_HDRS ${MODULE_PUBLIC_HDRS})
ocf_add_sources(PRIVATE_HDRS ${MODULE_PRIVATE_HDRS})
ocf_add_sources(SRCS ${MODULE_SRCS})


