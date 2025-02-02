#pragma once
#ifndef SEBAPI
#define SEBAPI 1
#define SEBCALL __stdcall
#define SEBALWAYSUSE [[nodiscard]]
#define SEBNORETURN [[noreturn]]
#define SEBNOEXCEPTION noexcept
#define SEBFUNC inline decltype(auto)
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
namespace seb {
    SEBCALL inline void check_gl_error(const std::string& function_name) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL error in " << function_name << ": " << err << std::endl;
        }
    }
    class Shader {
    public:
        unsigned int ID;
        SEBCALL inline void open(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
            std::string vertexCode;
            std::string fragmentCode;
            std::string geometryCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            std::ifstream gShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {

                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
                if (geometryPath != nullptr) {
                    gShaderFile.open(geometryPath);
                    std::stringstream gShaderStream;
                    gShaderStream << gShaderFile.rdbuf();
                    gShaderFile.close();
                    geometryCode = gShaderStream.str();
                }
            }
            catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();
            unsigned int vertex, fragment;
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");
            unsigned int geometry;
            if (geometryPath != nullptr) {
                const char* gShaderCode = geometryCode.c_str();
                geometry = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geometry, 1, &gShaderCode, NULL);
                glCompileShader(geometry);
                checkCompileErrors(geometry, "GEOMETRY");
            }
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            if (geometryPath != nullptr)
                glAttachShader(ID, geometry);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            if (geometryPath != nullptr)
                glDeleteShader(geometry);
        }
        SEBCALL inline void bind() const {
            glUseProgram(ID);
        }
        SEBCALL inline void unbind() const {
            glUseProgram(0);
        }
        SEBCALL inline void set_bool(const std::string& name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        SEBCALL inline void set_int(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        SEBCALL inline void set_float(const std::string& name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        SEBCALL inline void set_vec2(const std::string& name, const glm::vec2& value) const {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        SEBCALL inline void set_vec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }
        SEBCALL inline void set_vec3(const std::string& name, const glm::vec3& value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        SEBCALL inline void set_vec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }
        SEBCALL inline void set_vec4(const std::string& name, const glm::vec4& value) const {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        SEBCALL inline void set_vec4(const std::string& name, float x, float y, float z, float w) {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }
        SEBCALL inline void set_mat2(const std::string& name, const glm::mat2& mat) const {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        SEBCALL inline void set_mat3(const std::string& name, const glm::mat3& mat) const {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        SEBCALL inline void set_mat4(const std::string& name, const glm::mat4& mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

    private:
        SEBCALL void inline checkCompileErrors(GLuint shader, std::string type) const {
            GLint success;
            GLchar infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };

    class Texture {
    public:
        SEBCALL inline void open(const char* filename, const int format) {
            int width, height, nrChannels;
            unsigned char* pixels;
            pixels = stbi_load(filename, &width, &height, &nrChannels, 0);
            if (!pixels) {
                std::cout << "Failed to load texture: " << filename << std::endl;
                return;
            }
            glGenTextures(1, &id);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(0);
            stbi_image_free(pixels);
        }
        SEBCALL inline void bind(const unsigned int unit) const {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, id);
        }
        SEBCALL inline void unbind() const {
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(0);
        }
        unsigned int id;
    };


    class Object2D {
    public:
        SEBCALL SEBALWAYSUSE inline glm::vec2 get_position() const {
            return position;
        }
        SEBCALL SEBALWAYSUSE inline glm::mat4 get_matrix() {
            matrix = glm::ortho<double>(orthographic.x, orthographic.y, orthographic.z, orthographic.w, 0, 0.1);
            matrix = glm::translate(matrix, glm::vec3(this->position, 0));
            // while (this->angle >= 360.f) this->angle -= 360.f;
            matrix = glm::rotate(matrix, glm::radians(this->angle), glm::vec3(0, 0, 1));
            matrix = glm::scale(matrix, glm::vec3(this->scale, 0));
            return matrix;
        }
        SEBCALL SEBALWAYSUSE inline glm::vec2 get_scale() const {
            return scale;
        }
        SEBCALL SEBALWAYSUSE inline float get_angle() const {
            return angle;
        }
        SEBCALL SEBALWAYSUSE inline const float* get_buffer_data(unsigned int index) const {
            return vbo_data[index].data();
        }
        SEBCALL SEBALWAYSUSE inline const unsigned long long get_buffer_data_size(unsigned int index) const {
            return vbo_data[index].size();
        }
        SEBCALL SEBALWAYSUSE inline const unsigned int* get_elements_data() const {
            return ebo_data.data();
        }
        SEBCALL SEBALWAYSUSE inline const unsigned long long get_elements_data_size() const {
            return ebo_data.size();
        }
        SEBCALL inline void draw() const {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, ebo_data.size(), GL_UNSIGNED_INT, NULL);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        SEBCALL inline void set_orthographic(const double left, const double right, const double bottom, const double top) {
            this->orthographic = glm::vec4(left, right, bottom, top);
        }
        SEBCALL inline void set_position(const glm::vec2& position) {
            this->position = position;
        }
        SEBCALL inline void move(const glm::vec2& position) {
            this->position += position;
        }
        SEBCALL inline void set_angle(const float& angle) {
            this->angle = angle;
        }
        SEBCALL inline void set_angle_from_point(const float& angle, glm::vec2 point = glm::vec2(0)) {
            this->angle = angle;
            float xx = position.x - point.x;
            float yy = position.y - point.y;
            float ang = angle * 3.1415f / 180.f;
            float x = cos(ang) * xx - sin(ang) * yy + point.x;
            float y = sin(ang) * xx + cos(ang) * yy + point.y;
            position = glm::vec2(x, y);
        }
        SEBCALL inline void rotate(const float& angle) {
            this->angle += angle;
        }
        SEBCALL inline void rotate_from_point(const float& angle, glm::vec2 point = glm::vec2(0)) {
            this->angle += angle;
            float xx = position.x - point.x;
            float yy = position.y - point.y;
            float ang = angle * 3.1415f / 180.f;
            float x = cos(ang) * xx - sin(ang) * yy + point.x;
            float y = sin(ang) * xx + cos(ang) * yy + point.y;
            position = glm::vec2(x, y);
        }
        SEBCALL inline void set_scale(const glm::vec2 scale) {
            this->scale = scale;
        }
        SEBCALL inline std::vector<float> get_vbo_data(const unsigned short index) const {
            return vbo_data[index];
        }

    protected:
        unsigned int VAO, EBO;
        std::vector<unsigned int> VBO;

        std::vector<std::vector<float>> vbo_data;
        std::vector<unsigned int> ebo_data;

        glm::vec4 orthographic = glm::vec4(0);
        glm::vec2 position = glm::vec2(0);
        float angle = 0.f;
        glm::vec2 scale = glm::vec2(0);
        glm::mat4 matrix = glm::mat4(1);

        SEBCALL inline void nondsa_generate_buffers(unsigned int size) {
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            VBO.resize(size);
            glGenBuffers(size, VBO.data());
            glGenBuffers(1, &EBO);
            glBindVertexArray(0);
        }
        SEBCALL inline void nondsa_initialize_vertex_buffer(unsigned int vbo_id, unsigned short _size, unsigned int usage) const {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_id]);
            glBufferData(GL_ARRAY_BUFFER, vbo_data[vbo_id].size() * sizeof(float), vbo_data[vbo_id].data(), usage);
            glEnableVertexAttribArray(vbo_id);
            glVertexAttribPointer(vbo_id, _size, GL_FLOAT, GL_FALSE, _size * sizeof(float), (void*)(0));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        SEBCALL inline void nondsa_update_vertex_buffer(unsigned int vbo_id, unsigned short _size, unsigned int usage) const {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[vbo_id]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vbo_data[vbo_id].size() * sizeof(float), vbo_data[vbo_id].data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        SEBCALL inline void nondsa_initialize_elements_buffer(unsigned int usage) const {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data.size() * sizeof(unsigned int), ebo_data.data(), usage);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        SEBCALL inline void nondsa_update_elements_buffer(unsigned int usage) const {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ebo_data.size() * sizeof(unsigned int), ebo_data.data());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }


        SEBCALL inline void dsa_generate_buffers(unsigned int size) {
            glCreateVertexArrays(1, &VAO);
            VBO.resize(size);
            glCreateBuffers(size, VBO.data());
            glCreateBuffers(1, &EBO);
        }
        SEBCALL inline void dsa_initialize_vertex_buffer(unsigned int vbo_id, unsigned short _size, unsigned int usage) const {
            glNamedBufferData(VBO[vbo_id], vbo_data[vbo_id].size() * sizeof(float), vbo_data[vbo_id].data(), usage);
            glEnableVertexArrayAttrib(VAO, vbo_id);
            glVertexArrayAttribFormat(VAO, vbo_id, _size, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(VAO, vbo_id, vbo_id);
            glVertexArrayVertexBuffer(VAO, vbo_id, VBO[vbo_id], 0, _size * sizeof(float));
        }
        SEBCALL inline void dsa_update_vertex_buffer(unsigned int vbo_id, unsigned short _size, unsigned int usage) const {
            glNamedBufferSubData(VBO[vbo_id], 0, vbo_data[vbo_id].size() * sizeof(float), vbo_data[vbo_id].data());
        }
        SEBCALL inline void dsa_initialize_elements_buffer(unsigned int usage) const {
            glNamedBufferData(EBO, ebo_data.size() * sizeof(unsigned int), ebo_data.data(), usage);
            glVertexArrayElementBuffer(VAO, EBO);
        }
        SEBCALL inline void dsa_update_elements_buffer(unsigned int usage) const {
            glNamedBufferSubData(EBO, 0, ebo_data.size() * sizeof(unsigned int), ebo_data.data());
        }
    };
}
#endif
