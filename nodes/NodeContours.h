#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../NodeList.h"
#include "../Image.h"

class NodeContours : public Node
{
public:
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    int selectedContour = 0;

    Image3 image;
    cv::Mat img;

    NodeContours(int id, int& currentPinId) : Node(NodeType::Contours, id, 1, 0, currentPinId)
    {
        inputPins[0].type = PinType::Image;
    }
    NodeContours(const json& j) : Node(j)
    {

    }

    void compute(const NodeList& nodes);

    


    // Inherited via Node
    void render() override;

};


void to_json(json& j, const NodeContours& node);