#pragma once

#include <glm/glm.hpp>

namespace Engine::Draw::Line {
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

    void Line(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
}