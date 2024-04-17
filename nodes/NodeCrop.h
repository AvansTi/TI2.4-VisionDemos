#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>

class NodeCrop : public Node
{
public:
    Image3 image;
    cv::Rect rect;

    NodeCrop(int id, int& currentPinId) : Node(NodeType::Erode, id, 1, 4, currentPinId)
    {
        inputPins[0].type = PinType::Image3;
        setImage3OutputPin(0, image, false);
    }
    NodeCrop(json j) : Node(j) {
        setImage3OutputPin(0, image, true);
        j["x"].get_to(rect.x);
        j["y"].get_to(rect.y);
        j["width"].get_to(rect.width);
        j["height"].get_to(rect.height);

    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeCrop& node);