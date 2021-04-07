#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

float near = 0.1; 
float far  = 100.0; 

const float offset = 1.0 / 300.0;  

void main()
{
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset,  offset), // top-left
    //     vec2( 0.0f,    offset), // top-center
    //     vec2( offset,  offset), // top-right
    //     vec2(-offset,  0.0f),   // center-left
    //     vec2( 0.0f,    0.0f),   // center-center
    //     vec2( offset,  0.0f),   // center-right
    //     vec2(-offset, -offset), // bottom-left
    //     vec2( 0.0f,   -offset), // bottom-center
    //     vec2( offset, -offset)  // bottom-right    
    // );

    // float kernel[9] = float[](
    //     0.125, 0.125, 0.125,
    //     0.125, 0.000, 0.125,
    //     0.125, 0.125, 0.125
    // );
    
    // vec3 sampleTex[9];
    // float avg[9];
    // vec3 center = texture(screenTexture, TexCoords).rgb;
    
    // for(int i = 0; i < 9; i++)
    // {   
    //     vec3 a = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    //     sampleTex[i] = a;
    //     avg[i] = (a.r + a.g + a.b)/3.0;

    // }
    
    // vec3 col;

    // for(int i = 0; i < 9; i++) {
    //     if(avg[i] < 0.4) {
    //         col += sampleTex[i] * kernel[i];
    //     } else {
    //         col = vec3(0.0);
    //     }
    // }

    // // code to get linear depth from the framebuffer
    // float depth = texture(depthTexture, TexCoords).r;
    // float ndc = depth * 2.0 - 1.0; 
    // float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));	
    // // FragColor.rgb *= vec3(linearDepth/far);

    // float fac = linearDepth/far;
    // fac = 1-fac;
    // fac = max(fac-0.5, 0);
        
    // if(col.r + col.g + col.b > 0.0) {
    //     FragColor = vec4(mix(center, col, fac), 1.0);
    // } else {
    //     FragColor = vec4(center, 1.0);
    // }
    
    FragColor.rgb = texture(screenTexture, TexCoords).rgb;
    
}  

