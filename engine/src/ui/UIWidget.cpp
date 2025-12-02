#include "ocf/ui/UIWidget.h"

namespace ocf {

namespace ui {

Widget::Widget()
{
}
Widget::~Widget()
{
}

bool Widget::init()
{
    if (Node::init()) {
        initRenderer();
        setAnchorPoint(math::vec2(0.5f, 0.5f));

        return true;
    }
    return false;
}

void Widget::onMouseClicked() {}

void Widget::onMousePressed() {}

void Widget::onMouseReleased() {}

void Widget::onSetFocus() {}

void Widget::onKillFocus() {}

void Widget::initRenderer() {}

}

} // namespace ocf