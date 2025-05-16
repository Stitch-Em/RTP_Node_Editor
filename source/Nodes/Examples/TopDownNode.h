#pragma once
#include "../Node.h"
#include <random>
#include <imgui_internal.h>

class Node_TopDown : public Node
{
public:
	Node_TopDown(int id) : Node(id) { // Call the base class constructor
		ID = id;
		Name = "Top Down";
		Color = ImColor(100, 100, 255, 150);
		Outputs.emplace_back(rand(), "Output", PinType::Action);
		Outputs.back().Node = this;
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Num = -1; // Allow multiple connections
		Inputs.emplace_back(rand(), "Input", PinType::Action);
		Inputs.back().Node = this;
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Num = -1; // Allow multiple connections
	}

	void Render() override
	{
		//Mostly copied from the blueprint example
		const float rounding = 5.0f;
		const float padding = 12.0f;

		ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 255, 200));
		ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
		ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 180, 255, 150));
		ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

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
		ImColor pinBarColor = ImColor(60, 180, 255, 150);
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
};