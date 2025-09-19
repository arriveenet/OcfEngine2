# ==============================================================================
# Backend Sources and headers
# ==============================================================================
set(OCF_BACKEND_HEADER
    "src/renderer/backend/DriverBase.h"
    "src/renderer/backend/HandleAllocator.h"
)

set(OCF_BACKEND_SRC
    "src/renderer/backend/Driver.cpp"
    "src/renderer/backend/DriverBase.cpp"
    "src/renderer/backend/HandleAllocator.cpp"
)

# ==============================================================================
# OpenGL Sources
# ==============================================================================
if(OCF_USE_GL)
    list(APPEND OCF_BACKEND_HEADER
        "src/renderer/backend/opengl/OpenGLContext.h"
        "src/renderer/backend/opengl/OpenGLDriver.h"
        "src/renderer/backend/opengl/OpenGLInclude.h"
        "src/renderer/backend/opengl/OpenGLUtility.h"
        )

    list(APPEND OCF_BACKEND_SRC
        "src/renderer/backend/opengl/OpenGLContext.cpp"
        "src/renderer/backend/opengl/OpenGLDriver.cpp"
        "src/renderer/backend/opengl/OpenGLUtility.cpp"
        )
endif()


