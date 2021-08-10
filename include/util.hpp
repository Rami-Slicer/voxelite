#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int get_max(std::vector<unsigned int> in);

void CheckGLError();
// #ifndef NDEBUG
// #	define CHECK_GL_ERROR CheckGLError();
// #else
// #	define CHECK_GL_ERROR
// #endif

void genFramebuffer(unsigned int * buf, int width, int height);

struct WindowInfo {
    int width;
    int height;
};