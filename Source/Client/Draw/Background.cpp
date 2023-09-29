#include <Draw/Background.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <GL/gl3w.h>
#include <defines.h>

#include <chrono>

namespace Engine::Draw::Background
{

    // Vertex Shader that takes three control points and raises the height of each vertex based on the distance from the points
    static const char *vertexShader = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 projection;
        uniform mat4 view;

        uniform vec3 controlPoints[3];        
        uniform float time;

        float distanceToControlPoint(vec3 point) {
            return sqrt(pow(point.x - aPos.x, 2) + pow(point.y - aPos.y, 2) + pow(point.z - aPos.z, 2));
        }

        void main()
        {
            const float peak = 2.0;
            const float width = 4.0; 
            float ring_size = mod(time, 80);

            float offset =  0.0;
            for (int i = 0; i < 3; i++) {
                float control_distance = distanceToControlPoint(controlPoints[i]);
                offset = max(sin(control_distance*.25 - time*.25), 0);

                float proximity = min(width, abs(control_distance - ring_size));
                float factor = (width - proximity) / width;

                // offset = max(factor * peak, offset);
                // offset += factor * peak;
            }

            gl_Position = projection * view * vec4(aPos.x, aPos.y + offset, aPos.z, 1.0);
        }
    )";

    static const char* fragmentShader = R"(
        #version 330 core

        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0f, 1.0f, 1.0f, .4f);
        }
    )";

    glm::mat4 view_matrix;
    glm::vec3 control_points[3] = {
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 2)
    };

    glm::vec3 cameraPositon;
    glm::vec3 cameraTarget;
    glm::vec3 cameraDirection;

    const int vertex_x_count = 140;
    const int vertex_y_count = 120;

    const int vertex_count = vertex_x_count * vertex_y_count;
    const int line_count = (vertex_x_count - 1) * (vertex_y_count -1) * 2;
    
    glm::vec3 points[vertex_x_count * vertex_y_count];
    
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;

    void Init(glm::vec2 resolution) {
        // Set up the shaders
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderID, 1, &vertexShader, NULL);
        glCompileShader(vertexShaderID);

        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderID, 1, &fragmentShader, NULL);
        glCompileShader(fragmentShaderID);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShaderID);
        glAttachShader(shaderProgram, fragmentShaderID);
        glLinkProgram(shaderProgram);

        // Create a vertex for each point on the grid
        for (int x = 0; x < vertex_x_count; x++) {
            for (int y = 0; y < vertex_y_count; y++) {
                points[x + y * vertex_x_count] = glm::vec3(x, 0, y);
            }
        }

        // Connect each point to form a grid
        GLuint indices[(vertex_x_count - 1) * (vertex_y_count - 1) * 4];
        for (int x = 0; x < vertex_x_count - 2; x++) {
            for (int y = 0; y < vertex_y_count - 2; y++) {
                const int vertex_index = x + y * vertex_x_count;

                indices[vertex_index * 4 + 0] = x + (y + 0) * vertex_x_count;
                indices[vertex_index * 4 + 1] = x + (y + 1) * vertex_x_count;
                indices[vertex_index * 4 + 2] = (x + 0) + y * vertex_x_count;
                indices[vertex_index * 4 + 3] = (x + 1) + y * vertex_x_count;
            }
        }

        // Set up the vertex array object
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Set up the vertex buffer object
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Send Vertices to GPU
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set up the element buffer object
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Set up view matrix to look at grid from above 
        glm::mat4x4 view_matrix = glm::lookAt(
            glm::vec3(70, 10, -10), // Camera is at (0,10,0), in World Space
            glm::vec3(70, 0, 20),  // and looks at the origin
            glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
        );

        glUseProgram(shaderProgram);
        SetResolution(resolution);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, false, &view_matrix[0][0]);

        // Set random control points
        control_points[0] = glm::vec3(40.0f, 0, 40.0f);
        control_points[1] = glm::vec3(40.0f, 0, 40.0f);
        control_points[2] = glm::vec3(40.0f, 0, 40.0f);

        glUniform3fv(glGetUniformLocation(shaderProgram, "controlPoints"), 3, &control_points[0][0]);
    }

    int ctime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    void Draw(){ 
        // Clear to light blue
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glUseProgram(shaderProgram);
        auto start = std::chrono::system_clock::now();

        float time = (float)(ctime() % 100000) / 1000.0f;
        glUniform1f(glGetUniformLocation(shaderProgram, "time"), time*8.0f);

                // Bind VAO 
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES,vertex_x_count * vertex_y_count * 4, GL_UNSIGNED_INT, 0);
    }

    void SetResolution(const glm::vec2 &resolution) {
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), resolution.x / resolution.y, 0.1f, 1000.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, false, &proj[0][0]);
    }
}