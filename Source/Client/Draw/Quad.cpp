#include <Draw/Quad.hpp>

#include <GL/gl3w.h>
#include <Draw/Shader.hpp>

#include <array>

namespace Engine::Draw::Quad {

static const char *vertexShader = R"(
#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexIndex;
uniform mat4 u_projection;
uniform mat4 u_transform;
out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
void main()
{
    gl_Position = u_projection * u_transform * vec4(a_Pos, 1.0);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
}
)";

static const char *normalFragmentShader = R"(
#version 330 core
out vec4 FragColor;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
uniform sampler2D u_Textures[32];
void main()
{
    int index = int(v_TexIndex);
    switch(index) {
        case 0: FragColor = texture(u_Textures[0], v_TexCoord) * v_Color; break;
        case 1: FragColor = texture(u_Textures[1], v_TexCoord) * v_Color; break;
        case 2: FragColor = texture(u_Textures[2], v_TexCoord) * v_Color; break;
        case 3: FragColor = texture(u_Textures[3], v_TexCoord) * v_Color; break;
        case 4: FragColor = texture(u_Textures[4], v_TexCoord) * v_Color; break;
        case 5: FragColor = texture(u_Textures[5], v_TexCoord) * v_Color; break;
        case 6: FragColor = texture(u_Textures[6], v_TexCoord) * v_Color; break;
        case 7: FragColor = texture(u_Textures[7], v_TexCoord) * v_Color; break;
        case 8: FragColor = texture(u_Textures[8], v_TexCoord) * v_Color; break;
        case 9: FragColor = texture(u_Textures[9], v_TexCoord) * v_Color; break;
        case 10: FragColor = texture(u_Textures[10], v_TexCoord) * v_Color; break;
        case 11: FragColor = texture(u_Textures[11], v_TexCoord) * v_Color; break;
        case 12: FragColor = texture(u_Textures[12], v_TexCoord) * v_Color; break;
        case 13: FragColor = texture(u_Textures[13], v_TexCoord) * v_Color; break;
        case 14: FragColor = texture(u_Textures[14], v_TexCoord) * v_Color; break;
        case 15: FragColor = texture(u_Textures[15], v_TexCoord) * v_Color; break;
        case 16: FragColor = texture(u_Textures[16], v_TexCoord) * v_Color; break;
        case 17: FragColor = texture(u_Textures[17], v_TexCoord) * v_Color; break;
        case 18: FragColor = texture(u_Textures[18], v_TexCoord) * v_Color; break;
        case 19: FragColor = texture(u_Textures[19], v_TexCoord) * v_Color; break;
        case 20: FragColor = texture(u_Textures[20], v_TexCoord) * v_Color; break;
        case 21: FragColor = texture(u_Textures[21], v_TexCoord) * v_Color; break;
        case 22: FragColor = texture(u_Textures[22], v_TexCoord) * v_Color; break;
        case 23: FragColor = texture(u_Textures[23], v_TexCoord) * v_Color; break;
        case 24: FragColor = texture(u_Textures[24], v_TexCoord) * v_Color; break;
        case 25: FragColor = texture(u_Textures[25], v_TexCoord) * v_Color; break;
        case 26: FragColor = texture(u_Textures[26], v_TexCoord) * v_Color; break;
        case 27: FragColor = texture(u_Textures[27], v_TexCoord) * v_Color; break;
        case 28: FragColor = texture(u_Textures[28], v_TexCoord) * v_Color; break;
        case 29: FragColor = texture(u_Textures[29], v_TexCoord) * v_Color; break;
        case 30: FragColor = texture(u_Textures[30], v_TexCoord) * v_Color; break;
        case 31: FragColor = texture(u_Textures[31], v_TexCoord) * v_Color; break;
    }
}
)";

static const char *textFragmentShader = R"(
#version 330 core
out vec4 FragColor;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
uniform sampler2D u_Textures[32];
void main()
{
    int index = int(v_TexIndex);
    float alpha = 0.0;
    switch(index) {
        case 0: alpha = texture(u_Textures[0], v_TexCoord).r; break;
        case 1: alpha = texture(u_Textures[1], v_TexCoord).r; break;
        case 2: alpha = texture(u_Textures[2], v_TexCoord).r; break;
        case 3: alpha = texture(u_Textures[3], v_TexCoord).r; break;
        case 4: alpha = texture(u_Textures[4], v_TexCoord).r; break;
        case 5: alpha = texture(u_Textures[5], v_TexCoord).r; break;
        case 6: alpha = texture(u_Textures[6], v_TexCoord).r; break;
        case 7: alpha = texture(u_Textures[7], v_TexCoord).r; break;
        case 8: alpha = texture(u_Textures[8], v_TexCoord).r; break;
        case 9: alpha = texture(u_Textures[9], v_TexCoord).r; break;
        case 10: alpha = texture(u_Textures[10], v_TexCoord).r; break;
        case 11: alpha = texture(u_Textures[11], v_TexCoord).r; break;
        case 12: alpha = texture(u_Textures[12], v_TexCoord).r; break;
        case 13: alpha = texture(u_Textures[13], v_TexCoord).r; break;
        case 14: alpha = texture(u_Textures[14], v_TexCoord).r; break;
        case 15: alpha = texture(u_Textures[15], v_TexCoord).r; break;
        case 16: alpha = texture(u_Textures[16], v_TexCoord).r; break;
        case 17: alpha = texture(u_Textures[17], v_TexCoord).r; break;
        case 18: alpha = texture(u_Textures[18], v_TexCoord).r; break;
        case 19: alpha = texture(u_Textures[19], v_TexCoord).r; break;
        case 20: alpha = texture(u_Textures[20], v_TexCoord).r; break;
        case 21: alpha = texture(u_Textures[21], v_TexCoord).r; break;
        case 22: alpha = texture(u_Textures[22], v_TexCoord).r; break;
        case 23: alpha = texture(u_Textures[23], v_TexCoord).r; break;
        case 24: alpha = texture(u_Textures[24], v_TexCoord).r; break;
        case 25: alpha = texture(u_Textures[25], v_TexCoord).r; break;
        case 26: alpha = texture(u_Textures[26], v_TexCoord).r; break;
        case 27: alpha = texture(u_Textures[27], v_TexCoord).r; break;
        case 28: alpha = texture(u_Textures[28], v_TexCoord).r; break;
        case 29: alpha = texture(u_Textures[29], v_TexCoord).r; break;
        case 30: alpha = texture(u_Textures[30], v_TexCoord).r; break;
        case 31: alpha = texture(u_Textures[31], v_TexCoord).r; break;
    }

    FragColor = v_Color * alpha;
}
)";

static const u32 MaxQuadCount = 1000;
static const u32 MaxVertexCount = MaxQuadCount * 4;
static const u32 MaxIndexCount = MaxQuadCount * 6;
static const u32 MaxTextures = 32;

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    GLfloat texIndex;
};

static struct RendererData
{
    Shader *current_shader;
    Shader *normal_shader;
    Shader *text_shader;

    GLuint QuadVA = 0;
    GLuint QuadVB = 0;
    GLuint QuadIB = 0;
    GLuint WhiteTexture = 0;
    uint32_t WhiteTextureSlot = 0;

    uint32_t IndexCount;

    Vertex *QuadBuffer = nullptr;
    Vertex *QuadBufferPtr = nullptr;

    std::array<uint32_t, MaxTextures> TextureSlots;
    uint32_t TextureSlotIndex = 1;
} renderer_data;
 
void Setup()
{
    renderer_data.normal_shader = new Shader(normalFragmentShader, vertexShader);
    renderer_data.normal_shader->fillSamplers();
    renderer_data.text_shader = new Shader(textFragmentShader, vertexShader);
    renderer_data.text_shader->fillSamplers();

    // Default renderer State
    EnableNormalShader();
    SetTransformationMatrix(glm::mat4(1.0f));

    renderer_data.QuadBuffer = new Vertex[MaxVertexCount];

    glGenVertexArrays(1, &renderer_data.QuadVA);
    glBindVertexArray(renderer_data.QuadVA);

    glGenBuffers(1, &renderer_data.QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_data.QuadVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxVertexCount, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texIndex));

    uint32_t indices[MaxIndexCount];

    uint32_t offset = 0;
    for (unsigned int i = 0; i < MaxIndexCount; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glGenBuffers(1, &renderer_data.QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_data.QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(1, &renderer_data.WhiteTexture);
    glBindTexture(GL_TEXTURE_2D, renderer_data.WhiteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t color = 0xFFFFFFFF;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    renderer_data.TextureSlots[0] = renderer_data.WhiteTexture;
    for (unsigned int i = 1; i < MaxTextures; i++)
    {
        renderer_data.TextureSlots[i] = renderer_data.WhiteTexture;
    }
}

void SetProjectionMatrix(const glm::highp_mat4& matrix) {
    renderer_data.normal_shader->use();
    renderer_data.normal_shader->setMat4("u_projection", matrix);
    renderer_data.text_shader->use();
    renderer_data.text_shader->setMat4("u_projection", matrix);
}

void SetTransformationMatrix(const glm::highp_mat4& matrix) {
    renderer_data.normal_shader->use();
    renderer_data.normal_shader->setMat4("u_transform", matrix);
    renderer_data.text_shader->use();
    renderer_data.text_shader->setMat4("u_transform", matrix);
}

void EnableNormalShader() {
    renderer_data.current_shader = renderer_data.normal_shader;
}

void EnableTextShader() {
    renderer_data.current_shader = renderer_data.text_shader;
}

void BeginBatch()
{
    renderer_data.QuadBufferPtr = renderer_data.QuadBuffer;
    renderer_data.IndexCount = 0;
}

void EndBatch()
{
    GLsizeiptr size = (char *)renderer_data.QuadBufferPtr - (char *)renderer_data.QuadBuffer;

    glBindBuffer(GL_ARRAY_BUFFER, renderer_data.QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, renderer_data.QuadBuffer);
}

void Flush()
{
    renderer_data.current_shader->use();

    for (u32 i = 0; i < renderer_data.TextureSlotIndex; i++)
    {
        glActiveTexture((GLenum)GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderer_data.TextureSlots[i]);
    }

    glBindVertexArray(renderer_data.QuadVA);
    glDrawElements(GL_TRIANGLES, renderer_data.IndexCount, GL_UNSIGNED_INT, nullptr);

    renderer_data.IndexCount = 0;
    renderer_data.TextureSlotIndex = 1;
}

void Flat(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        // If we run out of indices or textures slots, draw everything and reset the buffers
        if (renderer_data.IndexCount >= MaxIndexCount || renderer_data.TextureSlotIndex > 31)
        {
            EndBatch();
            Flush();
            BeginBatch();
        }

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x, position.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {0.0f, 0.0f};
        renderer_data.QuadBufferPtr->texIndex = renderer_data.WhiteTextureSlot;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {1.0f, 0.0f};
        renderer_data.QuadBufferPtr->texIndex = renderer_data.WhiteTextureSlot;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {1.0f, 1.0f};
        renderer_data.QuadBufferPtr->texIndex = renderer_data.WhiteTextureSlot;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {0.0f, 1.0f};
        renderer_data.QuadBufferPtr->texIndex = renderer_data.WhiteTextureSlot;
        renderer_data.QuadBufferPtr++;

        renderer_data.IndexCount += 6;
    }

    void Border(const glm::vec2& position, const glm::vec2& size, float thickness, const glm::vec4& color) {
        Flat(glm::vec2{position.x - thickness, position.y - thickness},
             glm::vec2{thickness, size.y + thickness * 2}, color);
        Flat(glm::vec2{position.x + size.x, position.y - thickness},
             glm::vec2{thickness, size.y + thickness * 2}, color);

        Flat(glm::vec2{position.x, position.y - thickness},
             glm::vec2{size.x, thickness}, color);

        Flat(glm::vec2{position.x, position.y + size.y},
             glm::vec2{size.x, thickness}, color);
    }

    void BorderBox(const glm::vec2& position, const glm::vec2& size, float thickness, const glm::vec4& borderColor, const glm::vec4& color) {
        Border(position - glm::vec2{thickness, thickness}, size + glm::vec2{thickness*2, thickness*2}, thickness, borderColor); 
        Flat(position, size, color);
    }

    void Textured(const glm::vec2& position, const glm::vec2& size, uint32_t texture_id, const glm::vec4& color) {

        //If we run out of indices or textures slots, draw everything and reset the buffers
        if (renderer_data.IndexCount >= MaxIndexCount || renderer_data.TextureSlotIndex > 31) {
            EndBatch();
            Flush();
            BeginBatch();
        }
        
        
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < renderer_data.TextureSlotIndex; i++) {
            if (renderer_data.TextureSlots[i] == texture_id) {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            textureIndex = (float)renderer_data.TextureSlotIndex;
            renderer_data.TextureSlots[(int)textureIndex] = texture_id;
            renderer_data.TextureSlotIndex++;
        }

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x, position.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = { 0.0f,0.0f };
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = { 1.0f,0.0f };
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = { 1.0f,1.0f };
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = { 0.0f,1.0f };
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.IndexCount += 6;
    }

    void TexturedUV(const glm::vec2& position, const glm::vec2& size, const u32 texture_id, const glm::vec4 uv, const glm::vec4& color)
    {
        // If we run out of indices or textures slots, draw everything and reset the buffers
        if (renderer_data.IndexCount >= MaxIndexCount || renderer_data.TextureSlotIndex > 31)
        {
            EndBatch();
            Flush();
            BeginBatch();
        }

        // Find the first open texture slot and insert the texture
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < renderer_data.TextureSlotIndex; i++)
        {
            if (renderer_data.TextureSlots[i] == texture_id)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)renderer_data.TextureSlotIndex;
            renderer_data.TextureSlots[(int)textureIndex] = texture_id;
            renderer_data.TextureSlotIndex++;
        }

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x, position.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {uv.r, uv.g};
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {uv.b, uv.g};
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {uv.b, uv.a};
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.QuadBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0);
        renderer_data.QuadBufferPtr->color = color;
        renderer_data.QuadBufferPtr->texCoords = {uv.r, uv.a};
        renderer_data.QuadBufferPtr->texIndex = textureIndex;
        renderer_data.QuadBufferPtr++;

        renderer_data.IndexCount += 6;
    }
}