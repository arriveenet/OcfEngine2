#pragma once
#include "ocf/3d/Mesh.h"
#include <string>

namespace ocf {

class ModelLoader {
public:
    ModelLoader() = default;
    virtual ~ModelLoader() =default;

    virtual bool load(std::string_view fileName, Mesh& mesh) = 0;
};

} // namespace ocf
