#include <Draw/Text.hpp>

#include <spdlog/spdlog.h>

namespace Engine {
namespace Draw {
namespace Text {

void Immediate(const glm::vec2& position, const char* text, const Assets::Font& font) {
    Quad::EnableTextShader();
    Quad::BeginBatch();

    glm::vec2 cursor_position{0, font.baseline};
    int kerningAdvance;

    while (*text)
    {
        int codepoint = *text;
        
        if (codepoint == '\n'){
            cursor_position.x = 0;
            cursor_position.y += font.linegap;
            text++;
            continue;
        }        
        const Assets::Font::Glyph &glyph = font.ascii_cache[codepoint];
        // TODO: Kerning pairs
        // int nextCodepoint = *(text + 1);
        // int nextFontGlyph = stbtt_FindGlyphIndex(&font.font, nextCodepoint);
        // int kerningAdvance = stbtt_GetGlyphKernAdvance(&font.font, glyph.glyph, nextFontGlyph);
        const glm::vec2 glyph_position = position + cursor_position + glm::vec2{glyph.lsb - glyph.offsetX, glyph.offsetY};
        const glm::vec2 glyph_size = {glyph.width, glyph.height};
        Quad::TexturedUV(glyph_position, glyph_size, (u32)font.atlas.m_atlasID, glyph.UV);
        cursor_position.x += (float)(glyph.advance);

        text++;
    }

    Quad::Finish();
}

}
}
}