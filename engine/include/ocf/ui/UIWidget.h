#pragma once
#include "ocf/2d/Node2D.h"

namespace ocf {

namespace ui {

class Widget : public Node2D {
public:
    Widget();
    virtual ~Widget();

    virtual bool init();

    virtual void onMouseClicked();
    virtual void onMousePressed();
    virtual void onMouseReleased();
    virtual void onSetFocus();
    virtual void onKillFocus();

    bool isFocused() const { return m_focus; }

protected:
    virtual void initRenderer();

    bool m_focus = false;
    bool m_pressed = false;
};

}

} // namespace ocf