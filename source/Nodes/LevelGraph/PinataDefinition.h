#pragma once
#include "../Node.h"
#include <random>
#include <imgui_internal.h>

class Node_PinataDefinition : public Node
{
public:
	

	Node_PinataDefinition(int id) : Node(id) { // Call the base class constructor
		ID = id;
		Data = -1; // Placeholder for pinata ID
		if (Data == -1)
			Name = "Select a Pinata";
		else
		{
			Name = pinataNames[Data];
		}
		UniqueName = "PinataDefinition";
		Color = ImColor(100, 100, 255, 150);
		//Outputs.emplace_back(rand(), "Output", PinType::Action);
		//Outputs.back().Node = this;
		//Outputs.back().Kind = PinKind::Output;
		//Outputs.back().Num = -1; // Allow multiple connections
		Inputs.emplace_back(rand(), "Level", PinType::Action);
		Inputs.back().Node = this;
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Num = -1; // Allow multiple connections

	}

	void Render() override
	{
		//Mostly copied from the blueprint example
		const float rounding = 5.0f;
		const float padding = 12.0f;

		ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(40, 220, 40, 200));         // Strong green background
		ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(20, 140, 20, 220));     // Deep green border
		ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 240, 60, 180));        // Bright green pin
		ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(30, 180, 30, 200));  // Darker green pin border


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
		ImColor pinBarColor = ImColor(20, 140, 20, 220);     // Deep green border
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

		ImGui::EndVertical();

		ed::EndNode();
		ed::PopStyleVar(7);
		ed::PopStyleColor(4);
	}

	void DoubleClick() override;

	void RenderDetails() override
	{
		const char* PinataName;
		if (Data == -1) {
			PinataName = "Select a Pinata";
		}
		else {
			PinataName = pinataNames[Data].c_str();
		}
		if (ImGui::BeginCombo("Pinata", PinataName)) {
			for (int i = 0; i < pinataNames.size(); ++i) {
				bool isSelected = (Data == i);
				if (ImGui::Selectable(pinataNames[i].c_str(), isSelected)) {
					Data = i;
					Name = pinataNames[Data];
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	//Restores the node data from the given data
	void RestoreData(const NodeSave data) override {
		ID = data.ID;
		UniqueName = data.Name;
		Data = data.Data;
		Inputs = data.Inputs;
		Outputs = data.Outputs;
		Name = pinataNames[Data];
	}

	private:
		std::vector<std::string> pinataNames = csv::Load("Pinatas", 0);
};