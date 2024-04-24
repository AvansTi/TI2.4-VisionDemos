//NOTICE
// This code was made in a rush with little time, so it is not the best example of coding styles
// feel free to improve and send in pull requests
#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#include <commdlg.h>
#endif

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imnodes.h>
#include <stdio.h>
#include <fstream>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include "nodes/Node.h"
#include "nodes/Nodes.h"
#include "NodeList.h"
#include <vector>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <json.hpp>
using json = nlohmann::json;



static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

//state
NodeList   nodes;
GLFWwindow* window;
ImNodesMiniMapLocation minimap_location_ = ImNodesMiniMapLocation_TopLeft;
int currentNodeId = 1;
int currentPinId = 1;
std::string currentFileName = "";

void init();
void update();
void draw();
void refresh();
std::string GetSaveFileName(std::string defaultFilename = "");
std::string GetLoadFileName();

int main(int argc, char** argv)
{
    init();

    ImGuiIO& io = ImGui::GetIO(); (void)io;


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        update();

        draw();

        ImGui::End();

        // actual Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void init()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1280, 720, "Vision Flow Editor", nullptr, nullptr);
    if (window == nullptr)
        return;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImNodes::CreateContext();
}

void update()
{
    bool ok = true;
    for (auto node : nodes)
        if (!node->computed)
            ok = false;
    if (!ok)
        refresh();



    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().
    {
        int start_attr, end_attr;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            //start_attr and end_attr might be swapped if dragged in wrong order
            Node* nodeStart = nodes.findNodeWithPin(start_attr);
            Node* nodeEnd = nodes.findNodeWithPin(end_attr);
            Pin* pinStart = nodeStart->getOutputPin(start_attr);
            Pin* pinEnd = nodeEnd->getInputPin(end_attr);

            const bool valid_link = pinStart && pinEnd && ((int)pinStart->type & (int)pinEnd->type) != 0 && pinStart->type != PinType::Unknown;
            if (valid_link)
            {
                //TODO: check if the output of the startAttr isn't linked to something else already. if it is, break that connection first or we can have nodes linked to different outputs. I guess don't even have to store the output pins?
                nodeStart->setPin(start_attr, end_attr);
                nodeEnd->setPin(end_attr, start_attr);
                refresh();
            }
        }
    }

    /* { //delete links
        const int num_selected = ImNodes::NumSelectedLinks();
        if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X) && !ImGui::GetIO().WantCaptureKeyboard)
        {
            static std::vector<int> selected_links;
            selected_links.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedLinks(selected_links.data());
            for (const int edge_id : selected_links)
            {
                graph_.erase_edge(edge_id);
            }
        }
    }*/

    { //delete nodes
        const int num_selected = ImNodes::NumSelectedNodes();
        if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X) && !ImGui::GetIO().WantCaptureKeyboard)
        {
            static std::vector<int> selected_nodes;
            selected_nodes.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedNodes(selected_nodes.data());

            for(int i = (int)nodes.size()-1; i >= 0; i--)
            {
                if (std::find(selected_nodes.begin(), selected_nodes.end(), nodes[i]->id) == selected_nodes.end())
                    continue;
                delete nodes[i];
                nodes.erase(nodes.begin() + i);
            }
        }
    }

}

inline bool ends_with(std::string const& value, std::string const& ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
void draw()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::Begin("Vision node editor", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                for (auto n : nodes)
                    delete n;
                nodes.clear();
            }
            if (ImGui::MenuItem("Open"))
            {
                std::string fileName = GetLoadFileName();
                if (fileName != "")
                {
                    for (auto n : nodes)
                        delete n;
                    nodes.clear();


                    std::ifstream ifile(fileName);
                    if (ifile.is_open() && !ifile.bad())
                    {
                        json data;
                        ifile >> data;
                        nodes = data.get<NodeList>();
                        ImNodes::LoadCurrentEditorStateFromIniFile((fileName + ".ini").c_str());

                        for (auto node : nodes)
                        {
                            currentNodeId = std::max(currentNodeId + 1, node->id+1);
                            for (int i = 0; i < node->inputs; i++)
                                currentPinId = std::max(node->inputPins[i].id + 1, currentPinId);
                            for (int i = 0; i < node->outputs; i++)
                                currentPinId = std::max(node->outputPins[i].id + 1, currentPinId);

                        }
                        std::cout << "Current node id: " << currentNodeId << ", Current Pin Id: " << currentPinId << std::endl;
                    }
                    currentFileName = fileName;
                }
            }
            if (ImGui::MenuItem("Save"))
            {
                if (currentFileName == "")
                {
                    auto newFileName = GetSaveFileName();
                    if (newFileName != "")
                    {
                        currentFileName = newFileName;
                        if (!ends_with(currentFileName, ".vision"))
                            currentFileName += ".vision";
                    }
                }
                if (currentFileName != "")
                {
                    ImNodes::SaveCurrentEditorStateToIniFile((currentFileName + ".ini").c_str());
                    json j = nodes;
                    std::ofstream file(currentFileName);
                    file << j;
                }
            }
            if (ImGui::MenuItem("Save As"))
            {
                auto newFileName = GetSaveFileName();
                if (newFileName != "")
                {
                    currentFileName = newFileName;
                    if (!ends_with(currentFileName, ".vision"))
                        currentFileName += ".vision";
                    ImNodes::SaveCurrentEditorStateToIniFile((currentFileName + ".ini").c_str());
                    json j = nodes;
                    std::ofstream file(currentFileName);
                    file << j;
                }
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Mini-map"))
        {
            const char* names[] = {
                "Top Left",
                "Top Right",
                "Bottom Left",
                "Bottom Right",
            };
            int locations[] = {
                ImNodesMiniMapLocation_TopLeft,
                ImNodesMiniMapLocation_TopRight,
                ImNodesMiniMapLocation_BottomLeft,
                ImNodesMiniMapLocation_BottomRight,
            };

            for (int i = 0; i < 4; i++)
            {
                bool selected = minimap_location_ == locations[i];
                if (ImGui::MenuItem(names[i], NULL, &selected))
                    minimap_location_ = locations[i];
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Style"))
        {
            if (ImGui::MenuItem("Classic"))
            {
                ImGui::StyleColorsClassic();
                ImNodes::StyleColorsClassic();
            }
            if (ImGui::MenuItem("Dark"))
            {
                ImGui::StyleColorsDark();
                ImNodes::StyleColorsDark();
            }
            if (ImGui::MenuItem("Light"))
            {
                ImGui::StyleColorsLight();
                ImNodes::StyleColorsLight();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }


    ImNodes::BeginNodeEditor();

    const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImNodes::IsEditorHovered() && ImGui::IsKeyReleased(ImGuiKey_A);

    if (!ImGui::IsAnyItemHovered() && open_popup)
    {
        ImGui::OpenPopup("add node");
    }

    {
        const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
            ImNodes::IsEditorHovered() && ImGui::IsKeyReleased(ImGuiKey_A);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
        if (!ImGui::IsAnyItemHovered() && open_popup)
        {
            ImGui::OpenPopup("add node");
        }

        if (ImGui::BeginPopup("add node"))
        {
            const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

            auto size = nodes.size();
            if (ImGui::MenuItem("ImageLoad"))
                nodes.push_back(new NodeImageLoad(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("VideoCapture"))
                nodes.push_back(new NodeCameraStream(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Resize"))
                nodes.push_back(new NodeResize(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Add"))
                nodes.push_back(new NodeAdd(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Subtract"))
                nodes.push_back(new NodeSubtract(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Convert Color Space"))
                nodes.push_back(new NodeConvertColor(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Threshold"))
                nodes.push_back(new NodeThreshold(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Erode"))
                nodes.push_back(new NodeErode(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Dilate"))
                nodes.push_back(new NodeDilate(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Histogram"))
                nodes.push_back(new NodeHistogram(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("InRange"))
                nodes.push_back(new NodeInRange(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("DistanceTransform"))
                nodes.push_back(new NodeDistanceTransform(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Normalize"))
                nodes.push_back(new NodeNormalize(currentNodeId++, currentPinId));
            if (ImGui::MenuItem("Convolve"))
                nodes.push_back(new NodeConvolve(currentNodeId  ++, currentPinId));

            if(size != nodes.size())
                ImNodes::SetNodeScreenSpacePos(currentNodeId - 1, click_pos);


            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }


    for (const auto& node : nodes)
        node->render();

    for (const auto& node : nodes)
        node->renderLinks(nodes);

    ImNodes::MiniMap(0.2f, minimap_location_);
    ImNodes::EndNodeEditor();
}   

void refresh()
{
    for (auto& node : nodes)
    {
        node->computed = false;
        node->computeError = "";
    }
    bool allOk = false;
    while (!allOk)
    {
        allOk = true;
        for (auto& node : nodes)
        {
            if(!node->computed)
                node->compute(nodes);
            if (node->computed == false)
                allOk = false;
        }
    }

    //json j = nodes;
    //std::cout << j<< std::endl;
}



#ifdef _WIN32
std::string GetSaveFileName(std::string defaultFilename)
{
    CoInitializeEx(0, 0);
    char curdir[100];
    _getcwd(curdir, 100);

    HWND hWnd = nullptr;
    char buf[256];
    ZeroMemory(&buf, sizeof(buf));
    strcpy_s(buf, 256, defaultFilename.c_str());

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = 1024;
    ofn.lpstrFilter = "Vision File\0*.vision\0All Files\0*.*\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
    if (GetSaveFileNameA(&ofn))
    {
        _chdir(curdir);
        return buf;
    }
    _chdir(curdir);
    return "";
}
std::string GetLoadFileName()
{
    CoInitializeEx(0, 0);
    char curdir[100];
    _getcwd(curdir, 100);

    HWND hWnd = nullptr;
    char buf[256];
    ZeroMemory(&buf, sizeof(buf));

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = 1024;
    ofn.lpstrFilter = "Vision File\0*.vision\0All Files\0*.*\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT;
    if (GetOpenFileNameA(&ofn))
    {
        _chdir(curdir);
        return buf;
    }
    _chdir(curdir);
    return "";
}
#else
std::string GetLoadFileName() { return "file.vision"; }
std::string GetSaveFileName(std::string defaultFilename = "") { return "file.vision"; }
#endif
