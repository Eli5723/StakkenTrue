#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <algorithm>

namespace Engine {
namespace Assets {

struct SimpleAtlas
    {
        const int WIDTH = 2048;
        const int HEIGHT = 2048;
        
        // Storage
        GLuint m_atlasID;

        // State
        int cursor_x;
        int cursor_y;
        int current_row_max;

        SimpleAtlas()
        {
            glGenTextures(1, &m_atlasID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_atlasID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

            cursor_x = 0;
            cursor_y = 0;
            current_row_max = 0;
        }

        // Insert a single channel bitmap and output UV
        void insert(unsigned char* bitmap, int width, int height, glm::vec4& uv) {
            if (cursor_x + width > WIDTH) {
                cursor_x = 0;
                cursor_y += current_row_max;
                current_row_max = 0;
            }

            // Update Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_atlasID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexSubImage2D(GL_TEXTURE_2D, 0, cursor_x, cursor_y, width, height, GL_RED, GL_UNSIGNED_BYTE, bitmap);

            // Set output UV
            uv.r = (float)(cursor_x) / (float)WIDTH; // Left
            uv.g = (float)(cursor_y) / (float)HEIGHT; // Top
            uv.b = (float)(cursor_x + width) / (float)WIDTH; // Right
            uv.a = (float)(cursor_y + height) / (float)HEIGHT; // Bottom
            
            // Advance Cursor
            cursor_x += width;
            current_row_max = std::max(current_row_max, height);
        }
    };
}
}