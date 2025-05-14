#pragma once
#include "../Graph.h"
#include <zpp_bits.h>

inline ImTextureID g_HeaderTexture; // define and assign this once, like `s_HeaderBackground`

class Node {

public:
    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;

    Node(int id) : ID(id) {};

    virtual void Render();

    virtual void RenderDetails();

    //Converts the current node data for saving
	virtual NodeSave GetSaveData() { return NodeSave(); }

	//Restores the node data from the given data
	virtual void RestoreData(const NodeSave data) {}


};

// Helper to find the node connected to a given input pin
inline Node* GetConnectedNode(Pin& inputPin, const std::vector<Link>& links, const std::vector<Node*>& nodes) {
    for (const auto& link : links) {
        if (link.To == inputPin.ID) {
            // Find the pin with ID == link.From
            for (auto* node : nodes) {
                for (auto& outputPin : node->Outputs) {
                    if (outputPin.ID == link.From) {
                        return node;
                    }
                }
            }
        }
    }
    return nullptr;
}


