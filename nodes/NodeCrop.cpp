#include "NodeCrop.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void NodeCrop::render()
{
    renderBegin("Resize");

    renderInput("Input", 0);

    renderInputInt("x", -1, rect.x);
    renderInputInt("y", -1, rect.y);
    renderInputInt("Width", -1, rect.width);
    renderInputInt("Height", -1, rect.height);

    renderOutputImage("Image", 0, image);


    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeCrop::compute(const NodeList& nodes)
{
    auto node = nodes.findNodeWithPin(inputPins[0].connection);
    if (!node)
    {
        computed = true;
        return;
    }
    if (!node->computed)
        return;

    computed = true;

    auto img = node->getPinImage3(inputPins[0].connection);
    if (!img.data)
    {
        computeError = "Image not loaded";
        return;
    }
    if (img.cols != image.mat.cols || img.cols != image.mat.rows)
    {
        image.mat = cv::Mat(img.cols, img.rows, img.type());
    }

    image.mat = img(rect);//no imgCrop
    
    image.refresh();

}


void to_json(json& j, const NodeCrop& node)
{
    j["x"] = node.rect.x;
    j["y"] = node.rect.y;
    j["width"] = node.rect.width;
    j["height"] = node.rect.height;

}
