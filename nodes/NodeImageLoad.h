#pragma once

#include "Node.h"

class NodeImageLoad : public Node
{
    std::string fileLoaded;
public:
    std::string fileName;
    Image3 image;
    NodeImageLoad(int id, int& currentPinId);
    NodeImageLoad(const json& j);

    void render() override;
    void compute(const NodeList& nodes);
    cv::Mat getPinImage3(int pinId) override;
    cv::Mat getPinImage1(int pinId) override;

};

void to_json(json& j, const NodeImageLoad& node);