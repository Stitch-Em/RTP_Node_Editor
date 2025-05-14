#pragma once
#include "../Graph.h"

inline ImTextureID g_HeaderTexture; // define and assign this once, like `s_HeaderBackground`

class Node {

public:
    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;
	int Data = 0; // Placeholder for node data

    Node(int id) : ID(id) {};

    virtual void Render();

    virtual void RenderDetails();

    //Converts the current node data for saving
	virtual NodeSave GetSaveData() {
		NodeSave data;
		data.ID = ID;
		data.Name = Name;
		data.Data = Data;
		data.Inputs = Inputs;
		data.Outputs = Outputs;
		return data;
    }

	//Restores the node data from the given data
	virtual void RestoreData(const NodeSave data) {
		ID = data.ID;
		Name = data.Name;
		Data = data.Data;
		Inputs = data.Inputs;
		Outputs = data.Outputs;
    }


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


