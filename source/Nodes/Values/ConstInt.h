#pragma once
#include "../Node.h"
#include <random>

class Node_Int : public Node
{


public:
    Node_Int(int id) : Node(id) { // Call the base class constructor  
        ID = id;
        Name = "Int";
        Color = ImColor(200, 120, 0, 150);
		Outputs.emplace_back(rand(), "Int", PinType::Int);
		Outputs.back().Kind = PinKind::Output;
		Outputs.back().Node = this;
    }

    void Render() override;

    void RenderDetails() override;
};