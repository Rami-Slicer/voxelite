#include <util.hpp>

unsigned int get_max(std::vector<unsigned int> in) {
    unsigned int max = 0;
    for(int i = 0; i < in.size(); i++) {
        if(in[i] > max) max = in[i];
    }
    return max;
}

void CheckGLError() {	GLuint err = glGetError();	if (err != GL_NO_ERROR)		std::cerr << std::hex << err << std::endl;}

void genFramebuffer(unsigned int * buf, int width, int height) {
    glGenFramebuffers(1, buf);
    glBindFramebuffer(GL_FRAMEBUFFER, *buf);

    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    // create a depth stencil attachment texture

    unsigned int textureDepthStencilbuffer;
    glGenTextures(1, &textureDepthStencilbuffer);

    glBindTexture(GL_TEXTURE_2D, textureDepthStencilbuffer);

    
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, 
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureDepthStencilbuffer, 0);
}