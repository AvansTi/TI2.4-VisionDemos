#include "NodeDilate.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void NodeDilate::render()
{
    renderBegin("Dilate");

    renderInput("Input", 0);

    renderInputInt("Iterations", -1, iterations);
    renderInputInt("Anchor", -1, anchor.x);
    renderInputInt("Anchor", -1, anchor.y);

    renderOutputImage("Image", 0, image);


    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeDilate::compute(const NodeList& nodes)
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

    cv::dilate(img, image.mat, cv::Mat(), anchor, iterations);
    image.refresh();

}


void to_json(json& j, const NodeDilate& node)
{
    j["anchorx"] = node.anchor.x;
    j["anchory"] = node.anchor.y;
    j["iterations"] = node.iterations;

}
