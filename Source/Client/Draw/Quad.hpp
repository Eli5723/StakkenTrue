#pragma once

#include <glm/glm.hpp>

#include <defines.h>

namespace Engine::Draw::Quad {
    void Setup();
    void Shutdown();
    
    void Flush();
    void BeginBatch();
    void EndBatch();
    inline void Finish() {
        EndBatch();
        Flush();
    } 

    void SetProjectionMatrix(const glm::highp_mat4& matrix);
    void SetTransformationMatrix(const glm::highp_mat4& matrix);

    void EnableTextShader();
    void EnableNormalShader();

    void Flat(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});
    void Border(const glm::vec2& position, const glm::vec2& size, float thickness, const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});

    void BorderBox(const glm::vec2& position, const glm::vec2& size, float thickness, const glm::vec4& borderColor, const glm::vec4& color);

    void Textured(const glm::vec2& position, const glm::vec2& size, const u32 texture_id, const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});
    void TexturedUV(const glm::vec2& position, const glm::vec2& size, const u32 texture_id, const glm::vec4 uv, const glm::vec4& color = {1.0, 1.0, 1.0, 1.0});
}