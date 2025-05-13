#pragma once
#include "../Node.h"
#include <random>


class Node_PinataRequirement : public Node
{
public:
	Node_PinataRequirement(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = "Pinata Requirement";
		Color = ImColor(255, 200, 100, 150);
		Inputs.emplace_back(rand(), "Pinata", PinType::Pinata);
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Node = this;
		Inputs.emplace_back(rand(), "Count", PinType::Int);
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Node = this;
		Outputs.emplace_back(rand(), "Requirement", PinType::Requirement);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;
	}
};