#include "Node.h"
#include <imnodes.h>
#include "../lib/imgui-docking/misc/cpp/imgui_stdlib.h"
#include "../NodeList.h"
#include "Nodes.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <json.hpp>
using json = nlohmann::json;


int Node::linkId = 0;



Node::Node(const NodeType t, int id, int inputs, int outputs, int& currentPinId) : type(t), id(id), inputs(inputs), outputs(outputs) {
    if (inputs > 0)
    {
        inputPins = new Pin[inputs];
        for (int i = 0; i < inputs; i++)
            inputPins[i].id = currentPinId++;
    }
    if (outputs > 0)
    {
        outputPins = new Pin[outputs];
        for (int i = 0; i < outputs; i++)
            outputPins[i].id = currentPinId++;
    }
}

Node::Node(const json& j)
{
    j["id"].get_to(id);
    j["type"].get_to(type);
    if (j.contains("inputs") && j["inputs"].size() > 0)
    {
        inputs = (int)j["inputs"].size();
        inputPins = new Pin[inputs];
        for (int i = 0; i < inputs; i++)
        {
            j["inputs"][i]["id"].get_to(inputPins[i].id);
            j["inputs"][i]["connection"].get_to(inputPins[i].connection);
            j["inputs"][i]["type"].get_to(inputPins[i].type);
        }
    }
    if (j.contains("outputs") && j["outputs"].size() > 0)
    {
        outputs = (int)j["outputs"].size();
        outputPins = new Pin[outputs];
        for (int i = 0; i < outputs; i++)
        {
            j["outputs"][i]["id"].get_to(outputPins[i].id);
            j["outputs"][i]["connection"].get_to(outputPins[i].connection);
            j["outputs"][i]["type"].get_to(outputPins[i].type);
        }
    }
}

void Node::setImage3OutputPin(int index, Image3& image, bool fromJson)
{
    if (index + 3 >= outputs)
    {
        std::cout << "Error: not enough output pins" << std::endl;
        throw "";
    }

    if(!fromJson)
        outputPins[index+0].type = PinType::Image3;
    outputPins[index+0].data = &image;
    if (!fromJson)
        outputPins[index+1].type = PinType::Image1;
    outputPins[index+1].data = &image;
    outputPins[index+1].image1Index = 1;
    if (!fromJson)
        outputPins[index+2].type = PinType::Image1;
    outputPins[index+2].data = &image;
    outputPins[index+2].image1Index = 2;
    if (!fromJson)
        outputPins[index+3].type = PinType::Image1;
    outputPins[index+3].data = &image;
    outputPins[index+3].image1Index = 3;
}

void Node::renderBegin(const char* title)
{
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(title);
    ImNodes::EndNodeTitleBar();
}

void Node::renderInput(const char* title, int index)
{
    ImNodes::BeginInputAttribute(inputPins[index].id);
    ImGui::TextUnformatted(title);
    ImNodes::EndOutputAttribute();
}

void Node::renderInputFloat(const char* title, int index, float& value)
{
    if (index != -1)
        ImNodes::BeginInputAttribute(inputPins[index].id);
    if (index == -1 || inputPins[index].connection == 0)
    {
        ImGui::PushItemWidth(100);
        if (ImGui::InputFloat(title, &value))
            computed = false;
    }
    else
        ImGui::TextUnformatted(title);
    if (index != -1)
        ImNodes::EndOutputAttribute();
}

void Node::renderInputInt(const char* title, int index, uint8_t& value)
{
    if(index != -1)
        ImNodes::BeginInputAttribute(inputPins[index].id);
    if (index == -1 || inputPins[index].connection == 0)
    {
        int v = value;
        ImGui::PushItemWidth(100);
        if (ImGui::SliderInt(title, &v, 0, 255))
            computed = false;
        value = v;
    }
    else
        ImGui::TextUnformatted(title);
    if (index != -1)
        ImNodes::EndOutputAttribute();
}
void Node::renderInputInt(const char* title, int index, int& value)
{
    if (index != -1)
        ImNodes::BeginInputAttribute(inputPins[index].id);
    if (index == -1 || inputPins[index].connection == 0)
    {
        ImGui::PushItemWidth(100);
        if (ImGui::SliderInt(title, &value, 0, 255))
            computed = false;
    }
    else
        ImGui::TextUnformatted(title);
    if (index != -1)
        ImNodes::EndOutputAttribute();
}

void Node::renderOutputImage(const char* title, int index, Image3& image)
{
    if (image.threeComponent)
    {
        ImNodes::BeginOutputAttribute(outputPins[index].id);
        ImGui::Image((ImTextureID)image.texId, ImVec2(100, 100));
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            ImGui::OpenPopup("Image Debugger RGB");
        ImGui::SameLine();
        if (ImGui::Button(image.threeComponent ? "3" : "1"))
            image.threeComponent = !image.threeComponent;
        ImGui::Text("Size: %dx%dx%d", image.mat.cols, image.mat.rows, image.mat.channels());
        
        if (ImGui::BeginPopup("Image Debugger RGB"))
        {
            ImGui::Image((ImTextureID)image.texId, ImVec2(500, 500));
            ImGui::EndPopup();
        }
        ImNodes::EndOutputAttribute();
    }
    else
    { 
        ImNodes::BeginOutputAttribute(outputPins[index+1].id);
        ImGui::Image((ImTextureID)(image.texId+1), ImVec2(100, 100));
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            ImGui::OpenPopup("Image Debugger R");
        ImGui::SameLine();
        if (ImGui::Button(image.threeComponent ? "3" : "1"))
            image.threeComponent = !image.threeComponent;

        if (ImGui::BeginPopup("Image Debugger R"))
        {
            ImGui::Image((ImTextureID)(image.texId + 1), ImVec2(500, 500));
            ImGui::EndPopup();
        }
        ImNodes::EndOutputAttribute();

        ImNodes::BeginOutputAttribute(outputPins[index+2].id);
        ImGui::Image((ImTextureID)(image.texId+2), ImVec2(100, 100));
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            ImGui::OpenPopup("Image Debugger G");
        if (ImGui::BeginPopup("Image Debugger G"))
        {
            ImGui::Image((ImTextureID)(image.texId + 2), ImVec2(500, 500));
            ImGui::EndPopup();
        }
        ImNodes::EndOutputAttribute();
        ImNodes::BeginOutputAttribute(outputPins[index+3].id);
        ImGui::Image((ImTextureID)(image.texId+3), ImVec2(100, 100));
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            ImGui::OpenPopup("Image Debugger B");
        if (ImGui::BeginPopup("Image Debugger B"))
        {
            ImGui::Image((ImTextureID)(image.texId + 3), ImVec2(500, 500));
            ImGui::EndPopup();
        }
        ImNodes::EndOutputAttribute();
    } 

}


cv::Mat Node::getPinImage3(int pinId) 
{ 
    auto pin = getOutputPin(pinId);
    if (pin->data && pin->type == PinType::Image3)
        return ((Image3*)pin->data)->mat;
    return cv::Mat();
}

cv::Mat Node::getPinImage1(int pinId) 
{ 
    auto pin = getOutputPin(pinId);
    if (pin->data && pin->type == PinType::Image1)
    {
        if (pin->image1Index > 0) //is secretly an image3
        {
            auto img = (Image3*)pin->data;
            cv::Mat planes[3];
            cv::split(img->mat, planes);
            return planes[pin->image1Index - 1];
        }
        else
        {
            return ((Image1*)pin->data)->mat;
        }
    }
    return cv::Mat();

}


void Node::renderOutputImage(const char* title, int index, Image1& image)
{
    ImNodes::BeginOutputAttribute(outputPins[index].id);
    ImGui::Image((ImTextureID)image.texId, ImVec2(100, 100));
    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        ImGui::OpenPopup("Image Debugger");
    if (ImGui::BeginPopup("Image Debugger"))
    {
        ImGui::Image((ImTextureID)(image.texId), ImVec2(500, 500));
        ImGui::EndPopup();
    }
    ImGui::Text("Size: %dx%dx%d", image.mat.cols, image.mat.rows, image.mat.channels());
    ImNodes::EndOutputAttribute();
}

void Node::renderEnd()
{
    ImNodes::EndNode();
}

void Node::renderLinks(const NodeList& nodes)
{
    for (int i = 0; i < inputs; i++)
        if (inputPins[i].connection != 0)
        {
            auto connectedNode = nodes.findNodeWithPin(inputPins[i].connection);
            auto pin = connectedNode ? connectedNode->getOutputPin(inputPins[i].connection) : nullptr;
            if (pin && pin->type == PinType::Image1)
                ImNodes::PushColorStyle(ImNodesCol_Link, 0xff0000ff);
            if (pin && pin->type == PinType::Image3)
                ImNodes::PushColorStyle(ImNodesCol_Link, 0xffffffff);

            ImNodes::Link(linkId++, inputPins[i].id, inputPins[i].connection);
            if (pin && (pin->type == PinType::Image1 || pin->type == PinType::Image3))
                ImNodes::PopColorStyle();
        }

}

Pin* Node::getOutputPin(int pinId)
{
    for (int i = 0; i < outputs; i++)
        if (outputPins[i].id == pinId)
            return &outputPins[i];
    return nullptr;
}
Pin* Node::getInputPin(int pinId)
{
    for (int i = 0; i < inputs; i++)
        if (inputPins[i].id == pinId)
            return &inputPins[i];
    return nullptr;
}

void Node::setPin(int pinId, int pinValue)
{
    for (int i = 0; i < inputs; i++)
        if (inputPins[i].id == pinId)
            inputPins[i].connection = pinValue;
    for (int i = 0; i < outputs; i++)
        if (outputPins[i].id == pinId)
            outputPins[i].connection = pinValue;

}



void to_json(json& j, const Node& node) {
    j["id"] = node.id;
    j["type"] = node.type;
    for (int i = 0; i < node.inputs; i++)
        j["inputs"].push_back(json{ { "id", node.inputPins[i].id }, { "connection", node.inputPins[i].connection}, {"type", node.inputPins[i].type} });
    for (int i = 0; i < node.outputs; i++)
        j["outputs"].push_back(json{ { "id", node.outputPins[i].id }, { "connection", node.outputPins[i].connection}, {"type", node.outputPins[i].type} });

    if (const NodeImageLoad* nodeImageLoad = dynamic_cast<const NodeImageLoad*>(&node))
        to_json(j, *nodeImageLoad);
    else if (const NodeCameraStream* nodeCameraStream = dynamic_cast<const NodeCameraStream*>(&node))
        to_json(j, *nodeCameraStream);
    else if (const NodeAdd* nodeAdd = dynamic_cast<const NodeAdd*>(&node))
        to_json(j, *nodeAdd);
    else if (const NodeConvertColor* nodeRgb2Hsv = dynamic_cast<const NodeConvertColor*>(&node))
        to_json(j, *nodeRgb2Hsv);
    else if (const NodeThreshold* nodeThreshold = dynamic_cast<const NodeThreshold*>(&node))
        to_json(j, *nodeThreshold);
    else if (const NodeErode* nodeErode = dynamic_cast<const NodeErode*>(&node))
        to_json(j, *nodeErode);
    else if (const NodeDilate* nodeDilate = dynamic_cast<const NodeDilate*>(&node))
        to_json(j, *nodeDilate);
    else if (const NodeResize* nodeResize = dynamic_cast<const NodeResize*>(&node))
        to_json(j, *nodeResize);
    else if (const NodeCrop* nodeCrop = dynamic_cast<const NodeCrop*>(&node))
        to_json(j, *nodeCrop);
    else if (const NodeHistogram* nodeHistogram = dynamic_cast<const NodeHistogram*>(&node))
        to_json(j, *nodeHistogram);
    else if (const NodeNormalize* nodeNormalize = dynamic_cast<const NodeNormalize*>(&node))
        to_json(j, *nodeNormalize);
    else if (const NodeInRange* nodeInRange = dynamic_cast<const NodeInRange*>(&node))
        to_json(j, *nodeInRange);
}

Image3::Image3()
{
    glGenTextures(4, &texId);
    for (int i = 0; i < 4; i++)
    {
        glBindTexture(GL_TEXTURE_2D, texId + i);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

}

void Image3::refresh()
{
    if (mat.rows != 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, (int)mat.step / (int)mat.elemSize());
        glBindTexture(GL_TEXTURE_2D, texId);
        if (mat.channels() == 1)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.ptr());
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, mat.ptr());

            glBindTexture(GL_TEXTURE_2D, texId + 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr());

            glBindTexture(GL_TEXTURE_2D, texId + 2);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr() + 1);

            glBindTexture(GL_TEXTURE_2D, texId + 3);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.ptr() + 2);
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

Image1::Image1()
{
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Image1::refresh()
{
    glBindTexture(GL_TEXTURE_2D, texId);
    if (mat.rows != 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, (int)mat.step / (int)mat.elemSize());

        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mat.cols, mat.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mat.ptr());
    }
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}
