#include "NodeHistogram.h"
#include <imnodes.h>
#include <misc/cpp/imgui_stdlib.h>

void NodeHistogram::compute(const NodeList& nodes)
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
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    int histSize = 256;
    bool uniform = true, accumulate = false;
    std::vector<cv::Mat> bgr_planes;
    cv::split(img, bgr_planes);

    computed = true;
    cv::Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);


    int hist_w = 512, hist_h = 512;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    for (int i = 1; i < histSize; i++)
    {
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
            cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
            cv::Scalar(255, 0, 0), 1, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
            cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
            cv::Scalar(0, 255, 0), 1, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
            cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
            cv::Scalar(0, 0, 255), 1, 8, 0);
    }
    image.mat = histImage;

    image.refresh();
}

void NodeHistogram::render()
{
	renderBegin("Histogram");

	renderInput("Image", 0);

    ImGui::Image((ImTextureID)(image.texId), ImVec2(512, 512));
    ImGui::Image((ImTextureID)(image.texId+3), ImVec2(256, 256));
    ImGui::SameLine();
    ImGui::Image((ImTextureID)(image.texId+2), ImVec2(256, 256));
    ImGui::SameLine();
    ImGui::Image((ImTextureID)(image.texId+1), ImVec2(256, 256));

	renderEnd();
}

void to_json(json& j, const NodeHistogram& node)
{
}
