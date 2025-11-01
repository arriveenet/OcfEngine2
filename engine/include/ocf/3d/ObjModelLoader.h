#pragma once
#include "ocf/3d/ModelLoader.h"

namespace ocf {

class ObjModelLoader : public ModelLoader {
public:
    ObjModelLoader();
    ~ObjModelLoader() override;

    bool load(std::string_view fileName, Mesh& mesh) override;
};

} // namespace ocf
