#pragma once

#include "Node.h"

class NodeConvertColor : public Node
{
public:
    Image3 image;
    int convertMode = 0;

    NodeConvertColor(int id, int& currentPinId) : Node(NodeType::ConvertColor, id, 1, 4, currentPinId)
    {
        inputPins[0].type = PinType::Image3;
        outputPins[0].type = PinType::Image3;
        outputPins[1].type = PinType::Image1;
        outputPins[2].type = PinType::Image1;
        outputPins[3].type = PinType::Image1;
    }
    NodeConvertColor(json j) : Node(j) {}
    cv::Mat getPinImage3(int pinId) override { return image.mat; }
    cv::Mat getPinImage1(int pinId) override;
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeConvertColor& node);