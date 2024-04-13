#pragma once

#include "Node.h"


class NodeOutput : public Node
{
public:
    NodeOutput(int id, int& currentPinId) : Node(NodeType::Output, id, 1, 0, currentPinId)
    {
        inputPins[0].type = PinType::Image3;
    }
    void compute(const NodeList& nodes)
    {
        computed = true;
    }
};


void to_json(json& j, const NodeOutput& node);