#pragma once


class Casa {
public:
    float scale = 1;
    float xangle = 0, yangle = 0, zangle = 0;
    float xpos = 0, ypos = 1, zpos = 3;
    void create() {
        model.load("game/models/house/casa.obj");
        shader.open("game/models/house/shader.vert", "game/models/house/shader.frag");
    }
    void draw(float width, float height) {
        srand(time(0));
        shader.bind();
        shader.set_vec3("lightColor", glm::vec3(1, 1, 1));
        shader.set_vec3("lightPos", glm::vec3(xpos, ypos, zpos));
        shader.set_vec3("viewPos", glm::vec3(xpos, ypos, zpos));
        for (auto& mesh : model.meshes) {
            shader.set_mat4("model", mesh.get_transform_matrix());
            shader.set_mat4("projection", mesh.get_perspective_matrix(90, width / height, 0.1f, 1000.f));
            shader.set_mat4("view", mesh.get_view_matrix(glm::vec3(xpos, ypos, zpos), glm::vec3(0, 0, 0)));
            shader.set_vec3("materialColor", mesh.materialColor);
            shader.set_bool("useTexture", !mesh.textures.empty());

            mesh.set_scale(glm::vec3(scale, scale, scale));
            mesh.set_xangle(xangle);
            mesh.set_yangle(yangle);
            mesh.set_zangle(zangle);

            for (unsigned int i = 0; i < mesh.textures.size(); i++) {
                shader.set_int("texture" + std::to_string(i), i);
                mesh.textures[i].bind(i);
            }
            mesh.draw();
            for (unsigned int i = 0; i < mesh.textures.size(); i++) {
                mesh.textures[i].unbind();
            }
        }
        shader.unbind();
    }
private:
    seb::Model model;
    seb::Shader shader;
};
