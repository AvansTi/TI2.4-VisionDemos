#include "NodeGaussianBlur.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void NodeGaussianBlur::render()
{
    renderBegin("GaussianBlur");

    renderInput("Input", 0);

    renderInputInt("Size", -1, size);
    renderInputFloat("Sigma", -1, sigma);

    renderOutputImage("Image", 0, image);

    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeGaussianBlur::compute(const NodeList& nodes)
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

    if (size % 2 == 0)
    {
        computeError = "size is even! not possible!";
        return;
    }

    if (img.cols != image.mat.cols || img.cols != image.mat.rows)
    {
        image.mat = cv::Mat(img.cols, img.rows, img.type());
    }

    cv::GaussianBlur(img, image.mat, cv::Size(size,size), sigma, sigma);

    image.refresh();

    outputPins[0].type = pin->type;
}


void to_json(json& j, const NodeGaussianBlur& node)
{
    j["size"] = node.size;
    j["sigma"] = node.sigma;
}
