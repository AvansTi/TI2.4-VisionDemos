#pragma once

#include "Node.h"
#include <opencv2/opencv.hpp>

class NodeCameraStream : public Node
{
    cv::VideoCapture* cap;
    Image3 image;
public:
    int cameraId;
    NodeCameraStream(int id, int& currentPinId);
    void compute(const NodeList& nodes) override;
    void render() override;

    cv::Mat getPinImage3(int pinId) override;
    cv::Mat getPinImage1(int pinId) override;

};


void to_json(json& j, const NodeCameraStream& node);