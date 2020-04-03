#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "ParticleTexture.h"


typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

static const float GRAVITY = 1.0; //重力值 直接从外部获取 在Main.cpp中定义 或者其他文件中定义
//extern float deltaTime; //每一帧的时间 通过渲染主循环两帧的时间差计算 在Main.cpp中 是全局变量

class Particle{
private:
	vec3 position;
	vec3 velocity;
	float gravityEffect;  //正值是向上的
	float lifeLength;
	float rotation; //旋转角度 一般是绕z轴旋转的角度
	float scale;
	float distance; //和相机的距离 用于渲染顺序
	float elapsedTime = 0; //已经存活的时间

	bool alive = false; //对象池思想

	ParticleTexture texture; //每一个粒子有对应的纹理
	vec2 texOffset1; //粒子当前的偏移值 两个阶段 类似一个单位格子占据了前后两个格子
	vec2 texOffset2;
	float blend;

	void updateTextureCoordInfo();
	void setTextureOffset(vec2 &offset, int index);
	
public:
	Particle(vec3 position, vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale, ParticleTexture texture)
		: position(position), velocity(velocity), gravityEffect(gravityEffect), 
		lifeLength(lifeLength), rotation(rotation), scale(scale), texture(texture){}

	//激活粒子 反激活粒子
	void setActive(vec3 position, vec3 velocity, float rotation, float scale); //激活新的粒子 只需要改变这些属性 其他的例如重力因子、生命周期长度和纹理等不需改变
	void setNotActive();

	vec3 getPosition() const
	{
		return position;
	}
	float getRotation() const
	{
		return rotation;
	}
	float getScale() const
	{
		return scale;
	}
	ParticleTexture getTexture() const
	{
		return texture;
	}
	vec2 getTexOffset1() const
	{
		return texOffset1;
	}
	vec2 getTexOffset2() const
	{
		return texOffset2;
	}
	float getBlend() const
	{
		return blend;
	}
	float getDistance() const
	{
		return distance;
	}
	bool isAlive() const
	{
		return alive;
	}

	bool update(Camera camera)
	{
		float deltaTime = Engine::GetInstance().GetRenderer().GetDeltaTime();
		velocity.y += GRAVITY * gravityEffect * deltaTime; //粒子Y方向速度的变化 v = v + a*t  相当于前面就是加速度deltaTime就是时间间隔
		position += velocity * deltaTime; //新的位置 s += v*t
		elapsedTime += deltaTime;
		updateTextureCoordInfo();
		if (!texture.getIsAdditve()) //如果不是additive混合才需要计算并更新距离
		{
			distance = glm::length(camera.Position() - position);
		}
		return elapsedTime < lifeLength;
	}
};

void Particle::updateTextureCoordInfo()
{
	float lifeFactor = elapsedTime / lifeLength;
	int stageCount = texture.getNumberOfRows() * texture.getNumberOfRows(); //阶段的总格子数
	float atlasProgression = lifeFactor * stageCount; //目前的阶段 是一个float数 要划分为两个格子 且计算偏移值（浮点数）
	int index1 = (int)floor(atlasProgression); //向下取整
	int index2 = index1 < stageCount - 1 ? index1 + 1 : index1; //防止溢出 如25个格子 边界下标为24 不能>=24 否则不能+1
	blend = atlasProgression - index1; //偏移值 或者说混合 如5.6-5=0.6
	setTextureOffset(texOffset1, index1);
	setTextureOffset(texOffset2, index2);
}

void Particle::setTextureOffset(vec2 &offset, int index)
{
	int rows = texture.getNumberOfRows();
	int column = index % rows;
	int row = index / rows;
	offset.x = (float)column / rows; //映射到0-1
	offset.y = (float)row / rows;
}

void Particle::setActive(vec3 position, vec3 velocity,  float rotation, float scale)
{
	this->alive = true; //活了
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->scale = scale;
	this->elapsedTime = 0; //激活的时候重置存活时间
}

void Particle::setNotActive()
{
	this->alive = false;
}

#endif

