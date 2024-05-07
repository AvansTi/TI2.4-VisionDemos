#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../NodeList.h"
#include "../Image.h"

class NodeHistogram : public Node
{
public:
    Image3 image;

    NodeHistogram(int id, int& currentPinId) : Node(NodeType::Histogram, id, 1, 0, currentPinId)
    {
        inputPins[0].type = PinType::Image3;
    }
    NodeHistogram(const json& j) : Node(j)
    {

    }

    void compute(const NodeList& nodes);

    


    // Inherited via Node
    void render() override;

};


void to_json(json& j, const NodeHistogram& node);