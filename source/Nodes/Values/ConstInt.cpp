#include "ConstInt.h"

void Node_Int::Render()
{
    util::BlueprintNodeBuilder builder(g_HeaderTexture, 128, 128);
    builder.Begin(ID);
    float pinStartX = ImGui::GetCursorPosX();

    // Header with node-specific color
    builder.Header(Color);
    ImGui::TextUnformatted(Name.c_str());
    builder.EndHeader();

    ImGui::SetCursorPosX(pinStartX);

    std::string preview = std::to_string(Data);
    ImGui::Text("%s ", preview);

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

void Node_Int::RenderDetails()
{
    ImGui::InputInt("Int", &Data);
}
