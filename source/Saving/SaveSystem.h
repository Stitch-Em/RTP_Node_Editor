#pragma once
#include <fstream>
#include <vector>
#include <zpp_bits.h>
#include "../Nodes/Node.h"
#include "../Graph.h"
#include <imgui_node_editor.h>
namespace ed = ax::NodeEditor;

struct SaveData
{
    std::vector<NodeSave> nodes;
    std::vector<Link> links;
    //std::unordered_map<ed::NodeId, ImVec2> positions;

    auto members()
    {
        //return std::tie(nodes, links, positions);
    }
};

inline void SaveFile(const std::string& Path, ed::EditorContext* editor) {
    // Gather node save data
    std::vector<NodeSave> saves;
    //std::unordered_map<ed::NodeId, ImVec2> positions;
    for (const auto& node : nodes) {
        saves.push_back(node->GetSaveData());
        // Query node positions from the editor context
        if (editor) {
       //     positions[node->ID] = ed::GetNodePosition(node->ID);
        }
    }

    // Gather links
    std::vector<Link> linkSaves = links;

    // Bundle all data
    //SaveData data{ std::move(saves), std::move(linkSaves), std::move(positions) };

    // Serialize to memory buffer using zpp_bits and export to file or
	//Export to json in human readable format, OOORR
	//Export a json per pinata for better collaboration (Less merge conflicts if using source control) OOOOR
    //Dont export to file and update a online database ;)
    
}