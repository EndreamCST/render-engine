//
// Created by Krisu on 2020-02-06.
//

#ifndef RENDER_ENGINE_GUI_HPP
#define RENDER_ENGINE_GUI_HPP


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

/*
 * Handling GUI things.
 * GUI is using imgui, while window is using glfw in implementation
 */

#include "Renderer.hpp"

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

    void DrawUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Drawing UI... */
		// A simple window
		{
			static float f = 0.0f;
			static int cnt = 0;

			ImGui::Begin("Settings");
			
			ImGui::Text("Properties: ");
			ImGui::Checkbox("Use Texture", &ui.useTexture);
			ImGui::Checkbox("Use Model", &ui.useModel);

			ImGui::SliderFloat("Roughness", &ui.roughness, 0.001f, 1.0f);
			ImGui::SliderFloat("Metallic", &ui.metallic, 0.001f, 1.0f);
			ImGui::ColorEdit3("Albedo", (float *)&ui.albedo.x);

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
	}ui;

private:
    const Renderer &renderer;
};

#endif //RENDER_ENGINE_GUI_HPP
