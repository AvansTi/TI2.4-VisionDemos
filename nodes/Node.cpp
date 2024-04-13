#include "Node.h"
#include <imnodes.h>
#include "../lib/imgui-docking/misc/cpp/imgui_stdlib.h"
#include "../NodeList.h"
#include "Nodes.h"


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
    ImNodes::BeginInputAttribute(inputPins[index].id);
    if (inputPins[index].connection == 0)
    {
        ImGui::PushItemWidth(100);
        if (ImGui::InputFloat(title, &value))
            computed = false;
    }
    else
        ImGui::TextUnformatted(title);
    ImNodes::EndOutputAttribute();
}

void Node::renderInputInt(const char* title, int index, uint8_t& value)
{
    ImNodes::BeginInputAttribute(inputPins[index].id);
    if (inputPins[index].connection == 0)
    {
        int v = value;
        ImGui::PushItemWidth(100);
        if (ImGui::SliderInt(title, &v, 0, 255))
            computed = false;
        value = v;
    }
    else
        ImGui::TextUnformatted(title);
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
    ImGui::SameLine();
    ImNodes::EndOutputAttribute();
}

void Node::renderEnd()
{
    ImNodes::EndNode();
    for (int i = 0; i < inputs; i++)
        if (inputPins[i].connection != 0)
            ImNodes::Link(linkId++, inputPins[i].id, inputPins[i].connection);
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
    else if (const NodeAdd* nodeAdd = dynamic_cast<const NodeAdd*>(&node))
        to_json(j, *nodeAdd);
    else if (const NodeConvertColor* nodeRgb2Hsv = dynamic_cast<const NodeConvertColor*>(&node))
        to_json(j, *nodeRgb2Hsv);

}

