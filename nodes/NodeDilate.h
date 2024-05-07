#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../Image.h"

class NodeDilate : public Node
{
public:
    Image1 image;
    int iterations = 1;
    cv::Point anchor = cv::Point(-1,-1);
    cv::Mat kernel;


    NodeDilate(int id, int& currentPinId) : Node(NodeType::Dilate, id, 1, 4, currentPinId)
    {
        inputPins[0].type = PinType::Image1;
        outputPins[0].type = PinType::Image1;
        outputPins[0].image1Index = 0;
        outputPins[0].data = &image;
    }
    NodeDilate(json j) : Node(j) {
        outputPins[0].data = &image;
        outputPins[0].image1Index = 0;

        j["anchorx"].get_to(anchor.x);
        j["anchory"].get_to(anchor.y);
        j["iterations"].get_to(iterations);
    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeDilate& node);