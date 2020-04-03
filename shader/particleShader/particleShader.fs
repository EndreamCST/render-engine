#version 330 core
out vec4 FragColor;

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

uniform sampler2D particleTexture; //纹理单元 绑定纹理会激活这个纹理单元 并设置为绑定的这个纹理

//uniform vec3 color;

void main()
{
    vec4 color1 = texture(particleTexture, textureCoords1);
    vec4 color2 = texture(particleTexture, textureCoords2);
    vec4 texColor = mix(color1, color2, blend);
    FragColor = texColor;
}