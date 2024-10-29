#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexType;
in vec4 v_Color;

in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture_Metal_Light;
uniform sampler2D u_Texture_Metal_Dark;
uniform sampler2D u_Texture_Wood_Light;
uniform sampler2D u_Texture_Wood_Dark;

uniform int lighting;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightPos = vec3(100.0, 100.0, 100.0);

    vec4 texColor;

    if(v_TexType < 0.5)
    {
        texColor = texture(u_Texture_Metal_Light, v_TexCoord);
    }
    else if(v_TexType < 1.5)
    {
        texColor = texture(u_Texture_Metal_Dark, v_TexCoord);
    }
    else if(v_TexType < 2.5)
    {
        texColor = texture(u_Texture_Wood_Light, v_TexCoord);
    }
    else if(v_TexType < 3.5)
    {
        texColor = texture(u_Texture_Wood_Dark, v_TexCoord);
    }
    
    color = v_Color;

    if(lighting == 1)
    {
        // Ambient stuff
        float ambientStrength = 0.25;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse stuff
        vec3 norm = normalize(v_Normal);
        vec3 lightDir = normalize(lightPos - v_FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // result lighting
        vec3 result = ambient + diffuse;

        color = vec4(result,1.0) * texColor;
    }
    else
    {
        color = texColor;
    }
    
};