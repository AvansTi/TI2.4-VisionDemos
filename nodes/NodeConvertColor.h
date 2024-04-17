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
        setImage3OutputPin(0, image);
    }
    NodeConvertColor(json j) : Node(j) {
        image.threeComponent = j["threecomponent"];
        setImage3OutputPin(0, image, true);
    }
    cv::Mat getPinImage3(int pinId) override { return image.mat; }
    cv::Mat getPinImage1(int pinId) override;
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeConvertColor& node);