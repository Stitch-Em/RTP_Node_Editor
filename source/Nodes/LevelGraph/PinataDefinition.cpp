#include "PinataDefinition.h"
#include "../../Saving/SaveSystem.h"

void Node_PinataDefinition::DoubleClick()
{
	if (Data == -1) return;
	currentPinata = Data;
	currentGraph = GraphType::Requirements;
	//Save the current graph
	SaveFile("Graphs/Level" + std::to_string(currentLevel) + ".json");
	//Load graph
	//if file "Graphs/pinataNames[Data].json" exists
	if (std::filesystem::exists("Graphs/" + pinataNames[Data] + ".json"))
		LoadFile("Graphs/" + pinataNames[Data] + ".json");
	else {
		//Copy PinataGraph.json to the pinata name
		std::filesystem::copy("PinataGraph.json", "Graphs/" + pinataNames[Data] + ".json", std::filesystem::copy_options::overwrite_existing);
		std::cout << "Graph for " << pinataNames[Data] << " created!" << std::endl;
		//Load graph
		LoadFile("Graphs/" + pinataNames[Data] + ".json");
	}
}
