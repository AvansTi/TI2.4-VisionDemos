#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../Image.h"

class NodeResize : public Node
{
public:
    Image3 image;
    cv::Size size;
    float percX = 0;
    float percY = 0;
    int interType = 1;
    NodeResize(int id, int& currentPinId) : Node(NodeType::Resize, id, 1, 4, currentPinId)
    {
        inputPins[0].type = PinType::Image3;
        setImage3OutputPin(0, image, false);
    }
    NodeResize(json j) : Node(j) {
        setImage3OutputPin(0, image, true);
        j["sizex"].get_to(size.width);
        j["sizey"].get_to(size.height);
        j["percx"].get_to(percX);
        j["percy"].get_to(percY);
        j["intertype"].get_to(interType);

    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeResize& node);