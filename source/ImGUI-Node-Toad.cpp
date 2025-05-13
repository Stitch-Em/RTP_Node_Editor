#include "NodeHelpers.h"
#include "Graph.h"
#include "Nodes/Values/GetItem.h"
#include "Nodes/Values/ConstInt.h"
#include "Nodes/Values/GetPinata.h"
#include "Nodes/Requirements/PinataRequirement.h"
#include "Nodes/Requirements/ItemRequirement.h"
#include "Nodes/RootPinata.h"
#include "Nodes/Actions/RomanceAction.h"
#include "Saving/SaveSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// --- Globals ---
int nextId = 1;
int GetNextId() { return nextId++; }

ImVec2 nodeSpawnPos;
ImVec2 deferredNodePos;
ed::NodeId deferredNodeId;

Pin* FindPin(ed::PinId id)
{
    for (auto& node : nodes) {
        for (auto& pin : node->Inputs)
            if (pin.ID == id) return &pin;
        for (auto& pin : node->Outputs)
            if (pin.ID == id) return &pin;
    }
    return nullptr;
}

ImTextureID Application_LoadTexture(const char* path)
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

void DrawGraph()
{
    ImGui::Begin("Logic Tree");
    ed::SetCurrentEditor(editor);
    ed::Begin("Logic Tree");

    ed::NodeId current;
    int count = ed::GetSelectedNodes(&current, 1);
    if (count == 1){
        selectedNodeId = current;
    }else {
        selectedNodeId = ed::NodeId(0); // Clear if nothing is selected
    }

    for (auto& node : nodes)
		node->Render();

    for (auto& link : links) {
        Pin* from = FindPin(link.From);
        Pin* to = FindPin(link.To);

        // Pick the output pin (for color reference)
        Pin* colorPin = from && from->Kind == PinKind::Output ? from : to;

        ImVec4 color = GetPinColor(colorPin->Type); // from NodeHelpers.h
        ed::Link(link.ID, link.From, link.To, color, 2.5f); // thickness optional
    }

    if (ed::BeginCreate()) {
        ed::PinId start, end;
        if (ed::QueryNewLink(&start, &end)) {
            Pin* a = FindPin(start);
            Pin* b = FindPin(end);

            if (!a || !b || a == b || !ArePinsCompatible(*a, *b)) {
                ed::RejectNewItem();
            }
            else {
                Pin* input = (a->Kind == PinKind::Input) ? a : b;
                Pin* output = (a->Kind == PinKind::Output) ? a : b;

                bool inputAlreadyLinked = std::any_of(links.begin(), links.end(), [&](auto& l) {
                    return l.To == input->ID;
                    });

                if (input->Num != -1 && inputAlreadyLinked) {
                    ed::RejectNewItem();
                }
                else if (ed::AcceptNewItem()) {
                    links.push_back({ ed::LinkId(GetNextId()), output->ID, input->ID });
                    std::cout << "Created link: " << output->ID.Get() << " -> " << input->ID.Get() << "\n";
                }
            }
        }
        ed::EndCreate();
    }

    if (ed::BeginDelete()) {
        ed::LinkId deleted;
        while (ed::QueryDeletedLink(&deleted)) {
            if (ed::AcceptDeletedItem()) {
                std::cout << "Deleting link: " << deleted.Get() << "\n";
                links.erase(std::remove_if(links.begin(), links.end(),
                    [&](auto& l) { return l.ID == deleted; }),
                    links.end());
            }
        }
        ed::EndDelete();
    }

    ed::End();
    ImGui::End(); // End Logic Tree

}

int main()
{
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
    ImFontConfig fontConfig;
    fontConfig.OversampleH = 2;
    fontConfig.OversampleV = 2;
    fontConfig.PixelSnapH = true;

    static const ImWchar glyphRanges[] = {
        0x0020, 0x00FF, 0x25A0, 0x25FF, 0x2600, 0x26FF, 0x29BE, 0x29BF, 0
    };

    ImFont* customFont = io.Fonts->AddFontFromFileTTF("Ancientsans-rvyrK.ttf", 18.0f, &fontConfig, glyphRanges);
    if (!customFont)
        std::cerr << "Failed to load custom font!\n";

    io.FontDefault = customFont;
    ImGui::StyleColorsDark();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    g_HeaderTexture = Application_LoadTexture("Data/BlueprintBackground.png");
    editor = ed::CreateEditor();

    // --- Demo Nodes ---
    {
        nodes.push_back(new Node_GetItem(rand()));
        nodes.push_back(new Node_Int(rand()));
        nodes.push_back(new Node_GetPinata(rand()));
		nodes.push_back(new Node_PinataRequirement(rand()));
		nodes.push_back(new Node_ItemRequirement(rand()));
		nodes.push_back(new Node_RomanceAction(rand()));
		nodes.push_back(new Node_PinataRoot(rand()));

       
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        DrawGraph();

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