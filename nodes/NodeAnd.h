#pragma once

#include "Node.h"

class NodeAnd : public Node
{
public:
    Image3 image;

    uint8_t value1;
    uint8_t value2;

    NodeAnd(int id, int& currentPinId) : Node(NodeType::And, id, 2, 4, currentPinId)
    {
        inputPins[0].type = PinType::Any;
        inputPins[1].type = PinType::Any;
        setImage3OutputPin(0, image);
    }
    NodeAnd(json j) : Node(j) {
        setImage3OutputPin(0, image, true);
    }
    void render() override;
    void compute(const NodeList& nodes);
    cv::Mat getPinImage1(int pinId) override;
};

void to_json(json& j, const NodeAnd& node);