#include "NodeNormalize.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void NodeNormalize::render()
{
    renderBegin("Normalize");

    renderInput("Input", 0);
    renderOutputImage("Image", 0, image);

    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeNormalize::compute(const NodeList& nodes)
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

    auto img = node->getPinImage1(inputPins[0].connection);
    if (!img.data)
    {
        computeError = "Image not loaded";
        return;
    }
    if (img.cols != image.mat.cols || img.cols != image.mat.rows)
    {
        image.mat = cv::Mat(img.cols, img.rows, img.type());
    }

    cv::normalize(img, image.mat, 0, 255, cv::NormTypes::NORM_MINMAX, CV_8U);
    image.refresh();

}


void to_json(json& j, const NodeNormalize& node)
{
}
