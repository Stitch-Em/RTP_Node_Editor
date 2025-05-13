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

class Node; // Forward declaration

struct Link {
    ed::LinkId ID;
    ed::PinId From;
    ed::PinId To;
};

inline std::vector<Node*> nodes;
inline std::vector<Link> links;
inline ed::NodeId selectedNodeId;