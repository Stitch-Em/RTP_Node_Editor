#include "RootPinata.h"
#include "../Saving/SaveSystem.h"

void Node_PinataRoot::DoubleClick()
{
	currentPinata = -1;
	currentGraph = GraphType::Level;
	//Save the current graph
	SaveFile("Graphs/" + pinataNames[Data] + ".json");
	//Load the level graph
	LoadFile("Graphs/Level" + std::to_string(currentLevel) + ".json");
}
