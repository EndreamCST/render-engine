#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "myShader.h"
#include "Particle.h"
#include "Quad.h"

#include <list>
using namespace std;

typedef glm::mat4 mat4;

//声明为static 只能在本文件中使用
static const unsigned int SCR_WIDTH = 1280;
static const unsigned int SCR_HEIGHT = 720;
static const unsigned int MAX_INSTANCES = 100000; //每次场景绘制的最大粒子数 十万个 够你用了吧
static const unsigned int INSTANCE_DATA_LENGTH = 21; //每个实例的属性数据长度
static float *vboData = new float[MAX_INSTANCES * INSTANCE_DATA_LENGTH]; //这里使用静态数组会报错

class ParticleRenderer{
private:
	Quad *quad;
	myShader *particleShader;
	unsigned int VBO;
	unsigned int pointer; //用于指向vboData给每个位置赋值

	unsigned int createEmptyVBO(int floatCount);
	void setUpAttributes(unsigned int VAO, unsigned int VBO);

	void prepare(); //激活着色器中的顶点属性
	void bindTexture(ParticleTexture texture); //激活着色器中的纹理单元 设置1个uniform
	void updateModelViewMatrix(vec3 position, float rotation, float scale, mat4 view, float vboData[]);
	void storeMatrixData(mat4 matrix, float data[]);
	void updateTexCoordsInfo(Particle particle, float data[]);
	void updateVBOdata(unsigned int VBO, float data[]);
	void finishRendering();

public:
	ParticleRenderer();
	void setProjection(mat4 projection);
	void render(list<Particle> particles, Camera camera);
};

//构造函数 创建新的quad和shader 且每一个渲染器有独自的投影矩阵 
ParticleRenderer::ParticleRenderer()
{
	quad = new Quad();
	particleShader = new myShader("shader/particleShader/particleShader.vs", "shader/particleShader/particleShader.fs");
	VBO = createEmptyVBO(MAX_INSTANCES * INSTANCE_DATA_LENGTH); //实例的大VBO 包含了粒子数 * 每个粒子的属性总数 * 数据类型的长度（字节数）
	setUpAttributes(quad->getQuadVAO(), VBO);
	//使用默认的投影矩阵
	mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	particleShader->use();
	particleShader->setMat4("projection", projection);
	particleShader->stop();
}

//从外部设置投影矩阵
void ParticleRenderer::setProjection(mat4 projection)
{
	particleShader->setMat4("projection", projection); 
}

//更新粒子的模型视图矩阵 以便渲染 并经过处理使得粒子朝向正确
void ParticleRenderer::updateModelViewMatrix(vec3 position, float rotation, float scale, mat4 view, float vboData[])
{
	mat4 model(1.0f);
	model = glm::translate(model, position);

	//以下部分为尝试 修正最终的modelview矩阵的旋转部分 即左上角3x3的矩阵化为单位矩阵
	float *p = glm::value_ptr(model);
	float *q = glm::value_ptr(view);
	*p = *q;
	*(p + 1) = *(q + 4);
	*(p + 2) = *(q + 8);
	*(p + 4) = *(q + 1);
	*(p + 5) = *(q + 5);
	*(p + 6) = *(q + 9);
	*(p + 8) = *(q + 2);
	*(p + 9) = *(q + 6);
	*(p + 10) = *(q + 10);

	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, vec3(scale));

	mat4 modelView = view * model;  //不是 是应该先乘上再旋转？
	storeMatrixData(modelView, vboData); //存入VBO数据
}

//激活着色器 激活顶点属性 设置混合、深度缓冲
void ParticleRenderer::prepare()
{
	particleShader->use();
	glBindVertexArray(quad->getQuadVAO());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnable(GL_BLEND); 
	glDepthMask(false); 
}

//和prepare相反
void ParticleRenderer::finishRendering()
{
	glDepthMask(true);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glBindVertexArray(0);
	particleShader->stop();
}

void ParticleRenderer::render(list<Particle> particles, Camera camera)
{
	mat4 view = camera.GetViewMatrix();
	ParticleTexture texture = particles.front().getTexture();

	prepare();
	bindTexture(texture);
	
	//float *vboData = new float[particles.size() * INSTANCE_DATA_LENGTH]; //动态分配数组 会报错
	pointer = 0;
	int particlesToDraw = 0; //要绘制的粒子数
	for (auto &particle : particles)
	{
		if (particle.isAlive())
		{
			updateModelViewMatrix(particle.getPosition(), particle.getRotation(), particle.getScale(), view, vboData);
			updateTexCoordsInfo(particle, vboData);
			particlesToDraw++;
		}
	}
	updateVBOdata(VBO, vboData); //将存储好的vbo数据传递给VBO
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), particlesToDraw);
	
	finishRendering();
}

void ParticleRenderer::bindTexture(ParticleTexture texture)
{
	if (texture.getIsAdditve()) //是否使用addtive混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0); //激活纹理单元 绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
	particleShader->setFloat("numberOfRows", texture.getNumberOfRows());  //设置着色器的uniform
}

//构造渲染器的时候创建一个空的VBO 因为每一帧都要更新一批粒子的属性值 所以不能一开始就传递数据 需要每次更新数据给VBO 所以开始传递数据为NULL 后续使用subData来传递数据
unsigned int ParticleRenderer::createEmptyVBO(int floatCount) //最大的浮点数数量
{
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), NULL, GL_STREAM_DRAW); //这里用的是stream_draw 意思应该是每次绘制的时候数据都会改变 leanOpenGL教程里是static_draw应该是数据传递一次 被使用很多次 不会被改变
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return VBO;
}

//建立VAO和VBO的连接 告诉VAO 顶点上具有的这些属性 按照什么样的步长、节奏去从VBO身上获取顶点数据 然后如何更新顶点属性
void ParticleRenderer::setUpAttributes(unsigned int VAO, unsigned int VBO)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(0));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(8 * sizeof(float)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(12 * sizeof(float)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(16 * sizeof(float)));
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, INSTANCE_DATA_LENGTH * sizeof(float), (void*)(20 * sizeof(float)));
	//属性x 每渲染一个新的实例更新一次数据（即向后偏移迭代一个步长）
	glVertexAttribDivisor(1, 1); 
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleRenderer::updateVBOdata(unsigned int VBO, float data[])
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_INSTANCES * INSTANCE_DATA_LENGTH * sizeof(float), data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleRenderer::storeMatrixData(mat4 matrix, float data[])
{
	float *pMatrix = glm::value_ptr(matrix);
	for (int i = 0; i < 16; i++)
	{
		*(data + pointer++) = *pMatrix++;  //往data数组里填充数据 因为mat4矩阵中是按列来排序的 所以刚好符合
	}
}

void ParticleRenderer::updateTexCoordsInfo(Particle particle, float data[])
{
	*(data + pointer++) = particle.getTexOffset1().x;
	*(data + pointer++) = particle.getTexOffset1().y;
	*(data + pointer++) = particle.getTexOffset2().x;
	*(data + pointer++) = particle.getTexOffset2().y;
	*(data + pointer++) = particle.getBlend();
}

#endif
