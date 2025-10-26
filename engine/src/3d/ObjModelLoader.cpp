#include "ocf/3d/ObjModelLoader.h"

#include "platform/PlatformMacros.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "ocf/core/FileUtils.h"
#include <tiny_obj_loader.h>

namespace ocf {

ObjModelLoader::ObjModelLoader()
{
}

ObjModelLoader::~ObjModelLoader()
{
}

bool ObjModelLoader::load(std::string_view fileName)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName.data());

    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = FileUtils::getInstance()->getParentFullPath(fullPath);

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(fullPath, readerConfig)) {
        if (!reader.Error().empty()) {
            OCF_LOG_ERROR("TinyObjReader: {}", reader.Error().c_str());
        }
        return false;
    }

    if (!reader.Warning().empty()) {
        OCF_LOG_ERROR("TinyObjReader: {}", reader.Warning().c_str());
    }

    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
    const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t indexOffset = 0;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes[s].mesh.num_face_vertices[f];

             // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];

                 // access to vertex
                tinyobj::real_t vx =
                    attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 0];
                tinyobj::real_t vy =
                    attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 1];
                tinyobj::real_t vz =
                    attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 2];

                 // Check if `normal_index` is zero or positive. negative = no normal data
                if (index.normal_index >= 0) {
                    tinyobj::real_t nx =
                        attrib.normals[3 * static_cast<size_t>(index.normal_index) + 0];
                    tinyobj::real_t ny =
                        attrib.normals[3 * static_cast<size_t>(index.normal_index) + 1];
                    tinyobj::real_t nz =
                        attrib.normals[3 * static_cast<size_t>(index.normal_index) + 2];

                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (index.texcoord_index >= 0) {
                    tinyobj::real_t tx =
                        attrib.texcoords[2 * static_cast<size_t>(index.texcoord_index) + 0];
                    tinyobj::real_t ty =
                        attrib.texcoords[2 * static_cast<size_t>(index.texcoord_index) + 1];

                }
            }
            indexOffset += fv;

            // shapes[s].mesh.material_ids[f];
        }
    }

    return true;
}

} // namespace ocf
