#include "Graph.h"
#include "Nodes/Node.h"
#include "Nodes/NodeLibrary.h"

ImVec2 mousePos;
ImVec2 loc;

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

static bool operator==(const ax::NodeEditor::NodeId& lhs, const ax::NodeEditor::NodeId& rhs) {
    return lhs.AsPointer() == rhs.AsPointer();
}

static bool operator==(const ax::NodeEditor::PinId& lhs, const ax::NodeEditor::PinId& rhs) {
    return lhs.AsPointer() == rhs.AsPointer();
}

static bool operator==(const ax::NodeEditor::LinkId& lhs, const ax::NodeEditor::LinkId& rhs) {
    return lhs.AsPointer() == rhs.AsPointer();
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

        for (auto& node : nodes) {
            node->Render();
            // Check for double-click on this node
			if (ed::GetDoubleClickedNode() == node->ID) {
                node->DoubleClick();
            }
        }

        for (auto& link : links) {
            Pin* from = FindPinById(link.From);
            Pin* to = FindPinById(link.To);

            // Pick the output pin (for color reference)
            Pin* colorPin = from && from->Kind == PinKind::Output ? from : to;
            if (!colorPin) { continue; }
            ImVec4 color = GetPinColor(colorPin->Type); // from NodeHelpers.h
            ed::Link(link.ID, link.From, link.To, color, 5.0f); // thickness optional
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

            ed::NodeId deletedNodeId;
            while (ed::QueryDeletedNode(&deletedNodeId)) {
                if (ed::AcceptDeletedItem()) {
					std::cout << "Deleting node: " << deletedNodeId.Get() << "\n";

					// Remove all links connected to this node
					links.erase(std::remove_if(links.begin(), links.end(),
						[&](Link& link) { return link.From.Get() == deletedNodeId || link.To.Get() == deletedNodeId; }),
						links.end());

					// Remove the node itself
					auto it = std::remove_if(nodes.begin(), nodes.end(),
						[&](Node* node) { return node->ID == deletedNodeId; });
					if (it != nodes.end() && *it) {
						// Delete the node

                        nodes.erase(it, nodes.end());
                    }

                }
            }

            ed::EndDelete();
        }
       

        ed::Suspend();
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1)) {
            ImGui::OpenPopup("SpawnNodeContextMenu");
            mousePos = ImGui::GetMousePos();
			loc = ax::NodeEditor::ScreenToCanvas(mousePos);
        }
        ImVec2 loc = ax::NodeEditor::ScreenToCanvas(mousePos);
		ImGui::SetNextWindowPos(mousePos);
        if (ImGui::BeginPopup("SpawnNodeContextMenu")) {
            SpawnMenu(loc);
            ImGui::EndPopup();
        }
        ed::Resume();

        ed::End();
        ImGui::End(); // End Logic Tree

    }
}
