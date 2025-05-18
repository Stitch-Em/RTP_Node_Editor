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

//Saves the current graph as a json file to a given path
inline void SaveFile(const std::string& Path) {
    //Idk what this does ask Solar
    SaveData saveData;
    saveData.nodes.reserve(nodes.size());
    
    //initialise json object to store data
    nlohmann::json save_json;

    //Loop through nodes and add them to the json object
    for (const auto& node : nodes) {
        //IDK
        NodeSave nodeSave = node->GetSaveData();
        NodePosition nodePos;
        nodePos.ID = node->ID;

        

        //Get node ID as string
        std::string node_id = std::to_string(nodePos.ID.Get());
        nodePos.Position = GetNodePosition(node->ID);

        std::cout << node_id << std::endl;


        //Set values
		save_json["Nodes"][node_id]["Name"] = node->UniqueName;
        save_json["Nodes"][node_id]["Position"]["y"] = nodePos.Position.y;
        save_json["Nodes"][node_id]["Position"]["x"] = nodePos.Position.x;
        save_json["Nodes"][node_id]["Data"] = node->Data;

        //Add the input pins
        for (const auto& input : node->Inputs) {
            std::string pin_id = std::to_string(input.ID.Get());

            std::cout << pin_id << std::endl;

            save_json["Nodes"][node_id]["Inputs"][pin_id]["Name"] = input.Name;
            save_json["Nodes"][node_id]["Inputs"][pin_id]["Type"] = static_cast<int>(input.Type);
            save_json["Nodes"][node_id]["Inputs"][pin_id]["Kind"] = static_cast<int>(input.Kind);
            save_json["Nodes"][node_id]["Inputs"][pin_id]["Num"] = input.Num;
        }

        //Add the output pins
        for (const auto& output : node->Outputs) {
            std::string pin_id = std::to_string(output.ID.Get());
            
            save_json["Nodes"][node_id]["Outputs"][pin_id]["Name"] = output.Name;
            save_json["Nodes"][node_id]["Outputs"][pin_id]["Type"] = static_cast<int>(output.Type);
            save_json["Nodes"][node_id]["Outputs"][pin_id]["Kind"] = static_cast<int>(output.Kind);
            save_json["Nodes"][node_id]["Outputs"][pin_id]["Num"] = output.Num;
        }
    }
    //IDK
    //saveData.links = links;
   
    //Loop through the links and add those
    for (const auto& link : links) {
        std::string link_id = std::to_string(link.ID.Get());

        save_json["Links"][link_id]["From"] = static_cast<int>(link.From.Get());
        save_json["Links"][link_id]["To"] = static_cast<int>(link.To.Get());
    }

    // Write the JSON data to a file
    std::ofstream file(Path);
    if (file.is_open()) {
        file << save_json.dump(4); // Pretty print with 4 spaces
        file.close();
    }
}


//Function to load a graph given the path to a json file of graph data
inline void LoadFile(const std::string& Path) {
    // Clear current graph
    for (auto* node : nodes) {
        delete node;
    }
    nodes.clear();
    links.clear();

    //std::string file_path = "C:/Users/InventorausRex/Desktop/test.json";

    // Read file
    std::ifstream file(Path);
    if (!file.is_open()) return;

    //get json stuff ready
    nlohmann::json load_json;
    file >> load_json;
    file.close();

    //if the json file contains node data
    if (load_json.contains("Nodes")) {
        //loop thorugh each node object
        for (auto& [node_key, node_json] : load_json["Nodes"].items()) {
            //get the node id form the key
            int id = std::stoi(node_key, 0, 10);
            std::string name = node_json["Name"];

            //Spawn node and check to see if it was successful
            if (SpawnNodeFromName(name, ImVec2(0, 0), id) == false) {
                std::cerr << "Failed to spawn node: " << name << std::endl;
                continue; // Skip this node if it fails to spawn
            }

            //Get the nodes position from the json object
            ImVec2 pos(
                node_json["Position"]["x"].get<float>(),
                node_json["Position"]["y"].get<float>()
            );

            //Set the nods position
            ed::SetNodePosition(id, pos);


            Node* node = nodes.back();

            NodeSave nodeData;
            nodeData.ID = id;
            nodeData.Name = name;
            nodeData.Data = load_json["Nodes"][node_key]["Data"];


            nodeData.Inputs.clear();


            //If the node
            if (load_json["Nodes"][node_key].contains("Inputs")) {
                for (auto& [pin_key, pin_json] : load_json["Nodes"][node_key]["Inputs"].items()) {
                    int pin_id = std::stoi(pin_key, 0, 10);
                    Pin input(
                        pin_id,
                        pin_json["Name"].get<std::string>().c_str(),
                        static_cast<PinType>(pin_json["Type"].get<int>())
                    );
                    input.Kind = static_cast<PinKind>(pin_json["Kind"].get<int>());
                    input.Num = pin_json["Num"].get<int>();

                    nodeData.Inputs.push_back(input);
                }
            }

            nodeData.Outputs.clear();

            //If the node
            if (load_json["Nodes"][node_key].contains("Outputs")) {
                for (auto& [pin_key, pin_json] : load_json["Nodes"][node_key]["Outputs"].items()) {
                    int pin_id = std::stoi(pin_key, 0, 10);
                    Pin output(
                        pin_id,
                        pin_json["Name"].get<std::string>().c_str(),
                        static_cast<PinType>(pin_json["Type"].get<int>())
                    );

                    output.Kind = static_cast<PinKind>(pin_json["Kind"].get<int>());
                    output.Num = pin_json["Num"].get<int>();

                    nodeData.Outputs.push_back(output);
                }
            }

            //Give the data to the node
            node->RestoreData(nodeData);
        }
    }

    //If the json contains link data    
    if (load_json.contains("Links")) {
        //loop through the objects in the links object
        for (auto& [link_key, link_json] : load_json["Links"].items()) {
            //Intialise link
            Link link;
            //Conver the link key into the link id as an int
            int link_id = std::stoi(link_key);
            link.ID = link_id;
            std::cout << "Link ID: " << link_id << std::endl;
            //Set the from and to destinations of the link
            link.From = link_json["From"].get<int>();
            link.To = link_json["To"].get<int>();
            std::cout << link_json["From"].get<int>() << std::endl;
            std::cout << link_json["To"].get<int>() << std::endl;
            //push back, go swimming I don't know, but it's important
            links.push_back(link);
        }
    }
}
