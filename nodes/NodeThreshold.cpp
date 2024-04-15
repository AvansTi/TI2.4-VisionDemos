#include "NodeThreshold.h"
#include <imnodes.h>
#include "../NodeList.h"
#include <misc/cpp/imgui_stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

/*
enum ThresholdTypes {
    THRESH_BINARY     = 0, //!< \f[\texttt{dst} (x,y) =  \fork{\texttt{maxval}}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{0}{otherwise}\f]
    THRESH_BINARY_INV = 1, //!< \f[\texttt{dst} (x,y) =  \fork{0}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{\texttt{maxval}}{otherwise}\f]
    THRESH_TRUNC      = 2, //!< \f[\texttt{dst} (x,y) =  \fork{\texttt{threshold}}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{\texttt{src}(x,y)}{otherwise}\f]
    THRESH_TOZERO     = 3, //!< \f[\texttt{dst} (x,y) =  \fork{\texttt{src}(x,y)}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{0}{otherwise}\f]
    THRESH_TOZERO_INV = 4, //!< \f[\texttt{dst} (x,y) =  \fork{0}{if \(\texttt{src}(x,y) > \texttt{thresh}\)}{\texttt{src}(x,y)}{otherwise}\f]
    THRESH_MASK       = 7,
    THRESH_OTSU       = 8, //!< flag, use Otsu algorithm to choose the optimal threshold value
    THRESH_TRIANGLE   = 16 //!< flag, use Triangle algorithm to choose the optimal threshold value
};
*/
void NodeThreshold::render()
{
    renderBegin("Threshold");

    renderInput("Input", 0);
    renderInputInt("Threshold", 1, threshold);
    renderInputInt("Max Value", 2, maxValue);
    
    if (ImGui::Combo("Type", &thresholdType, "Binary\0Binary_Inv\0Trunc\0ToZero\0ToZero_Inv\0Mask\0Otsu\0Triangle"))
        computed = false;


    renderOutputImage("Image", 0, image);


    if (computeError != "")
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), computeError.c_str());

    renderEnd();

}

void NodeThreshold::compute(const NodeList& nodes)
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

    auto img1 = p1->getPinImage1(inputPins[0].connection);
    if (!img1.data)
    {
        computeError = "Image not loaded";
        return;
    }
    if (img1.cols != image.mat.cols || img1.cols != image.mat.rows)
    {
        image.mat = cv::Mat(img1.cols, img1.rows, img1.type());
    }

    cv::threshold(img1, image.mat, threshold, maxValue, (cv::ThresholdTypes)thresholdType);
    image.refresh();

}
cv::Mat NodeThreshold::getPinImage3(int pinId)
{
    return image.mat; //TODO: check pinId, though that's pointless here. also convert to image1
}
cv::Mat NodeThreshold::getPinImage1(int pinId)
{
    return image.mat; //TODO: check pinId, though that's pointless here. also double check if image1
}


void to_json(json& j, const NodeThreshold& node)
{
}
