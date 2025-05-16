#pragma once
#include "../Node.h"
#include <random>

class Node_GetTerrain : public Node
{


public:
    Node_GetTerrain(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = "Get Terrain";
		Color = ImColor(150, 200, 255, 150);
		Outputs.emplace_back(rand(), "Terrain", PinType::Ground);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;

		Data = 0;
		Data2 = 0;

		std::ifstream file("Data\\Sheets\\Areas\\Areas.csv");
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string pinata;
			if (std::getline(ss, pinata, ',')) {
				AreaNames.push_back(pinata);
			}
		}

		// Load the terrain names from the Mainland CSV file
		std::ifstream file1(std::string("Data\\Sheets\\Terrain\\Mainland.csv"));
		std::string line1;
		while (std::getline(file1, line1)) {
			std::stringstream ss(line1);
			std::string pinata;
			if (std::getline(ss, pinata, ',')) {
				TerrainNames.push_back(pinata);
			}
		}
    }

	void Render() {
		util::BlueprintNodeBuilder builder(g_HeaderTexture, 128, 128);
		builder.Begin(ID);
		float pinStartX = ImGui::GetCursorPosX();
		builder.Header(Color);
		std::string preview1 = AreaNames[Data];
		std::string title = "Get Terrain in " + preview1;
		ImGui::Text(title.c_str());
		builder.EndHeader();
		ImGui::SetCursorPosX(pinStartX);
			std::string preview2 = TerrainNames[Data2];
			ImGui::Text("%s ", preview2.c_str());
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

	void RenderDetails() override {

		if (ImGui::BeginCombo("Area", AreaNames[Data].c_str())) {
			for (int i = 0; i < AreaNames.size(); ++i) {
				bool isSelected = (Data == i);
				if (ImGui::Selectable(AreaNames[i].c_str(), isSelected)) {
					Data = i;

					// Clear the previous terrain names
					TerrainNames.clear();
					
					// Load the terrain names from the corresponding CSV file
					std::ifstream file(std::string("Data\\Sheets\\Terrain\\" + AreaNames[i] + ".csv"));
					std::string line;
					while (std::getline(file, line)) {
						std::stringstream ss(line);
						std::string pinata;
						if (std::getline(ss, pinata, ',')) {
							TerrainNames.push_back(pinata);
						}
					}

					Data2 = 0; // Reset Data to the first terrain
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (Data2 != -1) {
			if (ImGui::BeginCombo("Terrain", TerrainNames[Data2].c_str())) {
				for (int i = 0; i < TerrainNames.size(); ++i) {
					bool isSelected = (Data2 == i);
					if (ImGui::Selectable(TerrainNames[i].c_str(), isSelected)) {
						Data2 = i;
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
	}

	std::string GetSelectedTerrain() {
		return TerrainNames[Data2];
	}

private:
    std::vector<std::string> AreaNames;
	std::vector<std::string> TerrainNames;
};