#include "NodeList.h"

#include "nodes/Nodes.h"


void to_json(json& j, const NodeList& p) {
	j = json::array();
	for (auto node : p)
	{
		json n;
		to_json(n, *node);
		j.push_back(n);
	}
}

void from_json(const json& j, NodeList& nodeList) {
	nodeList.clear(); //TODO: delete p
	for (int i = 0; i < j.size(); i++)
	{
		const auto& n = j[i];
		if (n["type"] == (int)NodeType::ImageLoad)
			nodeList.push_back(new NodeImageLoad(n));
		else if (n["type"] == (int)NodeType::Add)
			nodeList.push_back(new NodeAdd(n));
		else if (n["type"] == (int)NodeType::ConvertColor)
			nodeList.push_back(new NodeConvertColor(n));
	}
}
