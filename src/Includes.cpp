#include "gl_core_3_3.hpp"
#include <glm/glm.hpp>
#include "Includes.hpp"

glm::vec2 size(100, 50);
GLFWwindow *window;

bool initWindow(){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (glfwInit() != 1){
        error("glfw init fail");
        return false;
    }

    glfwWindowHint(GLFW_RESIZABLE, gl::FALSE_);
    // glfwWindowHint(GLFW_DECORATED, gl::FALSE_);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    size.x = std::min((int)size.x, mode->width);
    size.y = std::min((int)size.y, mode->height);

    window = glfwCreateWindow(size.x, size.y, "Input Handler", nullptr, nullptr);
    glfwSetWindowPos(window, 400, 100);
    // glfwHideWindow(window);
    glfwShowWindow(window);

    if(!window){
        error("creating context fail");
        glfwTerminate();
        return false;
    }
    glfwSetWindowTitle(window, "Input Handler");
    glfwMakeContextCurrent(window);

    gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
    if(!didLoad){
        error("GL init fail");
        return false;
    }
    return true;
}

void refresh(){

}
