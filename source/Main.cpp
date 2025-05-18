#include "Graph.h"
#include "Saving/SaveSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImTextureID LoadTexture(const char* path)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
	if (!data) {
		std::cerr << "Failed to load image: " << path << std::endl;
		return nullptr;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return (ImTextureID)(intptr_t)textureId;
}

int main()
{
	std::vector<std::string> pinataNames;

	std::ifstream file("Data\\Sheets\\Pinata\\Pinata.csv");
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string pinata;
		if (std::getline(ss, pinata, ',')) {
			pinataNames.push_back(pinata);
		}
	}

	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Tree Logic Editor", nullptr, nullptr);
	if (!window) return -1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (gl3wInit()) return -1;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImFont* customFont = io.Fonts->AddFontFromFileTTF("Ancientsans-rvyrK.ttf", 64.0f);
	if (!customFont)
		std::cerr << "Failed to load custom font!\n";
	io.FontDefault = customFont;
	io.FontGlobalScale = 0.25f;

	io.FontDefault = customFont;

	ImGui::StyleColorsDark();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	g_HeaderTexture = LoadTexture("Data/BlueprintBackground.png");
	editor = ed::CreateEditor();
	bool hasloaded = false;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		DrawGraph();

		if (!hasloaded) {
			std::string path = std::string("Graphs/") + std::string("Level0.json");
			LoadFile(path);
			hasloaded = true;
		}

		ImGui::Begin("Details");

		if (selectedNodeId) {
			auto it = std::find_if(nodes.begin(), nodes.end(), [&](Node* n) {
				return n->ID == selectedNodeId;
				});

			if (it != nodes.end()) {
				Node* node = *it;
				ImGui::Text("Node: %s", node->Name.c_str());

				node->RenderDetails();
			}
		}
		else {
			ImGui::Text("No node selected");
		}

		ImGui::End();

		ImGui::Begin("Debug Window");
		if (ImGui::Button("Save")) {
			SaveFile("Test.json");
		}
		if (ImGui::Button("Load")) {
			if (DoesSaveExist("Test.json")) {
				LoadFile("Test.json");
			}
			else {
				std::cerr << "Save file does not exist!" << std::endl;
			}
		}
		//Displays the selected node ID/ Nodes / Links
		ImGui::Text("Selected Node ID: %d", selectedNodeId.Get());
		ImGui::Text("Nodes: %d", nodes.size());
		ImGui::Text("Links: %d", links.size());

		//show canvas location of mouse
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 canvasPos = ImGui::GetCursorScreenPos();
		ImVec2 canvasSize = ImGui::GetContentRegionAvail();
		ImVec2 relativeMousePos = ImVec2(mousePos.x - canvasPos.x, mousePos.y - canvasPos.y);
		ImGui::Text("Mouse Position: (%.1f, %.1f)", mousePos.x, mousePos.y);
		ImGui::Text("Canvas Position: (%.1f, %.1f)", canvasPos.x, canvasPos.y);
		ImGui::Text("Canvas Size: (%.1f, %.1f)", canvasSize.x, canvasSize.y);
		ImGui::Text("Relative Mouse Position: (%.1f, %.1f)", relativeMousePos.x, relativeMousePos.y);

		for (auto& node : nodes) {
			ImGui::Text("Node ID: %d", node->ID.Get());
			ImGui::Text("Node Name: %s", node->Name.c_str());
		}
		for (auto& link : links) {
			ImGui::Text("Link ID: %d", link.ID.Get());
			ImGui::Text("From: %d", link.From.Get());
			ImGui::Text("To: %d", link.To.Get());
		}

		ImGui::End();

		ImGui::Render();
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		glViewport(0, 0, w, h);
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ed::DestroyEditor(editor);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}