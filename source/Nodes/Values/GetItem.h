#pragma once
#include "../Node.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <random>

class Node_GetItem : public Node
{


public:
    Node_GetItem(int id) : Node(id) { // Call the base class constructor  
        ID = id;
        Name = "Get Item";
		UniqueName = Name;
        Color = ImColor(150, 200, 255, 150);
        Outputs.emplace_back(rand(), "Item", PinType::Item);
        Outputs.back().Kind = PinKind::Output;
        Outputs.back().Node = this;

		itemNames = csv::Load("Items", 0);

    }

    void Render() {
		util::BlueprintNodeBuilder builder(g_HeaderTexture, 128, 128);
		builder.Begin(ID);
		float pinStartX = ImGui::GetCursorPosX();
		// Header with node-specific color
		builder.Header(Color);
		ImGui::TextUnformatted(Name.c_str());
		builder.EndHeader();
		ImGui::SetCursorPosX(pinStartX);
        std::string preview = itemNames[Data];
		ImGui::Text("%s ", preview.c_str());
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
        if (ImGui::BeginCombo("Item", itemNames[Data].c_str())) {
			for (int i = 0; i < itemNames.size(); ++i) {
				bool isSelected = (Data == i);
				if (ImGui::Selectable(itemNames[i].c_str(), isSelected)) {
					Data = i;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
            ImGui::EndCombo();
        }
	}

	std::string GetSelectedItem() {
		return itemNames[Data];
	}

private:
	std::vector<std::string> itemNames;

};