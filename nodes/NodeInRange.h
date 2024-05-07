#pragma once

#include "Node.h"
#include "../Image.h"

class NodeInRange : public Node
{
public:
    Image1 image1;
    bool input3 = false;
    uint8_t minR = 0;
    uint8_t minG = 0;
    uint8_t minB = 0;
    uint8_t maxR = 255;
    uint8_t maxG = 255;
    uint8_t maxB = 255;

    NodeInRange(int id, int& currentPinId) : Node(NodeType::InRange, id, 3, 1, currentPinId)
    {
        inputPins[0].type = PinType::Image;
        outputPins[0].type = PinType::Image1;
        outputPins[0].data = &image1;
    }
    NodeInRange(json j) : Node(j) {
        outputPins[0].data = &image1;
        j["minr"].get_to(minR);
        j["ming"].get_to(minG);
        j["minb"].get_to(minB);
        j["maxr"].get_to(maxR);
        j["maxg"].get_to(maxG);
        j["maxb"].get_to(maxB);
    }
    void render() override;
    void compute(const NodeList& nodes);
};

void to_json(json& j, const NodeInRange& node);