#pragma once
#ifdef SEBAPI
namespace seb {
    class Object2D {
    public:
        SEBCALL SEBALWAYSUSE inline glm::vec2 get_position() const {
            return position;
        }
        SEBCALL SEBALWAYSUSE inline glm::mat4 get_transform_matrix() {
            matrix = glm::ortho<double>(left, right, bottom, top, 0, 0.1);
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
            this->left = left;
            this->right = right;
            this->bottom = bottom;
            this->top = top;
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

        double left, right, bottom, top;
        glm::vec2 position = glm::vec2(0);
        float angle = 0.f;
        glm::vec2 scale = glm::vec2(1);
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


    class Quad2D : public Object2D {
    public:
        void create() {
            vbo_data.resize(2);
            vbo_data[0] = {
                -1.0f, -1.0f,
                1.0f, -1.0f,
                1.0f,  1.0f,
                -1.0f,  1.0f
            };
            vbo_data[1] = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
            };
            ebo_data = { 0, 1, 2, 2, 3, 0 };
            nondsa_generate_buffers(2);
            nondsa_initialize_vertex_buffer(0, 2, GL_STATIC_DRAW);
            nondsa_initialize_vertex_buffer(1, 2, GL_STATIC_DRAW);
            nondsa_initialize_elements_buffer(GL_STATIC_DRAW);
        }
    };
}
#endif