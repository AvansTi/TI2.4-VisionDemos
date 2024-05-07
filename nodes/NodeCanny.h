#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../Image.h"

class NodeCanny : public Node
{
public:
    Image3 image;
    float threshold1 = 25;
    float threshold2 = 75;


    NodeCanny(int id, int& currentPinId) : Node(NodeType::Canny, id, 1, 4, currentPinId)
    {
        inputPins[0].type = PinType::Image;
        setImage3OutputPin(0, image, false);
    }
    NodeCanny(json j) : Node(j) {
        outputPins[0].data = &image;
        setImage3OutputPin(0, image, true);
        outputPins[0].image1Index = 0;
        j["threshold1"].get_to(threshold1);
        j["threshold2"].get_to(threshold2);
    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeCanny& node);