//生成VAO VBO并绑定 传递顶点数据vertices 存储VAO的ID 
#ifndef QUAD_H
#define QUAD_H

#include <glad/glad.h>

class Quad{
private:
	static float vertices[8];//硬编码
	unsigned int vertexCount;
	unsigned int quadVAO;
	unsigned int quadVBO;
	void loadToVAO(); //加载VAO
public:
	Quad()
	{	
		//由于vertices是硬编码 所以今后要改只用改这个数组的具体数据就行了
		vertexCount = sizeof(vertices)/sizeof(float); 
		loadToVAO();
	}
	unsigned int getVertexCount(){ return vertexCount;}
	unsigned int getQuadVAO(){ return quadVAO; }
};

void Quad::loadToVAO()
{
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindVertexArray(0); //解绑vao
}

float Quad::vertices[8] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };

#endif // !QUAD_H
