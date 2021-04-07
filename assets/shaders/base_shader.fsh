#version 330 core

// out vec4 gl_FragColor;

in vec4 vertexColor;
in vec3 vertexNormal;
// in vec4 gl_FragCoord;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;

float near = 0.1; 
float far  = 100.0; 

vec3 lightDir = vec3(-1, -1, -1);

void main() {
  gl_FragColor = texture(texture_diffuse1, texCoord);
  
  
  float shading = 1-dot(vertexNormal, lightDir);

  shading = min(shading, 1.0);
  shading = max(shading, 0.5);

  gl_FragColor.rgb *= shading;
  
  // gl_FragColor.rg = vertexColor;
  // gl_FragColor.a = 1.0;
  // float depth = gl_FragCoord.w;
  // float ndc = depth * 2.0 - 1.0; 
  // float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));	

  // gl_FragDepth.rgb = vec3(linearDepth);

  // color *= vec3(linearDepth);
  // color *= vec3(texture(texture_diffuse2, texCoord));
}