#pragma once
#include "../Node.h"
#include "../Requirements/TerrainInGardenRequirement.h"
#include <random>


class Node_RomanceAction : public Node
{
public:
    Node_RomanceAction(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = "Romance Action";
		UniqueName = Name;
		Color = ImColor(255, 64, 180, 255);
		Inputs.emplace_back(rand(), "Requirements", PinType::Requirement);
		Inputs.back().Num = -1; // Allow multiple connections
		Inputs.back().Node = this;
		Outputs.emplace_back(rand(), "Action", PinType::Action);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;
    }

	void Render() override {
		util::BlueprintNodeBuilder builder(g_HeaderTexture, 128, 128);
		builder.Begin(ID);
		float pinStartX = ImGui::GetCursorPosX();

		//Dont allow Terrain Requirements
		bool isValid = true;
		for (auto& input : Inputs) {
			//find the connected node
			Node* connectedNode = GetConnectedNode(input, links, nodes);
			Node_TerrainInGardenRequirement* terrainNode = dynamic_cast<Node_TerrainInGardenRequirement*>(connectedNode);
			if (terrainNode != nullptr) {
				isValid = false;
				break;
			}
		}

		if (isValid) {
			builder.Header(Color);
			ImGui::TextUnformatted(Name.c_str());
			builder.EndHeader();
		}
		else {
			builder.Header(ImColor(255, 0, 0, 150)); // Red color for invalid
			ImGui::TextUnformatted(Name.c_str());
			ImGui::Text("Terrain Requirement not allowed");
			builder.EndHeader();
			
		}

		
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
		Node_GetPinata* pinataNode = dynamic_cast<Node_GetPinata*>(connectedNode);
		if (pinataNode == nullptr) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::TextWrapped("Terrain Requirement not allowed This is due to the fact that you can have houses in trap areas");
			ImGui::PopStyleColor();
		}
	}
};
