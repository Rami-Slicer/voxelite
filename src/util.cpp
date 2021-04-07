#include <util.hpp>

unsigned int get_max(std::vector<unsigned int> in) {
    unsigned int max = 0;
    for(int i = 0; i < in.size(); i++) {
        if(in[i] > max) max = in[i];
    }
    return max;
}

void CheckGLError() {	GLuint err = glGetError();	if (err != GL_NO_ERROR)		std::cerr << std::hex << err << std::endl;}
