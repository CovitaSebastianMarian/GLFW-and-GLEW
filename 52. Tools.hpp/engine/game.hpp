#pragma once



class Square2D : public seb::Object2D {
public:
    inline void create() {
        vbo_data.resize(2);

        vbo_data[0] = {
            -1, -1,
            -1, 1,
            1, 1,
            1, -1
        };
        vbo_data[1] = {
            0, 0,
            0, 1,
            1, 1,
            1, 0
        };
        ebo_data = {
            0, 1, 2,
            0, 2, 3
        };

        generate_buffers(2);
        dsa_initialize_vertex_buffer(0, 2, GL_DYNAMIC_DRAW);
        dsa_initialize_vertex_buffer(1, 2, GL_DYNAMIC_DRAW);
        dsa_initialize_elements_buffer(GL_DYNAMIC_DRAW);
    }
};



Square2D obj;
seb::Shader sh;
seb::Texture tex0, tex1;
float sqsize = 300;
void init(int width, int height) {
    sh.open("include/resources/2D/shader.vert", "include/resources/2D/shader.frag");
    tex0.open("images/img2.jpg", GL_RGB);
    tex1.open("images/img2nm.png", GL_RGBA);

    obj.create();
    obj.set_orthographic(0, width, height, 0);
    obj.set_position(glm::vec2(width / 2, height / 2));
    obj.set_scale(glm::vec2(sqsize, sqsize));
}

void resize(int width, int height) {
    obj.set_orthographic(0, width, height, 0);
}

float shininess = 0;
float z = 0;

void draw(int width, int height, double xpos, double ypos) {
    sh.bind();
    sh.set_mat4("matrix" ,obj.get_matrix());
    sh.set_int("texture0", 0);
    sh.set_int("texture1", 1);
    sh.set_vec3("lightDir", 0, 0, z);
    sh.set_vec3("viewDir", 0, 0, z);
    sh.set_vec3("lightColor", 1, 1, 1);
    sh.set_vec3("ambientColor", 1, 1, 1);
    sh.set_float("shininess", shininess);
    tex0.bind(0);
    tex1.bind(1);
    obj.draw();
    tex0.unbind();
    tex1.unbind();
    sh.unbind();
}