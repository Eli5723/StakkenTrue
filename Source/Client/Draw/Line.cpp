#include <Draw/Line.hpp>

#include <GL/gl3w.h>

#include <defines.h>

namespace Engine::Draw::Line {

static const char *vertexSource = R"(
#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_projection;
uniform mat4 u_transform;

out vec4 v_Color;

void main()
{
    gl_Position = u_projection * u_transform * vec4(a_Pos, 1.0);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
}
)";


static const char *fragmentSource = R"(
#version 330 core
out vec4 FragColor;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
uniform sampler2D u_Textures[32];

void main()
{
    case 0: v_Color;
)";

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
};

static struct RendererData
{  
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLuint shader;
    GLuint texture;
    Vertex *buffer;
    Vertex *bufferPtr;
    u32 indexCount;
    glm::highp_mat4 projectionMatrix;
    glm::highp_mat4 transformationMatrix;
} s_Data;

static const u32 MaxLineCount = 2000;
static const u32 MaxVertexCount = MaxLineCount * 2;
static const u32 MaxIndexCount = MaxLineCount * 2;

void Setup() {
    // Set up vertices and indices
    glGenVertexArrays(1, &s_Data.vao);
    glBindVertexArray(s_Data.vao);

    glGenBuffers(1, &s_Data.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data.vbo);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    s_Data.buffer = new Vertex[MaxVertexCount];

    u32 *indices = new u32[MaxIndexCount];

    u32 offset = 0;
    for (u32 i = 0; i < MaxIndexCount; i += 2)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;

        offset += 2;
    }

    glGenBuffers(1, &s_Data.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndexCount * sizeof(u32), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] indices;

    // Set up Shaders
    s_Data.shader = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, 0);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    glAttachShader(s_Data.shader, vertexShader);
    glAttachShader(s_Data.shader, fragmentShader);
    glLinkProgram(s_Data.shader);
    glValidateProgram(s_Data.shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shutdown() {
    glDeleteProgram(s_Data.shader);
    glDeleteBuffers(1, &s_Data.vbo);
    glDeleteBuffers(1, &s_Data.ibo);
    glDeleteVertexArrays(1, &s_Data.vao);

    delete[] s_Data.buffer;
}

void Flush() {
    glBindVertexArray(s_Data.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.ibo);
    glUseProgram(s_Data.shader);

    glDrawElements(GL_LINES, s_Data.indexCount, GL_UNSIGNED_INT, nullptr);
}

void BeginBatch() {
    s_Data.indexCount = 0;
    s_Data.bufferPtr = s_Data.buffer;
}

void EndBatch() {
    u32 dataSize = (u8 *)s_Data.bufferPtr - (u8 *)s_Data.buffer;
    glBindBuffer(GL_ARRAY_BUFFER, s_Data.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_Data.buffer);
}

void SetProjectionMatrix(const glm::highp_mat4& matrix) {
    s_Data.projectionMatrix = matrix;
    glUniformMatrix4fv(glGetUniformLocation(s_Data.shader, "u_projection"), 1, false, &matrix[0][0]);
}

void SetTransformationMatrix(const glm::highp_mat4& matrix) {
    s_Data.transformationMatrix = matrix;
    glUniformMatrix4fv(glGetUniformLocation(s_Data.shader, "u_transform"), 1, false, &matrix[0][0]);
}

void Line(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color) {
    if (s_Data.indexCount >= MaxIndexCount) {
        EndBatch();
        Flush();
        BeginBatch();
    }

    s_Data.bufferPtr->position = {start.x, start.y, 0.0f};
    s_Data.bufferPtr->color = color;
    s_Data.bufferPtr++;

    s_Data.bufferPtr->position = {end.x, end.y, 0.0f};
    s_Data.bufferPtr->color = color;
    s_Data.bufferPtr++;

    s_Data.indexCount += 2;
}

}