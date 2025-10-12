#pragma once
#include "ocf/2d/Node2D.h"
#include "ocf/base/Types.h"
#include "ocf/base/Macros.h"
#include "ocf/renderer/QuadCommand.h"
#include "ocf/renderer/Material.h"

#include <string>
#include <vector>

namespace ocf {

class Font;
class FontAtlas;

class Label : public Node2D {
public:
    enum class LabelType {
        BMFONT,
        TTF,
    };

    static Label* create(const std::string& text);
    static Label* createWithBMFont(std::string_view bmFontPath, std::string_view text);

    Label();
    virtual ~Label();

    bool init();
    bool initWithBMFont(std::string_view bmFontPath);

    Font* getFont() const { return m_font; }

    void setString(std::string_view text);
    std::string getString() const { return m_text; }
    void setTextColor(const math::vec3& textColor);
    void setTextColor(unsigned char r, unsigned char g, unsigned char b);
    const math::vec3& getTextColor() const { return m_textColor; }

    void update(float deltaTime) override;

    void draw(Renderer* renderer, const math::mat4& transform) override;

protected:
    void updateContent();
    void updateQuads();
    void updateBatchCommands();

    void setProgram(Program* program);

    struct BatchCommand {
        QuadCommand quadCommand;
        std::vector<QuadV3fC3fT2f> quads;
        std::vector<unsigned short> indices;
        Texture* texture;
        Material material;

        BatchCommand();
        void clear();
        void insertQuad(const QuadV3fC3fT2f& quad);
    };

protected:
    Font* m_font;
    LabelType m_labelType;
    std::string m_text;
    std::u32string m_utf32Text;
    math::vec3 m_textColor;
    bool m_isDirty;
    FontAtlas* m_fontAtlas = nullptr;
    std::vector<BatchCommand> m_batchCommands;
};

} // namespace ocf
