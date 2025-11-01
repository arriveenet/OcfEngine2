#include "ocf/renderer/MeshCommand.h"

namespace ocf {

MeshCommand::MeshCommand()
    : RenderCommand()
{
    m_type = Type::MeshCommand;
}

MeshCommand::~MeshCommand()
{
}

} // namespace ocf
