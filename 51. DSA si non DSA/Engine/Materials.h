#pragma once



namespace mat {
	struct {
		struct {
			const glm::vec3 ambient = glm::vec3(0.0215, 0.1745, 0.0215);
			const glm::vec3 diffuse = glm::vec3(0.07568, 0.61424, 0.07568);
			const glm::vec3 specular = glm::vec3(0.633, 0.727811, 0.633);
			const float shininess = 0.6 * 128;
		}Emerald;
		struct {
			const glm::vec3 ambient = glm::vec3(0.135, 0.2225, 0.1575);
			const glm::vec3 diffuse = glm::vec3(0.54, 0.89, 0.63);
			const glm::vec3 specular = glm::vec3(0.316228, 0.316228, 0.316228);
			const float shininess = 0.1 * 128;
		}Jade;
		struct {
			const glm::vec3 ambient = glm::vec3(0.05375, 0.05, 0.06625);
			const glm::vec3 diffuse = glm::vec3(0.18275, 0.17, 0.22525);
			const glm::vec3 specular = glm::vec3(0.332741, 0.328634, 0.346435);
			const float shininess = 0.3 * 128;
		}Obsidian;
		struct {
			const glm::vec3 ambient = glm::vec3(0.25, 0.20725, 0.20725);
			const glm::vec3 diffuse = glm::vec3(1, 0.829, 0.829);
			const glm::vec3 specular = glm::vec3(0.296648, 0.296648, 0.296648);
			const float shininess = 0.88 * 128;
		}Pearl;
		struct {
			const glm::vec3 ambient = glm::vec3(0.1745, 0.01175, 0.01175);
			const glm::vec3 diffuse = glm::vec3(0.61424, 0.04136, 0.04136);
			const glm::vec3 specular = glm::vec3(0.727811, 0.626959, 0.626959);
			const float shininess = 0.6 * 128;
		}Ruby;
		struct {
			const glm::vec3 ambient = glm::vec3(0.1, 0.18725, 0.1745);
			const glm::vec3 diffuse = glm::vec3(0.396, 0.74151, 0.69102);
			const glm::vec3 specular = glm::vec3(0.297254, 0.30829, 0.306678);
			const float shininess = 0.1 * 128;
		}Turquoise;
		struct {
			const glm::vec3 ambient = glm::vec3(0.329412, 0.223529, 0.027451);
			const glm::vec3 diffuse = glm::vec3(0.780392, 0.568627, 0.113725);
			const glm::vec3 specular = glm::vec3(0.992157, 0.941176, 0.807843);
			const float shininess = 0.21794872 * 128;
		}Brass;
		struct {
			const glm::vec3 ambient = glm::vec3(0.2125, 0.1275, 0.054);
			const glm::vec3 diffuse = glm::vec3(0.714, 0.4284, 0.18144);
			const glm::vec3 specular = glm::vec3(0.393548, 0.271906, 0.166721);
			const float shininess = 0.2 * 128;
		}Bronze;
		struct {
			const glm::vec3 ambient = glm::vec3(0.25, 0.25, 0.25);
			const glm::vec3 diffuse = glm::vec3(0.4, 0.4, 0.4);
			const glm::vec3 specular = glm::vec3(0.774597, 0.774597, 0.774597);
			const float shininess = 0.6 * 128;
		}Chrome;
		struct {
			const glm::vec3 ambient = glm::vec3(0.19125, 0.0735, 0.0225);
			const glm::vec3 diffuse = glm::vec3(0.7038, 0.27048, 0.0828);
			const glm::vec3 specular = glm::vec3(0.256777, 0.137622, 0.086014);
			const float shininess = 0.1 * 128;
		}Copper;
		struct {
			const glm::vec3 ambient = glm::vec3(0.24725, 0.1995, 0.0745);
			const glm::vec3 diffuse = glm::vec3(0.75164, 0.60648, 0.22648);
			const glm::vec3 specular = glm::vec3(0.628281, 0.555802, 0.366065);
			const float shininess = 0.4 * 128;
		}Gold;
		struct {
			const glm::vec3 ambient = glm::vec3(0.19225, 0.19225, 0.19225);
			const glm::vec3 diffuse = glm::vec3(0.50754, 0.50754, 0.50754);
			const glm::vec3 specular = glm::vec3(0.508273, 0.508273, 0.508273);
			const float shininess = 0.4 * 128;
		}Silver;
		struct {
			const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
			const glm::vec3 diffuse = glm::vec3(0.01, 0.01, 0.01);
			const glm::vec3 specular = glm::vec3(0.50, 0.50, 0.50);
			const float shininess = 0.25 * 128;
		}Black_plastic;
		struct {
			const glm::vec3 ambient = glm::vec3(0.0, 0.1, 0.06);
			const glm::vec3 diffuse = glm::vec3(0.0, 0.50980392, 0.50980392);
			const glm::vec3 specular = glm::vec3(0.50196078, 0.50196078, 0.50196078);
			const float shininess = 0.25 * 128;
		}Cyan_plastic;
		struct {
			const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
			const glm::vec3 diffuse = glm::vec3(0.1, 0.35, 0.1);
			const glm::vec3 specular = glm::vec3(0.45, 0.55, 0.45);
			const float shininess = 0.25 * 128;
		}Green_plastic;
		struct {
			const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
			const glm::vec3 diffuse = glm::vec3(0.5, 0.0, 0.0);
			const glm::vec3 specular = glm::vec3(0.7, 0.6, 0.6);
			const float shininess = 0.25 * 128;
		}Red_plastic;
		struct  {
			const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
			const glm::vec3 diffuse = glm::vec3(0.55, 0.55, 0.55);
			const glm::vec3 specular = glm::vec3(0.70, 0.70, 0.70);
			const float shininess = 0.25 * 128;
		}White_plastic;
		struct {
			const glm::vec3 ambient = glm::vec3(0.0, 0.0, 0.0);
			const glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.0);
			const glm::vec3 specular = glm::vec3(0.60, 0.60, 0.50);
			const float shininess = 0.25 * 128;
		}Yellow_plastic;
		struct {
			const glm::vec3 ambient = glm::vec3(0.02, 0.02, 0.02);
			const glm::vec3 diffuse = glm::vec3(0.01, 0.01, 0.01);
			const glm::vec3 specular = glm::vec3(0.4, 0.4, 0.4);
			const float shininess = 0.078125 * 128;
		}Black_rubber;
		struct {
			const glm::vec3 ambient = glm::vec3(0.0, 0.05, 0.05);
			const glm::vec3 diffuse = glm::vec3(0.4, 0.5, 0.5);
			const glm::vec3 specular = glm::vec3(0.04, 0.7, 0.7);
			const float shininess = 0.078125 * 128;
		}Cyan_rubber;
		struct {
			const glm::vec3 ambient = glm::vec3(0.0, 0.05, 0.0);
			const glm::vec3 diffuse = glm::vec3(0.4, 0.5, 0.4);
			const glm::vec3 specular = glm::vec3(0.04, 0.7, 0.04);
			const float shininess = 0.078125 * 128;
		}Green_rubber;
		struct {
			const glm::vec3 ambient = glm::vec3(0.05, 0.0, 0.0);
			const glm::vec3 diffuse = glm::vec3(0.5, 0.4, 0.4);
			const glm::vec3 specular = glm::vec3(0.7, 0.04, 0.04);
			const float shininess = 0.078125 * 128;
		}Red_rubber;
		struct  {
			const glm::vec3 ambient = glm::vec3(0.05, 0.05, 0.05);
			const glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.5);
			const glm::vec3 specular = glm::vec3(0.7, 0.7, 0.7);
			const float shininess = 0.078125 * 128;
		}White_rubber;
		struct {
			const glm::vec3 ambient = glm::vec3(0.05, 0.05, 0.05);
			const glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.4);
			const glm::vec3 specular = glm::vec3(0.7, 0.7, 0.04);
			const float shininess = 0.078125 * 128;
		}Yellow_rubber;
	} Material;
}