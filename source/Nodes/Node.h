#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <imgui_node_editor.h>
namespace ed = ax::NodeEditor;

#include "ax/Builders.h"
#include "ax/Drawing.h"
#include "ax/Widgets.h"
namespace util = ax::NodeEditor::Utilities;

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>

#include "../Graph.h"
#include <zpp_bits.h>

inline ImTextureID g_HeaderTexture; // define and assign this once, like `s_HeaderBackground`

enum class PinKind { Output, Input };
enum class PinType { Item, Pinata, Int, Float, Requirement, Action };

class Node; // Forward declaration

struct Pin {
    ed::PinId ID;
    Node* Node;
    std::string Name;
    PinType Type;
    PinKind Kind;
    int Num = 1; // If num -1 then you can attach multible to one

    Pin(int id, const char* name, PinType type) :
        ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input) {
    }
};

inline static ImVec4 GetPinColor(PinType type) {
    switch (type) {
    case PinType::Item:
        return ImVec4(0.4f, 0.6f, 1.0f, 1.0f);   // Blue-ish
    case PinType::Pinata:
        return ImVec4(0.8f, 0.5f, 1.0f, 1.0f);   // Purple
    case PinType::Int:
        return ImVec4(1.0f, 0.7f, 0.2f, 1.0f);   // Orange
    case PinType::Float:
        return ImVec4(1.0f, 0.6f, 0.4f, 1.0f);   // Light orange
    case PinType::Requirement:
        return ImVec4(0.3f, 0.9f, 0.4f, 1.0f);   // Green
    case PinType::Action:
        return ImVec4(1.0f, 0.3f, 0.3f, 1.0f);   // Red-ish
    default:
        return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);   // Gray fallback
    }
}

inline static bool ArePinsCompatible(const Pin& a, const Pin& b)
{
    if (a.Kind == b.Kind)
        return false;

    const Pin& input = (a.Kind == PinKind::Input) ? a : b;
    const Pin& output = (a.Kind == PinKind::Output) ? a : b;

    if (input.Type == output.Type) {
        return true;
    }
    return false;
}

struct NodeSave {

    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;
    std::vector<std::byte> Data; // Additional data for saving

    // zpp_bits serialization support
    auto members() {
        return std::tie(ID, Name, Inputs, Outputs, Color, Data);
    }
};


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


