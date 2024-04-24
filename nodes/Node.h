#pragma once

#pragma warning(disable: 4312)

#include <string>
#include <opencv2/core.hpp>
#include <GLFW/glfw3.h>
#include "../Pin.h"
#include <json.hpp>
using json = nlohmann::json;

class NodeList;

enum class NodeType
{
    ImageLoad,
    CameraStream,
    Resize,
    Add,
    Output,
    ConvertColor,
    Threshold,
    ImgCrop,
    Multiply,
    WarpPerspective,
    Convolve,
    Canny,
    Dilate,
    Erode,
    Histogram,
    Subtract,
    InRange,
    DistanceTransform,
    Normalize,
};



class Image3
{
public:
    cv::Mat mat;
    GLuint texId;
    bool threeComponent = true;
    Image3();
    void refresh();
};

class Image1
{
public:
    cv::Mat mat;
    GLuint texId;
    Image1();
    void refresh();
};


class Node
{
public:
    NodeType type;
    int id;
    bool computed = false;
    std::string computeError = "";

    Pin* inputPins = nullptr;
    int inputs = 0;
    Pin* outputPins = nullptr;
    int outputs = 0;

    virtual void render() = 0;
    void renderLinks(const NodeList& nodes);
    virtual void compute(const NodeList& nodes) = 0;

    virtual cv::Mat getPinImage3(int pinId);
    virtual cv::Mat getPinImage1(int pinId);;
    virtual int getPinInt(int pinId) { throw "oops"; };

protected:
    Node(const NodeType t, int id, int inputs, int outputs, int& currentPinId);

    Node(const json& j);

    static int linkId; //TODO: save these
    void renderBegin(const char* title);
    void renderEnd();

    void renderInput(const char* title, int index);
    void renderInputFloat(const char* title, int index, float& value);
    void renderInputInt(const char* title, int index, uint8_t& value);
    void renderInputInt(const char* title, int index, int& value);
    void renderOutputImage(const char* title, int index, Image3& image);
    void renderOutputImage(const char* title, int index, Image1& image);

    void setImage3OutputPin(int index, Image3& image, bool fromJson = false);
public:
    Pin* getOutputPin(int pinId);
    Pin* getInputPin(int pinId);
    void setPin(int pinId, int pinValue);
};










void to_json(json& j, const Node& node);
