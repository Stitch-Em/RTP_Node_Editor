#pragma once
#include "Node.h"
#include <random>

class Node_PinataRoot : public Node
{
public:
	Node_PinataRoot(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = currentPinata == -1 ? "Invalid Pinata" : pinataNames[currentPinata];
		std::string UniqueName = "PinataRoot";
		Color = ImColor(255, 100, 100, 150);
		Inputs.emplace_back(rand(), "Actions", PinType::Action);
		Inputs.back().Node = this;
		Inputs.back().Kind = PinKind::Input;
		Inputs.back().Num = -1; // Allow multiple connections
	}

	void DoubleClick() override;

private:
	std::vector<std::string> pinataNames = csv::Load("Pinatas", 0);
};