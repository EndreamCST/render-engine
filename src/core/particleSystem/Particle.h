#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "ParticleTexture.h"


typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

static const float GRAVITY = 1.0; //����ֵ ֱ�Ӵ��ⲿ��ȡ ��Main.cpp�ж��� ���������ļ��ж���
//extern float deltaTime; //ÿһ֡��ʱ�� ͨ����Ⱦ��ѭ����֡��ʱ������ ��Main.cpp�� ��ȫ�ֱ���

class Particle{
private:
	vec3 position;
	vec3 velocity;
	float gravityEffect;  //��ֵ�����ϵ�
	float lifeLength;
	float rotation; //��ת�Ƕ� һ������z����ת�ĽǶ�
	float scale;
	float distance; //������ľ��� ������Ⱦ˳��
	float elapsedTime = 0; //�Ѿ�����ʱ��

	bool alive = false; //�����˼��

	ParticleTexture texture; //ÿһ�������ж�Ӧ������
	vec2 texOffset1; //���ӵ�ǰ��ƫ��ֵ �����׶� ����һ����λ����ռ����ǰ����������
	vec2 texOffset2;
	float blend;

	void updateTextureCoordInfo();
	void setTextureOffset(vec2 &offset, int index);
	
public:
	Particle(vec3 position, vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale, ParticleTexture texture)
		: position(position), velocity(velocity), gravityEffect(gravityEffect), 
		lifeLength(lifeLength), rotation(rotation), scale(scale), texture(texture){}

	//�������� ����������
	void setActive(vec3 position, vec3 velocity, float rotation, float scale); //�����µ����� ֻ��Ҫ�ı���Щ���� �����������������ӡ��������ڳ��Ⱥ�����Ȳ���ı�
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
		velocity.y += GRAVITY * gravityEffect * deltaTime; //����Y�����ٶȵı仯 v = v + a*t  �൱��ǰ����Ǽ��ٶ�deltaTime����ʱ����
		position += velocity * deltaTime; //�µ�λ�� s += v*t
		elapsedTime += deltaTime;
		updateTextureCoordInfo();
		if (!texture.getIsAdditve()) //�������additive��ϲ���Ҫ���㲢���¾���
		{
			distance = glm::length(camera.Position() - position);
		}
		return elapsedTime < lifeLength;
	}
};

void Particle::updateTextureCoordInfo()
{
	float lifeFactor = elapsedTime / lifeLength;
	int stageCount = texture.getNumberOfRows() * texture.getNumberOfRows(); //�׶ε��ܸ�����
	float atlasProgression = lifeFactor * stageCount; //Ŀǰ�Ľ׶� ��һ��float�� Ҫ����Ϊ�������� �Ҽ���ƫ��ֵ����������
	int index1 = (int)floor(atlasProgression); //����ȡ��
	int index2 = index1 < stageCount - 1 ? index1 + 1 : index1; //��ֹ��� ��25������ �߽��±�Ϊ24 ����>=24 ������+1
	blend = atlasProgression - index1; //ƫ��ֵ ����˵��� ��5.6-5=0.6
	setTextureOffset(texOffset1, index1);
	setTextureOffset(texOffset2, index2);
}

void Particle::setTextureOffset(vec2 &offset, int index)
{
	int rows = texture.getNumberOfRows();
	int column = index % rows;
	int row = index / rows;
	offset.x = (float)column / rows; //ӳ�䵽0-1
	offset.y = (float)row / rows;
}

void Particle::setActive(vec3 position, vec3 velocity,  float rotation, float scale)
{
	this->alive = true; //����
	this->position = position;
	this->velocity = velocity;
	this->rotation = rotation;
	this->scale = scale;
	this->elapsedTime = 0; //�����ʱ�����ô��ʱ��
}

void Particle::setNotActive()
{
	this->alive = false;
}

#endif

