#include "NodeCameraStream.h"
#include <imnodes.h>
#include <misc/cpp/imgui_stdlib.h>

NodeCameraStream::NodeCameraStream(int id, int& currentPinId) : Node(NodeType::CameraStream, id, 0, 1, currentPinId)
{
    cameraId = 0;
    outputPins[0].type = PinType::Image3;

    cap = new cv::VideoCapture(cameraId);
}

void NodeCameraStream::compute(const NodeList& nodes)
{
    computed = true;
}


void NodeCameraStream::render()
{
    (*cap) >> image.mat;
    image.refresh();
    computed = false;
    renderBegin("Image Load");

    ImGui::SetNextItemWidth(100);
    if (ImGui::InputInt("Camera", &cameraId))
    {
        cap->release();
        delete cap;
        cap = new cv::VideoCapture(cameraId);
    }

    renderOutputImage("Image", 0, image);

    renderEnd();
}


cv::Mat NodeCameraStream::getPinImage3(int pinId)
{
    return image.mat; //TODO: check pinId, though that's pointless here
}


cv::Mat NodeCameraStream::getPinImage1(int pinId)
{
    cv::Mat planes[3];
    cv::split(image.mat, planes);
    if (pinId == outputPins[1].id) //red
        return planes[0];
    if (pinId == outputPins[2].id) //green
        return planes[1];
    if (pinId == outputPins[3].id) //blue
        return planes[2];
}
