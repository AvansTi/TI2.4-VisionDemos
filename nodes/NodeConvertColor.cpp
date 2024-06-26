#include "NodeConvertColor.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

struct ConvertMode
{
    int mode;
    const char* label;
};

std::vector<ConvertMode> modes =
{
    { cv::COLOR_BGR2HSV, "Rgb to Hsv"},
    { cv::COLOR_HSV2BGR, "Hsv to Rgb"},
    { cv::COLOR_BGR2GRAY, "Rgb to grayscale"},
    { cv::COLOR_GRAY2BGR, "Grayscale to rgb"},
};


void NodeConvertColor::render()
{
    renderBegin("Convert Color");

    renderInput("Input", 0);
    renderOutputImage("Image", 0, image);
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo("Type", modes[convertMode].label))
    {
        for (auto i = 0; i < modes.size(); i++)
        {
            if (ImGui::Selectable(modes[i].label, convertMode == i))
            {
                computed = false;
                convertMode = i;
            }
        }
        ImGui::EndCombo();
    }

    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeConvertColor::compute(const NodeList& nodes)
{
    auto p1 = nodes.findNodeWithPin(inputPins[0].connection);
    if (!p1)
    {
        computed = true;
        return;
    }
    if (!p1->computed)
        return;

    computed = true;

    auto img1 = p1->getPinImage3(inputPins[0].connection);
    if (!img1.data)
    {
        computeError = "Image not loaded";
        return;
    }
    if (img1.cols != image.mat.cols || img1.cols != image.mat.rows)
    {
        //image.mat = cv::Mat(img1.cols, img1.rows, img1.type());
    }

    cv::cvtColor(img1, image.mat, modes[convertMode].mode);
    image.refresh();

    if (image.mat.channels() == 1)
        outputPins[0].type = PinType::Image1;
    else
        outputPins[0].type = PinType::Image3;

}

void to_json(json& j, const NodeConvertColor& node)
{
    j["threecomponent"] = node.image.threeComponent;
    j["convertmode"] = node.convertMode;
}
