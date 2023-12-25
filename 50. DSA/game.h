#pragma once




const char* vertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main() {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 450 core
    in vec2 TexCoord;

    uniform sampler2D ourTexture;

    void main() {
        gl_FragColor = texture(ourTexture, TexCoord);
    }
)";

class TexturedTriangle {
public:
    TexturedTriangle() {
        createVertexArray();
        createShaderProgram();
        createTexture("Images/concreate.jpg"); 
    }

    void draw() const {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        //glBindTextureUnit(0, textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    void animate(float x, float y, float z) const {
        

    }

    ~TexturedTriangle() {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shaderProgram);
        glDeleteTextures(1, &textureID);
    }

private:
    GLuint vao, vbocoord, vbotex;
    GLuint shaderProgram;
    GLuint textureID;
    float coord[9] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    float tex[6] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };
    void createVertexArray() {

        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbocoord);
        glCreateBuffers(1, &vbotex);

        glNamedBufferData(vbocoord, sizeof(coord), coord, GL_DYNAMIC_DRAW);
        glNamedBufferData(vbotex, sizeof(tex), tex, GL_DYNAMIC_DRAW);

        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, 0, 0);

        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
        glVertexArrayAttribBinding(vao, 1, 1);

        glVertexArrayVertexBuffer(vao, 0, vbocoord, 0, 3 * sizeof(float));
        glVertexArrayVertexBuffer(vao, 1, vbotex, 0, 2 * sizeof(float));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void createShaderProgram() {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDetachShader(shaderProgram, vertexShader);
        glDeleteShader(vertexShader);
        glDetachShader(shaderProgram, fragmentShader);
        glDeleteShader(fragmentShader);
    }

    void createTexture(const char* imagePath) {

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

        if (data) {
            glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
            glTextureStorage2D(textureID, 1, GL_RGB8, width, height);

            glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateTextureMipmap(textureID);

            stbi_image_free(data);
        }
        else {
            std::cerr << "Failed to load texture" << std::endl;
        }
    }
};