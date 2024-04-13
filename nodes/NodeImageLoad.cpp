#include "NodeImageLoad.h"
#include <imnodes.h>
#include <misc/cpp/imgui_stdlib.h>

NodeImageLoad::NodeImageLoad(int id, int& currentPinId) : Node(NodeType::ImageLoad, id, 0, 4, currentPinId)
{
    fileName = "";
    outputPins[0].type = PinType::Image3;
    outputPins[1].type = PinType::Image1;
    outputPins[2].type = PinType::Image1;
    outputPins[3].type = PinType::Image1;
}

NodeImageLoad::NodeImageLoad(const json& j) : Node(j)
{
    fileName = j["filename"];
    image.threeComponent = j["threecomponent"];
}

void to_json(json& j, const NodeImageLoad& node) {
    j["filename"] = node.fileName;
    j["threecomponent"] = node.image.threeComponent;
}


void NodeImageLoad::render()
{
    renderBegin("Image Load");

    ImGui::PushItemWidth(100);
    if (ImGui::InputText("Filename", &fileName))
        computed = false;

    renderOutputImage("Image", 0, image);

    renderEnd();
}

void NodeImageLoad::compute(const NodeList& nodes)
{
    if (fileLoaded != fileName)
    {
        image.mat = cv::imread(fileName, cv::IMREAD_COLOR);
        image.refresh();
        fileLoaded = fileName;
    }
    computed = true;
}

cv::Mat NodeImageLoad::getPinImage3(int pinId)
{
    return image.mat; //TODO: check pinId, though that's pointless here
}

cv::Mat NodeImageLoad::getPinImage1(int pinId)
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
