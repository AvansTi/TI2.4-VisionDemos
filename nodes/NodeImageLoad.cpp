#include "NodeImageLoad.h"
#include <imnodes.h>
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/imgcodecs.hpp>

NodeImageLoad::NodeImageLoad(int id, int& currentPinId) : Node(NodeType::ImageLoad, id, 0, 4, currentPinId)
{
    fileName = "";
    setImage3OutputPin(0, image);
}

NodeImageLoad::NodeImageLoad(const json& j) : Node(j)
{
    fileName = j["filename"];
    image.threeComponent = j["threecomponent"];
    setImage3OutputPin(0, image, true);
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