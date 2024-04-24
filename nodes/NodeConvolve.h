#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>

class NodeConvolve : public Node
{
public:
    Image3 image;
    cv::Mat kernel = cv::Mat::ones(3,3,CV_32F);
    int size = 3;

    NodeConvolve(int id, int& currentPinId) : Node(NodeType::Erode, id, 1, 4, currentPinId)
    {
        inputPins[0].type = PinType::Image3;
        setImage3OutputPin(0, image, false);
    }
    NodeConvolve(json j) : Node(j) {
        setImage3OutputPin(0, image, true);
    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeConvolve& node);