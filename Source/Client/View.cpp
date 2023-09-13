#include <View.hpp>

#include <Draw/Framebuffer.hpp>
#include <GL/gl3w.h>

namespace Engine::View {
    
    Engine::Draw::Framebuffer* m_Framebuffer;
    glm::vec2 m_Size;
    GLuint m_QuadVAO;
    GLuint m_QuadVBO;
    GLuint shader;

    // Shader
    const char* m_VertexShaderSource = R"(
        #version 330 core

        layout (location = 0) in vec2 a_Position;
        layout (location = 1) in vec2 a_TexCoord;

        out vec2 v_TexCoord;

        void main() {
            gl_Position = vec4(a_Position, 0.0, 1.0);
            v_TexCoord = a_TexCoord;
        }
    )";

    const char* m_FragmentShaderSource = R"(
        #version 330 core

        in vec2 v_TexCoord;

        out vec4 FragColor;

        uniform sampler2D u_Texture;

        void main() {
            FragColor = texture(u_Texture, v_TexCoord);
        }
    )";

    void Initialize(const glm::vec2& resolution) {
        m_Framebuffer = new Engine::Draw::Framebuffer(resolution);

        // Set up simple quad for rendering framebuffer
        struct Vertex {
            glm::vec2 position;
            glm::vec2 texCoord;
        };

        Vertex vertices[] = {
            { glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
            { glm::vec2( 1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec2( 1.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
            { glm::vec2(-1.0f,  1.0f), glm::vec2(0.0f, 1.0f) }
        };

        glGenVertexArrays(1, &m_QuadVAO);
        glBindVertexArray(m_QuadVAO);

        glGenBuffers(1, &m_QuadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    
        // Set up shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &m_VertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &m_FragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        shader = glCreateProgram();
        glAttachShader(shader, vertexShader);
        glAttachShader(shader, fragmentShader);
        glLinkProgram(shader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, "u_Texture"), 0);
    }

    void Resize(const glm::vec2& size) {
        m_Size = size;
        m_Framebuffer->Resize(m_Size);
    }

    void StartFrame() {
        m_Framebuffer->Bind();
        glViewport(0, 0, m_Size.x, m_Size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void EndFrame() {
        m_Framebuffer->Unbind();
        glViewport(0, 0, m_Size.x, m_Size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorAttachment());

        glBindVertexArray(m_QuadVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}