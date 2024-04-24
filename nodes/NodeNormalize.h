#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>

class NodeNormalize : public Node
{
public:
    Image1 image;
    NodeNormalize(int id, int& currentPinId) : Node(NodeType::Normalize, id, 1, 1, currentPinId)
    {
        inputPins[0].type = PinType::Image1;
        outputPins[0].type = PinType::Image1;
        outputPins[0].data = &image;
    }
    NodeNormalize(json j) : Node(j) {
        outputPins[0].data = &image;

    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeNormalize& node);