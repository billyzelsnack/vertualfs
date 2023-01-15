
#include "windowing.hpp"

#include <iostream>

#include <GLFW/glfw3.h>
#include <IconsFontAwesome4.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>







//#define VERTUALFSDEMO_WINDOWING_GLFW
#if defined(VERTUALFSDEMO_WINDOWING_GLFW)

GLFWwindow* gwindow = nullptr;

bool windowing_framebegin()
{
    if(glfwWindowShouldClose(gwindow)) { return false; }
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return true;
}

bool windowing_frameend()
{
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(gwindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(gwindow);

    return true;
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int argc, const char** argv)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) { return EXIT_FAILURE; }

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    gwindow = glfwCreateWindow(1280, 720, "virtualfsdemo", NULL, NULL);
    if (gwindow == NULL) { return 1; }
    glfwMakeContextCurrent(gwindow);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(gwindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    io.Fonts->AddFontDefault();
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    {
        ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = false; icons_config.GlyphOffset = ImVec2(0, 2);
        io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FA, 16.0f, &icons_config, icons_ranges);
    }

    int result = windowing_main(argc, argv);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(gwindow);
    glfwTerminate();

    return result;
}

#endif


