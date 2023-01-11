
#include <iostream>
#include <string>


#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vertualfs.hpp>

#include "commands.hpp"
#include "commandscli.hpp"



int main2(int argc, const char** argv)
{
    if (!vertualfs_startup()) { return false; }

    std::string cliresult;
    vertualfstool::commandscli(argc, argv, cliresult);
    vertualfstool::commandscli(argv[0], "help", cliresult);
    vertualfstool::commandscli(argv[0], "create volume nachos", cliresult);
    vertualfstool::commandscli(argv[0], "ls volumes", cliresult);
    vertualfstool::commandscli(argv[0], "cd volume nachos", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository c:/repos/headtest tag", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository https://github.com/billyzelsnack/simple_onshape_exportstls.git tag", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository https://gitlab.com/telemotor/users/billy/headtest tag", cliresult);
    vertualfstool::commandscli(argv[0], "ensureavailable repository c:/repos/headtest tag", cliresult);
    vertualfstool::commandscli(argv[0], "ls repositories", cliresult);

    while(!vertualfstool::exited())
    {
        std::cout << vertualfstool::prompt() << std::flush;
        std::string line;
        std::getline(std::cin, line);

        if(!vertualfstool::commandscli(argv[0], line, cliresult))
        { 
            printf("cliresult [%s]\n",cliresult.c_str()); 
        }
    }

    vertualfs_shutdown();

    return EXIT_SUCCESS;
}







static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit()) { return 1; }

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

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL) { return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}














