#pragma once
#include "Node.h"
#include <random>

class Node_PinataRoot : public Node
{
public:
	Node_PinataRoot(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = "Pinata";
		Color = ImColor(255, 100, 100, 150);
		Inputs.emplace_back(rand(), "Actions", PinType::Action);
		Inputs.back().Node = this;
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Num = -1; // Allow multiple connections
	}
};