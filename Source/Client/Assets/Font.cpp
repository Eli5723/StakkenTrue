#include <Assets/Font.hpp>

#include <spdlog/spdlog.h>

#include <memory.h>
#include <algorithm>

namespace Engine
{
    namespace Assets
    {

        Font::Font() : atlas(), font{} {};

        Font *Font::Load(const char *path)
        {
            Font *font = new Font();
            font->LoadFile(path);
            if (!font->loaded)
                return 0;
            return font;
        }

        void Font::LoadFile(const char *path)
        {
            const int font_size = 24;

            font_file = (const unsigned char *)File::slurpBytes(path);
            if (!stbtt_InitFont(&font, font_file, 0))
            {
                spdlog::info("stbtt failed to initialize font: {}", path);
                return;
            }

            int i_descent;
            int i_linegap;

            scale = stbtt_ScaleForPixelHeight(&font, font_size);
            stbtt_GetFontVMetrics(&font, &ascent, &i_descent, &i_linegap);
            baseline = ascent * scale;
            descent = i_descent * scale * -1;
            linegap = (i_linegap * scale) + baseline + descent;
            LoadAscii();

            loaded = true;
        }

        void Font::LoadAscii()
        {
            int glyph = 0;
            int width, height, offsetX, offsetY, advance, lsb;
            memset(ascii_cache, 0, sizeof(ascii_cache));

            // Generate Invalid Glpyh
            {
                u8 *bitmap = stbtt_GetGlyphBitmap(&font, scale, scale, 0, &width, &height, &offsetX, &offsetY);
                stbtt_GetGlyphHMetrics(&font, glyph, &advance, &lsb);
                atlas.insert(bitmap, width, height, ascii_cache[0].UV);
                ascii_cache[0].advance = advance * scale;
                ascii_cache[0].lsb = lsb * scale;
                ascii_cache[0].codepoint = 0;
                ascii_cache[0].glyph = glyph;
                ascii_cache[0].width = offsetY;
                ascii_cache[0].height = offsetY;
                ascii_cache[0].offsetX = offsetX;
                ascii_cache[0].offsetY = offsetY;
            }

            for (int codepoint = 1; codepoint < 128; codepoint++)
            {
                glyph = stbtt_FindGlyphIndex(&font, codepoint);
                stbtt_GetGlyphHMetrics(&font, glyph, &advance, &lsb);

                if (!glyph)
                {
                    ascii_cache[codepoint].advance = ascii_cache[0].advance * scale;
                    ascii_cache[codepoint].lsb = ascii_cache[0].lsb * scale;
                    ascii_cache[codepoint].codepoint = ascii_cache[0].codepoint;
                    ascii_cache[codepoint].glyph = ascii_cache[0].glyph;
                    ascii_cache[codepoint].width = ascii_cache[0].width;
                    ascii_cache[codepoint].height = ascii_cache[0].height;
                    ascii_cache[codepoint].offsetX = ascii_cache[0].offsetX;
                    ascii_cache[codepoint].offsetY = ascii_cache[0].offsetY;
                    continue;
                }

                u8 *bitmap = stbtt_GetGlyphBitmap(&font, scale, scale, glyph, &width, &height, &offsetX, &offsetY);
                atlas.insert(bitmap, width, height, ascii_cache[codepoint].UV);
                ascii_cache[codepoint].advance = advance * scale;
                ascii_cache[codepoint].lsb = lsb * scale;
                ascii_cache[codepoint].codepoint = codepoint;
                ascii_cache[codepoint].glyph = glyph;
                ascii_cache[codepoint].width = width;
                ascii_cache[codepoint].height = height;
                ascii_cache[codepoint].offsetX = offsetX * scale;
                ascii_cache[codepoint].offsetY = offsetY;
            }
        }
    }
}