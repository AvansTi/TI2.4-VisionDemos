#include "NodeCanny.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void NodeCanny::render()
{
    renderBegin("Canny");

    renderInput("Input", 0);

    renderInputFloat("Threshold1", -1, threshold1);
    renderInputFloat("Threshold2", -1, threshold2);

    renderOutputImage("Image", 0, image);

    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeCanny::compute(const NodeList& nodes)
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
    auto pin = node->getOutputPin(inputPins[0].connection);
    auto img = pin->type == PinType::Image1 ? node->getPinImage1(inputPins[0].connection) : node->getPinImage3(inputPins[0].connection);
    if (!img.data)
    {
        computeError = "Image not loaded";
        return;
    }
    if (img.cols != image.mat.cols || img.cols != image.mat.rows)
    {
        image.mat = cv::Mat(img.cols, img.rows, img.type());
    }

    cv::Canny(img, image.mat, threshold1, threshold2);

    image.refresh();

    outputPins[0].type = pin->type;
}


void to_json(json& j, const NodeCanny& node)
{
    j["threshold1"] = node.threshold1;
    j["threshold2"] = node.threshold2;
}
