#pragma once

#include "Node.h"

class NodeAdd : public Node
{
public:
    Image3 image;

    uint8_t value1;
    uint8_t value2;

    NodeAdd(int id, int& currentPinId) : Node(NodeType::Add, id, 2, 4, currentPinId)
    {
        inputPins[0].type = PinType::Any;
        inputPins[1].type = PinType::Any;
        outputPins[0].type = PinType::Image3;
        outputPins[1].type = PinType::Image1;
        outputPins[2].type = PinType::Image1;
        outputPins[3].type = PinType::Image1;
    }
    NodeAdd(json j) : Node(j) {}
    void render() override;
    void compute(const NodeList& nodes);
    cv::Mat getPinImage3(int pinId) override;
    cv::Mat getPinImage1(int pinId) override;
};

void to_json(json& j, const NodeAdd& node);