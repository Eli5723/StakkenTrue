#pragma once

#include <Assets/SimpleAtlas.hpp>
#include <defines.h>

namespace Engine::Assets {
    struct MsdfFont {
        static void Init();

        // Character Information substruct
        struct Glyph {
            bool valid = false;
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

        // Font Information
        Glyph ascii_cache[128];
        float scale;
        int ascent;
        int baseline;
        int descent;
        int linegap;

        // Font Metadata
        const u8* font_file;

        SimpleAtlas atlas;
        MsdfFont* Load(const char* path);
    };


}