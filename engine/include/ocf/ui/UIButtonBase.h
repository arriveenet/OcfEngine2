#pragma once
#include "ocf/ui/UIWidget.h"
#include <functional>

namespace ocf {

class DrawNode;
class Label;

namespace ui {

class ButtonBase : public Widget {
public:
    using ActionEvent = std::function<void()>;

    ButtonBase();
    ~ButtonBase() override;

    void updateNode(float deltaTime) override;

    void setOnAction(ActionEvent value);
    ActionEvent getOnAction() const { return m_onAction; }

    void setText(std::string_view text);
    std::string getText() const;

protected:
    bool createTextRendererIfNull();
    virtual void updateTextLocation();

protected:
    DrawNode* m_pButtonBackground;
    DrawNode* m_pCheckMark;
    Label* m_pTextRenderer;
    ActionEvent m_onAction;
};

} // namespace ui

} // namespace ocf