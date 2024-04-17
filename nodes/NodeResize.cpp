#include "NodeResize.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
/*
enum InterpolationFlags {
    INTER_NEAREST = 0,
    INTER_LINEAR = 1,
    INTER_CUBIC = 2,
    INTER_AREA = 3,
    INTER_LANCZOS4 = 4,
    INTER_LINEAR_EXACT = 5,
    INTER_NEAREST_EXACT = 6,
};*/
void NodeResize::render()
{
    renderBegin("Resize");

    renderInput("Input", 0);

    renderInputInt("Width", -1, size.width);
    renderInputInt("Height", -1, size.height);
    renderInputFloat("Width%", -1, percX);
    renderInputFloat("Height%", -1, percY);

    if (ImGui::Combo("Type", &interType, "Nearest\0Linear\0Cubic\0Area\0Lanczos4\0Linear Exact\nNearest Exact"))
        computed = false;

    renderOutputImage("Image", 0, image);


    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeResize::compute(const NodeList& nodes)
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

    if ((size.width == 0 || size.height == 0) && (size.width != 0 || size.height != 0))
    {
        computeError = "Please set width and height";
        return;
    }
    if ((percX == 0 || percY == 0) && (percX != 0 || percY != 0))
    {
        computeError = "Please set both percentages";
        return;
    }
    if (percX == 0 && size.width == 0)
    {
        computeError = "Please set scale";
        return;
    }

    cv::resize(img, image.mat, size, percX, percY, interType);
    image.refresh();

}


void to_json(json& j, const NodeResize& node)
{
    j["sizex"] = node.size.width;
    j["sizey"] = node.size.height;
    j["percx"] = node.percX;
    j["percy"] = node.percY;
    j["intertype"] = node.interType;

}
