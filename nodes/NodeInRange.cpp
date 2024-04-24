#include "NodeInRange.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>


void NodeInRange::render()
{
    renderBegin("InRange");

    renderInput("Input", 0);

    renderOutputImage("Image", 0, image1);

    if (input3)
    {
        renderInputInt("##redmin", -1, minR);
        ImGui::SameLine();
        renderInputInt("##redmax", -1, maxR);
        renderInputInt("##greenmin", -1, minG);
        ImGui::SameLine();
        renderInputInt("##greenmax", -1, maxG);
        renderInputInt("##bluemin", -1, minB);
        ImGui::SameLine();
        renderInputInt("##bluemax", -1, maxB);
    }
    else
    {
        renderInputInt("##redmin", -1, minR);
        ImGui::SameLine();
        renderInputInt("##redmax", -1, maxR);
    }


    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeInRange::compute(const NodeList& nodes)
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
    auto pin = p1->getOutputPin(inputPins[0].connection);

    input3 = pin->type == PinType::Image3;
    cv::Mat img1;
    if (pin->type == PinType::Image1)
        img1 = p1->getPinImage1(inputPins[0].connection);
    else
        img1 = p1->getPinImage3(inputPins[0].connection);
    if (!img1.data)
    {
        computeError = "Image not loaded";
        return;
    }

    if (pin->type == PinType::Image1)
        cv::inRange(img1, cv::Scalar(minR), cv::Scalar(maxR), image1.mat);
    else
        cv::inRange(img1, cv::Scalar(minR, minG, minB), cv::Scalar(maxR, maxG, maxB), image1.mat);
    image1.refresh();

}


void to_json(json& j, const NodeInRange& node)
{
    j["minr"] = node.minR;
    j["ming"] = node.minG;
    j["minb"] = node.minB;
    j["maxr"] = node.maxR;
    j["maxg"] = node.maxG;
    j["maxb"] = node.maxB;
}
