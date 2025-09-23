#include "ocf/base/CanvasItem.h"

namespace ocf {

CanvasItem::CanvasItem()
{
}

CanvasItem::~CanvasItem()
{
}

void CanvasItem::setVisible(bool visible)
{
    if (m_visible == visible) {
        return;
    }

    m_visible = visible;
}

bool CanvasItem::isVisible() const
{
    return m_visible;
}

} // namespace ocf