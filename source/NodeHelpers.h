#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Nodes/Node.h"

#include <imgui_node_editor.h> 
namespace ed = ax::NodeEditor;

ed::EditorContext* editor = nullptr;

namespace std {
    template <>
    struct hash<ed::NodeId> {
        std::size_t operator()(const ed::NodeId& id) const noexcept {
            return std::hash<void*>()(id.AsPointer());
        }
    };
}

std::vector<std::string> itemNames;
std::unordered_map<ed::NodeId, int> itemSelections; // NodeId → selected index
std::unordered_map<ed::NodeId, int> IntLookup; // NodeId → selected index

