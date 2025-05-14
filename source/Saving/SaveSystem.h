#pragma once
#include <fstream>
#include <vector>
#include "../Nodes/Node.h"
#include "../Graph.h" // contains ed::EditorContext* editor;
#include <imgui_node_editor.h>
namespace ed = ax::NodeEditor;
#include "../Nodes/NodeLibrary.h"

#include <json.hpp> // nlohmann/json.hpp

struct NodePosition {
	ed::NodeId ID;
	ImVec2 Position;
};;

struct SaveData
{
    std::vector<NodeSave> nodes;
    std::vector<Link> links;
	std::vector<NodePosition> positions;
};

inline bool DoesSaveExist(const std::string& Path) {
	std::ifstream file(Path);
	return file.good();
}

inline void SaveFile(const std::string& Path) {
    //Save using json to serialize the data
    SaveData saveData;
    saveData.nodes.reserve(nodes.size());
    for (const auto& node : nodes) {
        NodeSave nodeSave = node->GetSaveData();
        saveData.nodes.push_back(nodeSave);

        NodePosition nodePos;
        nodePos.ID = node->ID;
        nodePos.Position = GetNodePosition(node->ID);
        saveData.positions.push_back(nodePos);
    }
    saveData.links = links;

    // Serialize the data to JSON
    nlohmann::json jsonData;

    // Convert saveData.nodes to a JSON-compatible format
    jsonData["nodes"] = nlohmann::json::array();
    for (const auto& node : saveData.nodes) {
        nlohmann::json nodeJson;
        nodeJson["ID"] = static_cast<int>(node.ID.Get());
        nodeJson["Name"] = node.Name;
        nodeJson["Data"] = node.Data;

        // Serialize Inputs
        nodeJson["Inputs"] = nlohmann::json::array();
        for (const auto& input : node.Inputs) {
            nlohmann::json pinJson;
            pinJson["ID"] = static_cast<int>(input.ID.Get());
            pinJson["Name"] = input.Name;
            pinJson["Type"] = static_cast<int>(input.Type);
            pinJson["Kind"] = static_cast<int>(input.Kind);
            pinJson["Num"] = input.Num;
            nodeJson["Inputs"].push_back(pinJson);
        }

        // Serialize Outputs
        nodeJson["Outputs"] = nlohmann::json::array();
        for (const auto& output : node.Outputs) {
            nlohmann::json pinJson;
            pinJson["ID"] = static_cast<int>(output.ID.Get());
            pinJson["Name"] = output.Name;
            pinJson["Type"] = static_cast<int>(output.Type);
            pinJson["Kind"] = static_cast<int>(output.Kind);
            pinJson["Num"] = output.Num;
            nodeJson["Outputs"].push_back(pinJson);
        }

        jsonData["nodes"].push_back(nodeJson);
    }

    jsonData["links"] = nlohmann::json::array();
    for (const auto& link : saveData.links) {
        nlohmann::json linkJson;
        linkJson["ID"] = static_cast<int>(link.ID.Get());
        linkJson["From"] = static_cast<int>(link.From.Get());
        linkJson["To"] = static_cast<int>(link.To.Get());
        jsonData["links"].push_back(linkJson);
    }

    jsonData["positions"] = nlohmann::json::array();
    for (const auto& position : saveData.positions) {
        nlohmann::json positionJson;
        positionJson["ID"] = static_cast<int>(position.ID.Get());
        positionJson["Position"] = { position.Position.x, position.Position.y };
        jsonData["positions"].push_back(positionJson);
    }

    // Write the JSON data to a file
    std::ofstream file(Path);
    if (file.is_open()) {
        file << jsonData.dump(4); // Pretty print with 4 spaces
        file.close();
    }
}

inline void LoadFile(const std::string& Path) {
    // Clear current graph
    for (auto* node : nodes) {
        delete node;
    }
    nodes.clear();
    links.clear();

    // Read file
    std::ifstream file(Path);
    if (!file.is_open()) return;

    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    // Load nodes
    if (jsonData.contains("nodes") && jsonData["nodes"].is_array()) {
        for (const auto& nodeJson : jsonData["nodes"]) {
            int id = nodeJson["ID"];
            std::string name = nodeJson["Name"];

            if (SpawnNodeFromName(name, ImVec2(0, 0), id) == false) {
                std::cerr << "Failed to spawn node: " << name << std::endl;
                continue; // Skip this node if it fails to spawn
            }
            Node* node = nodes.back(); // Get the last node added

            NodeSave nodeData;
            nodeData.ID = id;
            nodeData.Name = name;
            nodeData.Data = nodeJson["Data"];

            // Deserialize Inputs
            nodeData.Inputs.clear();
            if (nodeJson.contains("Inputs") && nodeJson["Inputs"].is_array()) {
                for (const auto& inputJson : nodeJson["Inputs"]) {
                    Pin input(
                        inputJson["ID"],
                        inputJson["Name"].get<std::string>().c_str(),
                        static_cast<PinType>(inputJson["Type"].get<int>())
                    );
                    input.Kind = static_cast<PinKind>(inputJson["Kind"].get<int>());
                    input.Num = inputJson["Num"].get<int>();
                    nodeData.Inputs.push_back(input);
                }
            }

            // Deserialize Outputs
            nodeData.Outputs.clear();
            if (nodeJson.contains("Outputs") && nodeJson["Outputs"].is_array()) {
                for (const auto& outputJson : nodeJson["Outputs"]) {
                    Pin output(
                        outputJson["ID"],
                        outputJson["Name"].get<std::string>().c_str(),
                        static_cast<PinType>(outputJson["Type"].get<int>())
                    );
                    output.Kind = static_cast<PinKind>(outputJson["Kind"].get<int>());
                    output.Num = outputJson["Num"].get<int>();
                    nodeData.Outputs.push_back(output);
                }
            }

            node->RestoreData(nodeData);

        }
    }

    // Load links
    if (jsonData.contains("links") && jsonData["links"].is_array()) {
        for (const auto& linkJson : jsonData["links"]) {
            Link link;
            link.ID = linkJson["ID"].get<int>();
            link.From = linkJson["From"].get<int>();
            link.To = linkJson["To"].get<int>();
            links.push_back(link);
        }
    }

    // Load positions
    if (jsonData.contains("positions") && jsonData["positions"].is_array()) {
        for (const auto& posJson : jsonData["positions"]) {
            ed::NodeId id = posJson["ID"].get<int>();
            ImVec2 pos(
                posJson["Position"][0].get<float>(),
                posJson["Position"][1].get<float>()
            );
            ed::SetNodePosition(id, pos);
        }
    }
}
