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
#include <algorithm>
#include <random>
#include <iostream>

class Node; // Forward declaration

enum class PinKind { Output, Input };
enum class PinType { Item, Pinata, Accessory, Ground, Int, Float, Requirement, Action };
enum class GraphType {Level, Requirements};

//What pinata is currently being modified in the requirements graph
inline int currentPinata = -1;

inline GraphType currentGraph = GraphType::Level;
inline int currentLevel = 0;

struct Pin {
    ed::PinId ID;
    Node* Node;
    std::string Name;
    PinType Type;
    PinKind Kind;
    int Num = 1; // If num -1 then you can attach multiple to one

    Pin(int id, const char* name, PinType type) :
        ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input) {
    }
};

struct NodeSave {

    ed::NodeId ID;
    std::string Name;
    int Data;
	std::vector<Pin> Inputs;
	std::vector<Pin> Outputs;
};

struct Link {
    ed::LinkId ID;
    ed::PinId From;
    ed::PinId To;
};

inline std::vector<Node*> nodes;
inline std::vector<Link> links;
inline ed::NodeId selectedNodeId;

inline ed::EditorContext* editor = nullptr;

Pin* FindPinById(ed::PinId id);

void DrawGraph();

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
	case PinType::Accessory:
		return ImVec4(0.9f, 0.3f, 0.9f, 1.0f);   // Pink
	case PinType::Ground:
		return ImVec4(0.5f, 0.25f, 0.1f, 1.0f);   // Brown
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