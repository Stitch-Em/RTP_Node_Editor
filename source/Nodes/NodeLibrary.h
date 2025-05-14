#pragma once
#include "Values/GetItem.h"
#include "Values/ConstInt.h"
#include "Values/GetPinata.h"
#include "Requirements/PinatasAteRequirement.h"
#include "Requirements/PinatasInGardenRequirement.h"
#include "Requirements/ItemsAteRequirement.h"
#include "Requirements/ItemsInGardenRequirement.h"
#include "RootPinata.h"
#include "Actions/RomanceAction.h"
#include "Actions/VisitAction.h"
#include "Actions/VariantAction.h"
#include "Actions/AppearAction.h"
#include "Actions/ResidentAction.h"


inline bool SpawnNodeFromName(const std::string& name, ImVec2 pos, int ID = nodes.size() + 1) {
	bool isValid = false;

	if (name == "Get Item") {
		nodes.push_back(new Node_GetItem(ID));
		isValid = true;
	}
	else if (name == "Get Pinata") {
		nodes.push_back(new Node_GetPinata(ID));
		isValid = true;
	}
	else if (name == "Items Ate") {
		nodes.push_back(new Node_ItemsAteRequirement(ID));
		isValid = true;
	}
	else if (name == "Items In Garden") {
		nodes.push_back(new Node_ItemsInGardenRequirement(ID));
		isValid = true;
	}
	else if (name == "Pinatas Ate") {
		nodes.push_back(new Node_PinatasAteRequirement(ID));
		isValid = true;
	}
	else if (name == "Pinatas In Garden") {
		nodes.push_back(new Node_PinatasInGardenRequirement(ID));
		isValid = true;
	}
	else if (name == "Romance Action") {
		nodes.push_back(new Node_RomanceAction(ID));
		isValid = true;
	}
	else if (name == "Visit Action") {
		nodes.push_back(new Node_VisitAction(ID));
		isValid = true;
	}
	else if (name == "Variant Action") {
		nodes.push_back(new Node_VariantAction(ID));
		isValid = true;
	}
	else if (name == "Appear Action") {
		nodes.push_back(new Node_AppearAction(ID));
		isValid = true;
	}
	else if (name == "Resident Action") {
		nodes.push_back(new Node_ResidentAction(ID));
		isValid = true;
	}
	else if (name == "Pinata") {
		nodes.push_back(new Node_PinataRoot(ID));
		isValid = true;
	}
	else if (name == "Int") {
		nodes.push_back(new Node_Int(ID));
		isValid = true;
	}
	else {
		std::cerr << "Node type not recognized: " << name << std::endl;
	}

	return isValid;
}

