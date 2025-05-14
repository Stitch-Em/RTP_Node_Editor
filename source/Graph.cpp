#include "Graph.h"
#include "Nodes/Node.h"

Pin* FindPinById(ed::PinId id)
{
    for (auto& node : nodes) {
        for (auto& pin : node->Inputs)
            if (pin.ID == id) return &pin;
        for (auto& pin : node->Outputs)
            if (pin.ID == id) return &pin;
    }
    return nullptr;
}

void DrawGraph()
{
    {
        ImGui::Begin("Logic Tree");
        ed::SetCurrentEditor(editor);
        ed::Begin("Logic Tree");

        ed::NodeId current;
        int count = ed::GetSelectedNodes(&current, 1);
        if (count == 1) {
            selectedNodeId = current;
        }
        else {
            selectedNodeId = ed::NodeId(0); // Clear if nothing is selected
        }

        for (auto& node : nodes)
            node->Render();

        for (auto& link : links) {
            Pin* from = FindPinById(link.From);
            Pin* to = FindPinById(link.To);

            // Pick the output pin (for color reference)
            Pin* colorPin = from && from->Kind == PinKind::Output ? from : to;
            if (!colorPin) { continue; }
            ImVec4 color = GetPinColor(colorPin->Type); // from NodeHelpers.h
            ed::Link(link.ID, link.From, link.To, color, 2.5f); // thickness optional
        }

        if (ed::BeginCreate()) {
            ed::PinId start, end;
            if (ed::QueryNewLink(&start, &end)) {
                Pin* a = FindPinById(start);
                Pin* b = FindPinById(end);

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
                        links.push_back({ ed::LinkId(rand()), output->ID, input->ID });
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
}
