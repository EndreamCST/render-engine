#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in mat4 modelView;
layout (location = 5) in vec4 texOffsets;
layout (location = 6) in float blendFactor;

uniform mat4 projection;
uniform float numberOfRows;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

void main()
{
    vec2 textureCoords = position + vec2(0.5, 0.5); //将坐标映射到0-1 因为这是纹理坐标的区间
    //如果不在这里对y轴进行翻转的话 需要在主函数里加载texure的时候加上一句 stbi_set_flip_vertically_on_load(true)
    textureCoords.y = 1.0 -textureCoords.y;
    textureCoords /= numberOfRows ; //这里将纹理坐标的x,y值都除以行数的意思是将纹理坐标缩放到一个单元格 在这基础上才能正确计算偏移值 获取纹理的映射坐标
    textureCoords1 = textureCoords + texOffsets.xy; //对于一个quad映射到0-1并且缩放为对应纹理的一个单元格 的坐标(想一想一张代表纹理的正方形纸上左上角的一个小单元格) 然后加上相应的纹理偏移 就可以获取对应阶段的纹理
    textureCoords2 = textureCoords + texOffsets.zw;
    blend = blendFactor;
    gl_Position = projection * modelView * vec4(position, 0.0f, 1.0f);
}
