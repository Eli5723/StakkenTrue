#pragma once

#include <defines.h>

#include <File.h>
#include <Assets/SimpleAtlas.hpp>

#include <glm/glm.hpp>
#include <stb_truetype.h>

namespace Engine
{
    namespace Assets
    {

        struct Font
        {
            // Codepoint information, VH Metrics, UV
            struct Glyph
            {
                int codepoint;
                int glyph;
                glm::vec4 UV;
                int width;
                int height;
                int offsetX;
                int offsetY;
                int advance;
                int lsb;
            };
            Glyph ascii_cache[128];
            Glyph &INVALID = ascii_cache[0];

            // Vertical Metrics
            float scale;
            int ascent;
            int baseline;
            int descent;
            int linegap;

            // Font information
            const u8 *font_file;
            stbtt_fontinfo font;
            bool loaded = 0;

            // Graphical information
            SimpleAtlas atlas;

            Font();
            static Font *Load(const char *path);
            void LoadFile(const char *path);
            void LoadAscii();
        };
    }
}

// void Font::Draw(char *text, const glm::vec2 &position, const glm::vec4 &color)
//     {
//         glm::vec2 cursor_position{0, baseline};
//         int kerningAdvance;

//         while (*text)
//         {
//             int codepoint = *text;

//             if (codepoint == '\n'){
//                 cursor_position.x = 0;
//                 cursor_position.y += linegap;
//                 text++;
//                 continue;
//             }

//             Glyph &glyph = ascii_cache[codepoint];
//             int nextCodepoint = *(text + 1);
//             int nextFontGlyph = stbtt_FindGlyphIndex(&font, nextCodepoint);

//             int kerningAdvance = stbtt_GetGlyphKernAdvance(&font, glyph.glyph, nextFontGlyph);
//             Renderer::DrawQuadUV(position + cursor_position + glm::vec2{glyph.lsb - glyph.offsetX, glyph.offsetY}, {glyph.width, glyph.height}, atlas.m_atlasID, glyph.UV);
//             cursor_position.x += (float)(glyph.advance + kerningAdvance);

//             text++;
//         }
//     }