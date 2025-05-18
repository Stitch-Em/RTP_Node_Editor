#pragma once
#include "../Node.h"
#include <random>
#include <imgui_internal.h>

class Node_ItemDefinition : public Node
{
public:
	Node_ItemDefinition(int id) : Node(id) { // Call the base class constructor
		ID = id;
		Name = "Item List";
		UniqueName = Name;
		Color = ImColor(100, 100, 255, 150);
		Inputs.emplace_back(rand(), "Input", PinType::Action);
		Inputs.back().Node = this;
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Num = -1; // Allow multiple connections
	}

	std::vector<std::string> ItemList = {
		"Tomato Seed",
		"Carrot Seed",
		"Turnip Seed",
		"Tier 1 Shovel",
		"Tier 1 Watering Can"
	};

	void Render() override
	{
		//Mostly copied from the blueprint example
		const float rounding = 5.0f;
		const float padding = 12.0f;

		ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(40, 80, 220, 200));         // Strong blue background
		ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(20, 40, 140, 220));     // Deep blue border
		ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 120, 240, 180));       // Bright blue pin
		ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(30, 60, 180, 200));  // Darker blue pin border

		ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
		ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
		ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
		ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
		ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
		ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
		ed::PushStyleVar(ed::StyleVar_PinRadius, 5.0f);

		ed::BeginNode(ID);

		ImRect inputsRect(ImVec2(0, 0), ImVec2(0, 0)), outputsRect(ImVec2(0, 0), ImVec2(0, 0));
		bool hasInput = false, hasOutput = false;

		ImGui::BeginVertical(ID.AsPointer());

		ImGui::BeginHorizontal("inputs");
		ImGui::Spring(0, padding * 2);

		for (auto& pin : Inputs)
		{
			if (pin.Kind == PinKind::Input)
			{
				ImGui::Dummy(ImVec2(0, padding));
				ImGui::Spring(1, 0);
				inputsRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

				ed::PushStyleVar(ed::StyleVar_PinCorners, 12); // Top corners
				ed::BeginPin(pin.ID, ed::PinKind::Input);
				ed::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
				ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
				ed::EndPin();
				ed::PopStyleVar();
				hasInput = true;
				break;
			}
		}
		if (!hasInput)
			ImGui::Dummy(ImVec2(0, padding));

		ImGui::Spring(0, padding * 2);
		ImGui::EndHorizontal();

		ImGui::BeginHorizontal("content_frame");
		ImGui::Spring(1, padding);

		ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
		ImGui::Dummy(ImVec2(160, 0));
		ImGui::Spring(1);
		ImGui::TextUnformatted(Name.c_str());
		ImGui::Spring(1);
		ImGui::EndVertical();

		ImGui::Spring(1, padding);
		ImGui::EndHorizontal();

		ImGui::BeginHorizontal("outputs");
		ImGui::Spring(0, padding * 2);

		for (auto& pin : Outputs)
		{
			if (pin.Kind == PinKind::Output)
			{
				ImGui::Dummy(ImVec2(0, padding));
				ImGui::Spring(1, 0);
				outputsRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

				ed::PushStyleVar(ed::StyleVar_PinCorners, 3);
				ed::BeginPin(pin.ID, ed::PinKind::Output);
				ed::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
				ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
				ed::EndPin();
				ed::PopStyleVar();
				hasOutput = true;
				break;
			}
		}
		if (!hasOutput)
			ImGui::Dummy(ImVec2(0, padding));

		ImGui::Spring(0, padding * 2);
		ImGui::EndHorizontal();

		auto* drawList = ed::GetNodeBackgroundDrawList(ID);
		ImColor pinBarColor = ImColor(20, 40, 140, 220);     // Deep blue border
		float pinBarRounding = 4.0f;

		if (drawList)
		{
			if (hasInput)
			{
				int pinBarCorners = 12;
				drawList->AddRectFilled(
					inputsRect.GetTL(),
					inputsRect.GetBR(),
					pinBarColor,
					pinBarRounding,
					pinBarCorners
				);
				drawList->AddRect(
					inputsRect.GetTL(),
					inputsRect.GetBR(),
					pinBarColor,
					pinBarRounding,
					pinBarCorners
				);
			}
			if (hasOutput)
			{
				int pinBarCorners = 3;
				drawList->AddRectFilled(
					outputsRect.GetTL(),
					outputsRect.GetBR(),
					pinBarColor,
					pinBarRounding,
					pinBarCorners
				);
				drawList->AddRect(
					outputsRect.GetTL(),
					outputsRect.GetBR(),
					pinBarColor,
					pinBarRounding,
					pinBarCorners
				);
			}
		}

		for (const auto& item : ItemList)
		{
			ImGui::Spring(1);
			//Add dot
			ImGui::Bullet();
			ImGui::SameLine();
			ImGui::TextUnformatted(item.c_str());
			ImGui::Spring(1);
		}

		ImGui::EndVertical();

		ed::EndNode();
		ed::PopStyleVar(7);
		ed::PopStyleColor(4);
	}


	void RenderDetails() override
	{
		// Make it so i can add items to the list or remove them
		ImGui::Text("Item List");
		ImGui::Separator();
		for (size_t i = 0; i < ItemList.size(); ++i)
		{
			if (ImGui::BeginCombo(std::string("##" + std::to_string(i)).c_str(), ItemList[i].c_str())) {
				for (int j = 0; j < itemNames.size(); ++j) {
					bool isSelected = (ItemList[i] == itemNames[j]);
					if (ImGui::Selectable(itemNames[j].c_str(), isSelected)) {
						ItemList[i] = itemNames[j];
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::Button(("X##" + std::to_string(i)).c_str()))
			{
				ItemList.erase(ItemList.begin() + i);
				break;
			}
		}
		if (ImGui::Button("Add Item"))
		{
			ItemList.push_back("New Item");
		}

	}

	private:
		std::vector<std::string> itemNames = csv::Load("Items", 0);
};