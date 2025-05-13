#include "Node.h"

void Node::Render()
{
    util::BlueprintNodeBuilder builder(g_HeaderTexture, 128, 128);
    builder.Begin(ID);
    float pinStartX = ImGui::GetCursorPosX();

    builder.Header(Color);
    ImGui::TextUnformatted(Name.c_str());
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

void Node::RenderDetails()
{
}
