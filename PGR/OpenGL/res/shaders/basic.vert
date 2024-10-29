#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texType;
layout(location = 4) in vec3 normal;

out vec2 v_TexCoord;
out float v_TexType;
out vec4 v_Color;

out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 center;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * center * position;
   v_TexCoord = texCoord;
   v_TexType = texType;
   v_Color = color;

   v_Normal = vec3(model * center * vec4(normal,0.0));
   v_FragPos = vec3(model * center * position);
};