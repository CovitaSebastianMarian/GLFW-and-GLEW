#pragma once
#include "Library.h"

using namespace std;


class Object {
private:
    unsigned int shader, VAO, VBO;

    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "uniform mat4 Matrix;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = Matrix * vec4(aPos,1, 1);\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 inColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = inColor;\n"
        "}\0";
    int sze = 0;
public:
    vector<float> vertices;
    float width = 0.0f, height = 0.0f;
    Object() {
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        shader = glCreateProgram();
        glAttachShader(shader, vertexShader);
        glAttachShader(shader, fragmentShader);
        glLinkProgram(shader);


        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }

    inline void ConfigDrawTool() {
        sze = vertices.size() / 2;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    inline void Draw(double w, double h, GLenum mode, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
        glUseProgram(shader);
        GLint transformLoc = glGetUniformLocation(shader, "inColor");
        glUniform4f(transformLoc, r, g, b, a);

        glm::mat4 matrix(1.0);
        //matrix = glm::ortho(0.0, (double)width, 0.0, (double)height);
        matrix = glm::ortho(0.0, w, h, 0.0);
        int location = glGetUniformLocation(shader, "Matrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

        
        glBindVertexArray(VAO);
        glDrawArrays(mode, 0, sze);
        glBindVertexArray(0);
    }
    







};