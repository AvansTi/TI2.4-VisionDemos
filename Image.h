#pragma once

#include <opencv2/core.hpp>
#include <GLFW/glfw3.h>

class Image
{
public:
    cv::Mat mat;
    GLuint texId;

    virtual ~Image() {}
};
class Image3 : public Image
{
public:
    bool threeComponent = true;
    Image3();
    void refresh();
};

class Image1 : public Image
{
public:
    Image1();
    void refresh();
};