#include"Particle.h"

#include <math.h>

struct ParticleInfo{
	vec3 position;
	vec3 velocity;
	float rotation;
	float scale;
};

float randomFloat() // ����0-1�������
{
	return (rand() % 1000 / 1000.0); //���ؾ���Ϊ��λС�� 0-1����
}

ParticleInfo getFireworksParticleInfo(vec3 center, float speed)
{
	ParticleInfo fireworksParticleInfo;

	float dirX = randomFloat() * 0.5 - 0.25f; // ת���ɣ�-1��1��
	//float dirZ = randomFloat() * 0.5 - 0.25f; 
	float dirZ = randomFloat() * 0.5 - 0.25f;
	vec3 velocity(dirX, 1.0f, dirZ); // ��ʼY�����ٶȶ�Ϊ1 ��ֱ����
	velocity = glm::normalize(velocity); // ������ ��Ϊ�˽��ٶ�ֵ��С��Ϊ1
	velocity *= speed; // ��������ٶ� ���������������֮�� Ҳ�����ٶȵĴ�С��Ϊspeed��

	fireworksParticleInfo.position = center;
	fireworksParticleInfo.velocity = velocity;
	fireworksParticleInfo.rotation = 0.0f;
	fireworksParticleInfo.scale = 0.6f;
	return fireworksParticleInfo;
}

ParticleInfo getFireParticleInfo(vec3 center, float speed)
{
	ParticleInfo fireParticleInfo;

	float dirX = randomFloat() * 0.5 - 0.25f; 
	float dirZ = randomFloat() * 0.5 - 0.25f; 
	vec3 velocity(dirX, 1.0f, dirZ); 
	velocity = glm::normalize(velocity); 
	velocity *= speed; 

	fireParticleInfo.position = center;
	fireParticleInfo.velocity = velocity;
	fireParticleInfo.rotation = 0.0f;
	fireParticleInfo.scale = 3.0f; //�Ƕ������ţ�
	return fireParticleInfo;
}

ParticleInfo getSmokeParticleInfo(vec3 center, float speed)
{
	ParticleInfo smokeParticleInfo;

	float posX = (randomFloat() - float(0.5f)) / 1.5;
	float posZ = (randomFloat() - (float)0.5f) / 1.5;
	vec3 posOffset(posX, 0.0f, posZ);

	float dirX = randomFloat() * 0.01 - 0.005f;
	float dirZ = randomFloat() * 0.01 - 0.005f;
	vec3 velocity(dirX, 0.05, dirZ); // ��ʼY�����ٶȶ�Ϊ1 ��ֱ����
	velocity = glm::normalize(velocity); // ������ ��Ϊ�˽��ٶ�ֵ��С��Ϊ1
	velocity *= speed; // ��������ٶ� ���������������֮�� Ҳ�����ٶȵĴ�С��Ϊspeed��

	smokeParticleInfo.position = center + posOffset;
	smokeParticleInfo.velocity = velocity;
	smokeParticleInfo.rotation = 0.0f;
	smokeParticleInfo.scale = 1.0f;
	return smokeParticleInfo;
}

ParticleInfo getSnowParticleInfo(vec3 center, float speed) //������
{
	ParticleInfo snowParticleInfo;

	float posX = randomFloat() * 5 - 10.0f; //[-5,5]
	float posZ = randomFloat() * 5 - 10.0f;
	vec3 posOffset(posX, 0.0f, posZ);

	float dirX = randomFloat() * 2 - 1.0f; // ת���ɣ�-1��1��
	float dirZ = randomFloat() * 2 - 1.0f;
	vec3 velocity(dirX, 0.0f, dirZ); // ��ʼY�����ٶȶ�Ϊ1 ��ֱ����
	velocity = glm::normalize(velocity); // ������ ��Ϊ�˽��ٶ�ֵ��С��Ϊ1
	velocity *= speed; // ��������ٶ� ���������������֮�� Ҳ�����ٶȵĴ�С��Ϊspeed��

	snowParticleInfo.position = center + posOffset;
	snowParticleInfo.velocity = velocity;
	snowParticleInfo.rotation = 0.0f;
	snowParticleInfo.scale = randomFloat();

	return snowParticleInfo;
}

ParticleInfo getStarParticleInfo(vec3 center, float speed)
{
	ParticleInfo starParticleInfo;

	float posX = randomFloat() * 1 - 2.0f; //[-5,5]
	float posZ = randomFloat() * 5 - 10.0f;
	vec3 posOffset(posX, 0.0f, posZ);

	float dirX = randomFloat() * 0.5 - 0.25f; // ת���ɣ�-1��1��
	float dirZ = randomFloat() ; // ת���ɣ�-1��0��
	vec3 velocity(dirX, 0.0f, dirZ); // ��ʼY�����ٶȶ�Ϊ1 ��ֱ����
	velocity = glm::normalize(velocity); // ������ ��Ϊ�˽��ٶ�ֵ��С��Ϊ1
	velocity *= speed; // ��������ٶ� ���������������֮�� Ҳ�����ٶȵĴ�С��Ϊspeed��

	starParticleInfo.position = center + posOffset;
	starParticleInfo.velocity = velocity;
	starParticleInfo.rotation = 0.0f;
	starParticleInfo.scale = (randomFloat() + 5.0) / 2.2;

	return starParticleInfo;
}

ParticleInfo getCosmicParticleInfo(vec3 center, float speed)
{
	ParticleInfo cosmicParticleInfo;

	float dirX = randomFloat() * 2 - 1.0f;
	float dirZ = randomFloat() * 2 - 1.0f;
	//vec3 velocity(-2.0f, 0.0f, dirZ);
	vec3 velocity(dirX, 0.0f, 2.0);
	velocity = glm::normalize(velocity);
	velocity *= speed;

	cosmicParticleInfo.position = center;
	cosmicParticleInfo.velocity = velocity;
	cosmicParticleInfo.rotation = 0.0f; //Ҫ������ת��
	cosmicParticleInfo.scale = 0.7f; //�Ƕ������ţ�

	return cosmicParticleInfo;
}

ParticleInfo getDefaultParticleInfo(vec3 center, float speed)
{
	ParticleInfo defaultParticleInfo;
	float dirX = randomFloat() * 2 - 1.0f; // ת���ɣ�-1��1��
	float dirZ = randomFloat() * 2 - 1.0f;
	vec3 velocity(dirX, 0.0f, dirZ); // ��ʼY�����ٶȶ�Ϊ1 ��ֱ����
	velocity = glm::normalize(velocity); // ������ ��Ϊ�˽��ٶ�ֵ��С��Ϊ1
	velocity *= speed; // ��������ٶ� ���������������֮�� Ҳ�����ٶȵĴ�С��Ϊspeed��

	defaultParticleInfo.position = center;
	defaultParticleInfo.velocity = velocity;
	defaultParticleInfo.rotation = 0.0f;
	defaultParticleInfo.scale = 1.0f;
	return defaultParticleInfo;
}