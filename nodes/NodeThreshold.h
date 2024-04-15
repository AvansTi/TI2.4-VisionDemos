#pragma once

#include "Node.h"

class NodeThreshold : public Node
{
public:
    Image1 image;
    uint8_t threshold = 127;
    uint8_t maxValue = 255;
    int thresholdType = 0;

    NodeThreshold(int id, int& currentPinId) : Node(NodeType::Threshold, id, 3, 1, currentPinId)
    {
        inputPins[0].type = PinType::Image1;
        inputPins[1].type = PinType::Int;
        inputPins[2].type = PinType::Int;
        outputPins[0].type = PinType::Image1;
    }
    NodeThreshold(json j) : Node(j) {}
    void render() override;
    void compute(const NodeList& nodes);

    cv::Mat getPinImage3(int pinId) override;
    cv::Mat getPinImage1(int pinId) override;
};

void to_json(json& j, const NodeThreshold& node);