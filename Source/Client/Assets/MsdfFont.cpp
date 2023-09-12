#include <Assets/MsdfFont.hpp>

#include <msdfgen.h>
#include <msdfgen-ext.h>

namespace Engine::Assets {
    msdfgen::FreetypeHandle *ft;

    void MsdfFont::Init() {
        ft = msdfgen::initializeFreetype();
    }

    MsdfFont *MsdfFont::Load(const char *path) {
        msdfgen::FontHandle *font = msdfgen::loadFont(ft, path);
        if (!font) {
            return nullptr;
        }

        msdfgen::Shape shape;

        // Attempt to load the Ascii character set
        for (int i = 0; i < 128; i++) {
            if (msdfgen::loadGlyph(shape, font, i)) {
                shape.normalize();
                msdfgen::edgeColoringSimple(shape, 3.0);
                msdfgen::Bitmap<float, 3> msdf(32, 32);
                msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(0.5, 0.5));
                u8 *bitmap = new u8[32 * 32 * 3];
                // Convert Floating point bitmap to u8 bitmap
                for (int x = 0; x < 32; x++) {
                    for (int y = 0; y < 32; y++) {
                        bitmap[(x + (y * 32)) * 3 + 0] = (u8) (msdf(x, y)[0] * 255);
                        bitmap[(x + (y * 32)) * 3 + 1] = (u8) (msdf(x, y)[1] * 255);
                        bitmap[(x + (y * 32)) * 3 + 2] = (u8) (msdf(x, y)[2] * 255);
                    }
                }
            }
        }
        msdfgen::loadGlyph(shape, font, 'A');

        return nullptr;
    }
}