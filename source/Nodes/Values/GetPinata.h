#pragma once
#include "../Node.h"
#include <random>

class Node_GetPinata : public Node
{


public:
    Node_GetPinata(int id) : Node(id) { // Call the base class constructor  
		ID = id;
		Name = "Get Pinata";
		Color = ImColor(150, 200, 255, 150);
		Outputs.emplace_back(rand(), "Pinata", PinType::Pinata);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;
    }
};