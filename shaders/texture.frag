#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = vec4(TexCoord.xy, 0, 1.0);
    FragColor = texture(ourTexture, TexCoord);
}
