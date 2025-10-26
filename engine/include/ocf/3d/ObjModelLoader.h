#pragma once
#include "ocf/3d/ModelLoader.h"

namespace ocf {

class ObjModelLoader : public ModelLoader {
public:
    ObjModelLoader() = default;
    virtual ~ObjModelLoader() = default;
    bool load(std::string_view fileName) override;
};

} // namespace ocf
