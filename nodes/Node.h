#pragma once

#pragma warning(disable: 4312)

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <GLFW/glfw3.h>
#include "../Pin.h"
#include <iostream>
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
    GausianBlur,
    Canny,
    Dilate,
    Erode,
};



class Image3
{
public:
    cv::Mat mat;
    GLuint texId;
    bool threeComponent = true;
    Image3()
    {
        glGenTextures(4, &texId);
        for (int i = 0; i < 4; i++)
        {
            glBindTexture(GL_TEXTURE_2D, texId+i);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

    }
    void refresh()
    {
        if (mat.rows != 0)
        {
            glBindTexture(GL_TEXTURE_2D, texId);
            if (mat.channels() == 1)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.ptr());
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr());

                glBindTexture(GL_TEXTURE_2D, texId + 1);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr());

                glBindTexture(GL_TEXTURE_2D, texId + 2);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr() + 4);

                glBindTexture(GL_TEXTURE_2D, texId + 3);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr() + 8);
            }

        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                glBindTexture(GL_TEXTURE_2D, texId + i);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            }
        }
    }
};

class Image1
{
public:
    cv::Mat mat;
    GLuint texId;
    Image1()
    {
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    void refresh()
    {
        glBindTexture(GL_TEXTURE_2D, texId);
        if (mat.rows != 0)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.ptr());
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }
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

    virtual cv::Mat getPinImage3(int pinId) { throw "oops"; };
    virtual cv::Mat getPinImage1(int pinId) { throw "oops"; };
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
    void renderOutputImage(const char* title, int index, Image3& image);
    void renderOutputImage(const char* title, int index, Image1& image);

public:
    Pin* getOutputPin(int pinId)
    {
        for (int i = 0; i < outputs; i++)
            if (outputPins[i].id == pinId)
                return &outputPins[i];
        return nullptr;
    }
    Pin* getInputPin(int pinId)
    {
        for (int i = 0; i < inputs; i++)
            if (inputPins[i].id == pinId)
                return &inputPins[i];
        return nullptr;
    }

    void setPin(int pinId, int pinValue)
    {
        for (int i = 0; i < inputs; i++)
            if (inputPins[i].id == pinId)
                inputPins[i].connection = pinValue;
        for (int i = 0; i < outputs; i++)
            if (outputPins[i].id == pinId)
                outputPins[i].connection = pinValue;

    }
};










void to_json(json& j, const Node& node);
