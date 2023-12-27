#pragma once


class ShapeTexture2D {
public:
	ShapeTexture2D(bool use_dsa) {
		this->use_dsa = use_dsa;
	}
	inline void load_texture(const char* path, const int32_t format) const {
		texture_format = format;
		int32_t nrChannels;
		//stbi_set_flip_vertically_on_load(true);
		pixels = stbi_load(path, &texture_width, &texture_height, &nrChannels, 0);
		if (pixels == nullptr) {
			std::cerr << "Failed to load texture: "<< path << "!" << std::endl;
			return;
		}

		if (use_dsa) {
			glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
			glTextureStorage2D(texture_id, 1, GL_RGB8, texture_width, texture_height);

			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(texture_id, 0, 0, 0, texture_width, texture_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			glGenerateTextureMipmap(texture_id);
		}
		else {
			glGenTextures(1, &texture_id);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, format, texture_width, texture_height, 0, format, GL_UNSIGNED_BYTE, pixels);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	inline uint32_t get_texture() const {
		return texture_id;
	}
	inline void sprite(int32_t xoffset, int32_t yoffset, int32_t width, int32_t height) const {
		if (use_dsa) {
			glTextureSubImage2D(texture_id, 0, xoffset, xoffset, width, height, texture_format, GL_UNSIGNED_BYTE, pixels);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, texture_format, GL_UNSIGNED_BYTE, pixels);
			glBindTexture(GL_TEXTURE_2D, NULL);
		}
	}
private:
	mutable bool use_dsa;
	mutable unsigned char* pixels = nullptr;
	mutable uint32_t texture_id = 0U;
	mutable int32_t texture_width = 0, texture_height = 0;
	mutable int32_t texture_format;
};