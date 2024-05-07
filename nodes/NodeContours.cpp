#include "NodeContours.h"
#include <imnodes.h>
#include <misc/cpp/imgui_stdlib.h>

void NodeContours::compute(const NodeList& nodes)
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
    img = pin->type == PinType::Image1 ? node->getPinImage1(inputPins[0].connection) : node->getPinImage3(inputPins[0].connection);
    if (!img.data)
    {
        computeError = "Image not loaded";
        return;
    }
    cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

}

void NodeContours::render()
{
	renderBegin("Contours");

	renderInput("Image", 0);

    ImGui::BeginListBox("##Contours");
    for (int i = 0; i < contours.size(); i++)
    {
        if (ImGui::Selectable(("Contour " + std::to_string(i)).c_str(), selectedContour == i))
            selectedContour = i;
    }
    ImGui::EndListBox();
    ImGui::SameLine();
    ImGui::BeginChild(ImGui::GetID("sub"), ImVec2(200,300));
    if (contours.size() > 0 && selectedContour < contours.size())
    {
        ImGui::PushItemWidth(100);
        ImGui::LabelText("Area", "%f", cv::contourArea(contours[selectedContour]));
        float peri = cv::arcLength(contours[selectedContour], true);
        ImGui::LabelText("Perimeter", "%f", peri);
        std::vector<cv::Point> conPoly;
        cv::approxPolyDP(contours[selectedContour], conPoly, 0.02 * peri, true);
        ImGui::LabelText("Convex hull corner count", "%i", conPoly.size());

        cv::Rect boundingRect = cv::boundingRect(conPoly);

        if (image.mat.channels() == 3)
            image.mat = img.clone();
        else
            cv::cvtColor(img, image.mat, cv::COLOR_GRAY2BGR);
        cv::rectangle(image.mat, boundingRect, cv::Scalar(255, 0, 255), 5);
        image.refresh();
        ImGui::Image((ImTextureID)image.texId, ImVec2(200, 200));


        ImGui::PopItemWidth();
    }
    ImGui::EndChild();



	renderEnd();
}

void to_json(json& j, const NodeContours& node)
{
}
