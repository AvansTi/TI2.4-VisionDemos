#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../Image.h"

class NodeDistanceTransform : public Node
{
public:
    Image1 image;
    int maskSize = 1;
    NodeDistanceTransform(int id, int& currentPinId) : Node(NodeType::DistanceTransform, id, 1, 1, currentPinId)
    {
        inputPins[0].type = PinType::Image1;
        outputPins[0].type = PinType::Image1;
        outputPins[0].data = &image;
    }
    NodeDistanceTransform(json j) : Node(j) {
        outputPins[0].data = &image;
        j["masksize"].get_to(maskSize);

    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeDistanceTransform& node);