#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 

    // FragColor.rgb = texture(screenTexture, TexCoords).rgb;
    FragColor.rg *= TexCoords;
    FragColor.a = 1.0;
    // FragColor += vec4(0.1, 0.2, 0.1, 0.0);
}