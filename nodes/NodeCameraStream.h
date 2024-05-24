#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>
#include "../Image.h"

class NodeCameraStream : public Node
{
    cv::VideoCapture* cap;
public:
    Image3 image;
    int cameraId;
    NodeCameraStream(int id, int& currentPinId);
    NodeCameraStream(const json& j);
    ~NodeCameraStream();

    void compute(const NodeList& nodes) override;
    void render() override;
};


void to_json(json& j, const NodeCameraStream& node);