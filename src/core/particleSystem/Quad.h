//����VAO VBO���� ���ݶ�������vertices �洢VAO��ID 
#ifndef QUAD_H
#define QUAD_H

#include <glad/glad.h>

class Quad{
private:
	static float vertices[8];//Ӳ����
	unsigned int vertexCount;
	unsigned int quadVAO;
	unsigned int quadVBO;
	void loadToVAO(); //����VAO
public:
	Quad()
	{	
		//����vertices��Ӳ���� ���Խ��Ҫ��ֻ�ø��������ľ������ݾ�����
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

	glBindVertexArray(0); //���vao
}

float Quad::vertices[8] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };

#endif // !QUAD_H
