#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../Image.h"

class NodeGaussianBlur : public Node
{
public:
    Image3 image;
    int size = 3;
    float sigma = 3;


    NodeGaussianBlur(int id, int& currentPinId) : Node(NodeType::GaussianBlur, id, 1, 4, currentPinId)
    {
        inputPins[0].type = PinType::Image;
        setImage3OutputPin(0, image, false);
    }
    NodeGaussianBlur(json j) : Node(j) {
        outputPins[0].data = &image;
        setImage3OutputPin(0, image, true);
        outputPins[0].image1Index = 0;
        j["size"].get_to(size);
        j["sigma"].get_to(sigma);
    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeGaussianBlur& node);