#pragma once
#include "ocf/ui/UIButtonBase.h"

namespace ocf {

class Sprite;
class Label;

namespace ui {

class Button : public ButtonBase {
public:
    static Button* create();
    static Button* create(std::string_view text);
    static Button* create(std::string_view normalImage, std::string_view activeImage);

    Button();
    virtual ~Button();

    bool init() override;
    bool init(std::string_view normalImage, std::string_view activeImage);

protected:
    void initRenderer() override;
    void updateTextLocation() override;
    void onMouseClicked() override;
    void onSetFocus() override;
    void onKillFocus() override;

protected:
    Sprite* m_pButtonNormalRenderer;
    Sprite* m_pButtonActiveRenderer;
    std::string m_normalFilename;
    std::string m_activeFilename;
};

}

} // namespace ocf
