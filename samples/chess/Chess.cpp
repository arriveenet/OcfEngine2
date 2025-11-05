#include "Chess.h"
#include <ocf/3d/Mesh.h>

using namespace ocf;

Chess::Chess()
{
}

Chess::~Chess()
{
}

void Chess::initBoard()
{
    std::array<Variant, Mesh::ArrayType::ArrayMax> arrays;
    arrays[Mesh::ArrayType::ArrayVertex] = PackedVec3Array {
        { -1.0f, 0.0f, -1.0f },
        {  1.0f, 0.0f, -1.0f },
        {  1.0f, 0.0f,  1.0f },
        { -1.0f, 0.0f,  1.0f }};
    
}
