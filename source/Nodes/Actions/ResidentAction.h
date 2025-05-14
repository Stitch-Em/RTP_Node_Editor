#pragma once
#include "../Node.h"
#include <random>


class Node_ResidentAction : public Node
{
public:
    Node_ResidentAction(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = "Resident Action";
		Color = ImColor(255, 64, 180, 255);
		Inputs.emplace_back(rand(), "Requirements", PinType::Requirement);
		Inputs.back().Num = -1; // Allow multiple connections
		Inputs.back().Node = this;
		Outputs.emplace_back(rand(), "Action", PinType::Action);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;
    }
};
