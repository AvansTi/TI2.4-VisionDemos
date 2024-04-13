#pragma once
#include <vector>
#include "nodes/Node.h"
#include <json.hpp>
using json = nlohmann::json;

class NodeList : public std::vector<Node*>
{
public:
	Node* findNodeWithPin(int pinId) const
	{
		for (auto node : *this)
		{
			if (node->getInputPin(pinId))
				return node;
			if (node->getOutputPin(pinId))
				return node;
		}
		return nullptr;
	}
};


void to_json(json& j, const NodeList& p);
void from_json(const json& j, NodeList& p);