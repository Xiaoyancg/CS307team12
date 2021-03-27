// fragment shader
#version 450 core

uniform sampler2D texture1;
out vec4 FragColor;
in vec2 votex;
void main()
{
  //FragColor = vec4(1.0,1.0,1.0,1.0);
  FragColor = texture(texture1, votex);
}