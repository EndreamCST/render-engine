//
// Created by Krisu on 2020-02-06.
//

#ifndef RENDER_ENGINE_GUI_HPP
#define RENDER_ENGINE_GUI_HPP


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "ImGuiFileDialog.h"
/*
 * Handling GUI things.
 * GUI is using imgui, while window is using glfw in implementation
 */

#include "Renderer.hpp"
#include "stb_image.h"
#include "Texture.hpp"

class GUI {
public:
    explicit GUI(const Renderer &renderer) : renderer(renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(renderer.window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    ~GUI() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
	{
		// Load from file
		int image_width = 0;
		int image_height = 0;
		printf("\nfilename:");
		printf(filename);
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		
		if (image_data == NULL)
			return false;

		// Create a OpenGL texture identifier
		GLuint image_texture;
		glGenTextures(1, &image_texture);
		glBindTexture(GL_TEXTURE_2D, image_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload pixels into texture
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);

		*out_texture = image_texture;
		*out_width = image_width;
		*out_height = image_height;
		
		return true;
	}

    void DrawUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Drawing UI... */
		// A simple window
		{
			static float f = 0.0f;
			static int cnt = 0;
			static std::string filePathCopy = "";

			ImGui::Begin("Settings");
			
			ImGui::Text("Properties: ");
			ImGui::Checkbox("Use Texture", &ui.useTexture);
			ImGui::Checkbox("Use Model", &ui.useModel);

			ImGui::SliderFloat("Roughness", &ui.roughness, 0.001f, 1.0f);
			ImGui::SliderFloat("Metallic", &ui.metallic, 0.001f, 1.0f);
			ImGui::ColorEdit3("Albedo", (float *)&ui.albedo.x);

			GLuint curr_texture = 0;
			if (ImGui::Button("Load Texture"))
			{
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", " Choose File", ".*\0.cpp\0.h\0.hpp\0\0", ".");
			}
		
			if (ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
			{
				if (ImGuiFileDialog::Instance()->IsOk == true)
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
					std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
					std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
					
					// Show texture
					int my_image_width = 0;
					int my_image_height = 0;
					filePathCopy = filePathName;
					ui.texture_path = const_cast<char*>(filePathCopy.c_str());
					
					bool ret = LoadTextureFromFile(ui.texture_path, &curr_texture, &my_image_width, &my_image_height);
					IM_ASSERT(ret);
				}
				ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
			}
			
			// Set Texture
			if (ui.my_image_texture != curr_texture && curr_texture != 0)
			{
				ui.my_image_texture = curr_texture;
				Texture texture(ui.texture_path);
			}
			if (ui.my_image_texture != 0)
			{
				ImGui::Image((void*)(intptr_t)ui.my_image_texture, ImVec2(100, 100));
			}
			
			if (ImGui::Button("cnt plus plus")) {
				cnt++;
			}
			ImGui::Checkbox("Show another window", &ui.show_another_window);
			ImGui::SameLine();
			ImGui::Text("cnt = %d", cnt);

			ImGui::Text("Frame rate: %.1f FPS", ImGui::GetIO().Framerate);

			ImGui::End();
		}

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

	struct {
		glm::vec3 albedo = { 1, 1, 1 };
		float     roughness = 0.1;
		float     metallic = 0.9;
		bool      useTexture = false;
		bool      useModel = false;
		bool	  show_another_window = false;
		bool      my_tool_active = true;
		GLuint    my_image_texture = 0;
		char*     texture_path;
	}ui;

private:
    const Renderer &renderer;
};

#endif //RENDER_ENGINE_GUI_HPP
