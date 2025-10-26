#pragma once
#include <string>

namespace ocf {

class ModelLoader {
public:
    ModelLoader() = default;
    virtual ~ModelLoader() =default;

    virtual bool load(std::string_view fileName) = 0;
};

} // namespace ocf
