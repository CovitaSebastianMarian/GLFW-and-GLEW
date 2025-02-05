#pragma once
#ifdef SEBAPI
namespace seb {
    class Object3D {
    public:
        SEBCALL SEBALWAYSUSE inline glm::vec3 get_position() const {
            return position;
        }
        SEBCALL SEBALWAYSUSE inline glm::mat4 get_transform_matrix() {
            matrix = glm::mat4(1);
            matrix = glm::translate(matrix, this->position);
            
            matrix = glm::rotate(matrix, glm::radians(this->xangle), glm::vec3(1, 0, 0));
            matrix = glm::rotate(matrix, glm::radians(this->yangle), glm::vec3(0, 1, 0));
            matrix = glm::rotate(matrix, glm::radians(this->zangle), glm::vec3(0, 0, 1));

            matrix = glm::scale(matrix, this->scale);
            return matrix;
        }
        SEBCALL SEBALWAYSUSE inline glm::mat4 get_view_matrix(const glm::vec3& cameraPos, const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 1, 0)) {
            return glm::lookAt(cameraPos, target, up);
        }
        SEBCALL SEBALWAYSUSE inline glm::mat4 get_perspective_matrix(const float fov, const float aspect, const float near, const float far) {
            return glm::perspective(glm::radians(fov), aspect, near, far);
        }
        SEBCALL SEBALWAYSUSE inline glm::mat4 get_ortho_matrix(const float left, const float right, const float bottom, const float top, const float near, const float far) {
            return glm::ortho<double>(left, right, bottom, top, near, far);
        }
        SEBCALL SEBALWAYSUSE inline glm::vec3 get_scale() const {
            return scale;
        }
        SEBCALL SEBALWAYSUSE inline float get_xangle() const {
            return xangle;
        }
        SEBCALL SEBALWAYSUSE inline float get_yangle() const {
            return yangle;
        }
        SEBCALL SEBALWAYSUSE inline float get_zangle() const {
            return zangle;
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
        SEBCALL inline void set_orthographic(const double left, const double right, const double bottom, const double top, const double zNear, const double zFar) {
            this->left = left;
            this->right = right;
            this->bottom = bottom;
            this->top = top;
            this->zNear = zNear;
            this->zFar = zFar;
        }
        SEBCALL inline void set_position(const glm::vec3& position) {
            this->position = position;
        }
        SEBCALL inline void move(const glm::vec3& position) {
            this->position += position;
        }
        SEBCALL inline void set_xangle(const float& xangle) {
            this->xangle = xangle;
        }
        SEBCALL inline void set_yangle(const float& yangle) {
            this->yangle = yangle;
        }
        SEBCALL inline void set_zangle(const float& zangle) {
            this->zangle = zangle;
        }
        SEBCALL inline void rotatex(const float& xangle) {
            this->xangle += xangle;
        }
        SEBCALL inline void rotatey(const float& yangle) {
            this->yangle += yangle;
        }
        SEBCALL inline void rotatez(const float& zangle) {
            this->zangle += zangle;
        }
        SEBCALL inline void set_xangle_from_point(const float& angle, glm::vec3 point = glm::vec3(0)) {
            this->xangle = angle;
            float rad = glm::radians(angle);
            float deltaY = position.y - point.y;
            float deltaZ = position.z - point.z;
            float newY = cos(rad) * deltaY - sin(rad) * deltaZ;
            float newZ = sin(rad) * deltaY + cos(rad) * deltaZ;
            position.y = newY + point.y;
            position.z = newZ + point.z;
        }
        SEBCALL inline void set_yangle_from_point(const float& angle, glm::vec3 point = glm::vec3(0)) {
            this->yangle = angle;
            float rad = glm::radians(angle);
            float deltaX = position.x - point.x;
            float deltaZ = position.z - point.z;
            float newX = cos(rad) * deltaX + sin(rad) * deltaZ;
            float newZ = -sin(rad) * deltaX + cos(rad) * deltaZ;
            position.x = newX + point.x;
            position.z = newZ + point.z;
        }
        SEBCALL inline void set_zangle_from_point(const float& angle, glm::vec3 point = glm::vec3(0)) {
            this->zangle = angle;
            float rad = glm::radians(angle);
            float deltaX = position.x - point.x;
            float deltaY = position.y - point.y;
            float newX = cos(rad) * deltaX - sin(rad) * deltaY;
            float newY = sin(rad) * deltaX + cos(rad) * deltaY;
            position.x = newX + point.x;
            position.y = newY + point.y;
        }
        SEBCALL inline void set_scale(const glm::vec3 scale) {
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


        double left, right, bottom, top, zNear, zFar;
        float xangle = 0.f, yangle = 0.f, zangle = 0.f;
        glm::vec3 position = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1);
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


    class Quad3D : public Object3D {
    public:
        void create() {
            vbo_data.resize(2);
            vbo_data[0] = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                1.0f,  1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f
            };
            vbo_data[1] = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
            };
            ebo_data = { 0, 1, 2, 2, 3, 0 };
            nondsa_generate_buffers(2);
            nondsa_initialize_vertex_buffer(0, 3, GL_STATIC_DRAW);
            nondsa_initialize_vertex_buffer(1, 2, GL_STATIC_DRAW);
            nondsa_initialize_elements_buffer(GL_STATIC_DRAW);
        }
    };

}
#endif