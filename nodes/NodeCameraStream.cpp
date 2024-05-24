#include "NodeCameraStream.h"
#include <imnodes.h>
#include <misc/cpp/imgui_stdlib.h>

NodeCameraStream::NodeCameraStream(int id, int& currentPinId) : Node(NodeType::CameraStream, id, 0, 4, currentPinId)
{
    cameraId = 0;
    setImage3OutputPin(0, image);
    cap = new cv::VideoCapture(cameraId);
}
NodeCameraStream::NodeCameraStream(const json& j) : Node(j)
{
    cameraId = j["cameraId"];
    image.threeComponent = j["threecomponent"];
    cap = new cv::VideoCapture(cameraId);
    setImage3OutputPin(0, image, true);
}
void to_json(json& j, const NodeCameraStream& node) {
    j["cameraId"] = node.cameraId;
    j["threecomponent"] = node.image.threeComponent;
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

