#pragma once
#include "../Node.h"
#include "../Values/GetTerrain.h"
#include "../Values/ConstInt.h"
#include <random>


class Node_TerrainInGardenRequirement : public Node
{
public:
	Node_TerrainInGardenRequirement(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = "Terrain In Garden";
		Color = ImColor(77, 229, 102, 150);
		Inputs.emplace_back(rand(), "Terrain", PinType::Ground);
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Node = this;
		Inputs.emplace_back(rand(), "Count", PinType::Int);
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Node = this;
		Outputs.emplace_back(rand(), "Requirement", PinType::Requirement);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;
	}

	void Render() override {
		Node* connectedNode = GetConnectedNode(Inputs[0], links, nodes);
		Node_GetTerrain* pinataNode = dynamic_cast<Node_GetTerrain*>(connectedNode);

		Node* connectedNode2 = GetConnectedNode(Inputs[1], links, nodes);
		Node_Int* intNode = dynamic_cast<Node_Int*>(connectedNode2);

		bool isValid = pinataNode != nullptr && intNode != nullptr;

		util::BlueprintNodeBuilder builder(g_HeaderTexture, 128, 128);
		builder.Begin(ID);
		float pinStartX = ImGui::GetCursorPosX();

		// Header with node-specific color
		if (isValid) {
			builder.Header(Color);
			ImGui::TextUnformatted(Name.c_str());
		}
		else {
			builder.Header(ImColor(255, 0, 0, 150)); // Red color for invalid
			ImGui::TextUnformatted(Name.c_str());
			ImGui::Text("Invalid Node");
		}
		builder.EndHeader();


		ImGui::SetCursorPosX(pinStartX);


		for (auto& input : Inputs) {
			builder.Input(input.ID);
			ImGui::PushStyleColor(ImGuiCol_Text, GetPinColor(input.Type));
			ImGui::TextUnformatted(("| " + input.Name).c_str());
			ImGui::PopStyleColor();
			builder.EndInput();
		}

		for (auto& output : Outputs) {
			builder.Output(output.ID);
			ImGui::PushStyleColor(ImGuiCol_Text, GetPinColor(output.Type));
			ImGui::TextUnformatted((output.Name + " >").c_str());
			ImGui::PopStyleColor();
			builder.EndOutput();
		}

		builder.End();
	}

	void RenderDetails() override
	{
		Node* connectedNode = GetConnectedNode(Inputs[0], links, nodes);
		Node_GetTerrain* pinataNode = dynamic_cast<Node_GetTerrain*>(connectedNode);

		Node* connectedNode2 = GetConnectedNode(Inputs[1], links, nodes);
		Node_Int* intNode = dynamic_cast<Node_Int*>(connectedNode2);

		if (pinataNode == nullptr) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::Text("Get Terrain Node disconnected from Terrain In Garden");
			ImGui::PopStyleColor();
		}
		else {
			pinataNode->RenderDetails();
		}
		if (intNode == nullptr) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::Text("Int node disconnected from Terrain In Garden");
			ImGui::PopStyleColor();
		}
		else {
			intNode->RenderDetails();
		}
	}
};