#include "NodeConvolve.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <misc/cpp/imgui_stdlib.h>

void NodeConvolve::render()
{
    renderBegin("Convolute Filter");

    renderInput("Input", 0);
    ImGui::PushItemWidth(100);
    if (ImGui::SliderInt("Kernel Size", &size, 1, 9))
        computed = false;

    if(computed)
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            ImGui::SetNextItemWidth(50);
            if (ImGui::SliderFloat(("##" + std::to_string(row) + "_" + std::to_string(col)).c_str(), &kernel.at<float>(col, row), -10, 10))
                computed = false;
            if (col < size - 1)
                ImGui::SameLine();
        }
    }


    renderOutputImage("Image", 0, image);


    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeConvolve::compute(const NodeList& nodes)
{
    if (kernel.cols < size)
    {
        cv::Mat padded = cv::Mat(size, size, CV_32F, cv::Scalar(1));
        kernel.copyTo(padded(cv::Rect(0, 0, kernel.cols, kernel.rows)));
        kernel = padded;
    }
    if (kernel.cols > size)
        kernel = kernel(cv::Rect(0, 0, size, size));

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


    cv::filter2D(img, image.mat, -1, kernel);
    
    image.refresh();

}


void to_json(json& j, const NodeConvolve& node)
{


}
