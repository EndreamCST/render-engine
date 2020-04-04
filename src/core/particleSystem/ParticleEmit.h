#include"Particle.h"

#include <math.h>

struct ParticleInfo{
	vec3 position;
	vec3 velocity;
	float rotation;
	float scale;
};

float randomFloat() // 返回0-1的随机数
{
	return (rand() % 1000 / 1000.0); //返回精度为三位小数 0-1的数
}

ParticleInfo getFireworksParticleInfo(vec3 center, float speed)
{
	ParticleInfo fireworksParticleInfo;

	float dirX = randomFloat() * 0.5 - 0.25f; // 转换成（-1，1）
	//float dirZ = randomFloat() * 0.5 - 0.25f; 
	float dirZ = randomFloat() * 0.5 - 0.25f;
	vec3 velocity(dirX, 1.0f, dirZ); // 初始Y方向速度都为1 竖直向上
	velocity = glm::normalize(velocity); // 正交化 是为了将速度值大小设为1
	velocity *= speed; // 方向乘上速度 这样三个方向分量之和 也就是速度的大小就为speed了

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
	fireParticleInfo.scale = 3.0f; //是多少来着？
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
	vec3 velocity(dirX, 0.05, dirZ); // 初始Y方向速度都为1 竖直向上
	velocity = glm::normalize(velocity); // 正交化 是为了将速度值大小设为1
	velocity *= speed; // 方向乘上速度 这样三个方向分量之和 也就是速度的大小就为speed了

	smokeParticleInfo.position = center + posOffset;
	smokeParticleInfo.velocity = velocity;
	smokeParticleInfo.rotation = 0.0f;
	smokeParticleInfo.scale = 1.0f;
	return smokeParticleInfo;
}

ParticleInfo getSnowParticleInfo(vec3 center, float speed) //待调整
{
	ParticleInfo snowParticleInfo;

	float posX = randomFloat() * 5 - 10.0f; //[-5,5]
	float posZ = randomFloat() * 5 - 10.0f;
	vec3 posOffset(posX, 0.0f, posZ);

	float dirX = randomFloat() * 2 - 1.0f; // 转换成（-1，1）
	float dirZ = randomFloat() * 2 - 1.0f;
	vec3 velocity(dirX, 0.0f, dirZ); // 初始Y方向速度都为1 竖直向上
	velocity = glm::normalize(velocity); // 正交化 是为了将速度值大小设为1
	velocity *= speed; // 方向乘上速度 这样三个方向分量之和 也就是速度的大小就为speed了

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

	float dirX = randomFloat() * 0.5 - 0.25f; // 转换成（-1，1）
	float dirZ = randomFloat() ; // 转换成（-1，0）
	vec3 velocity(dirX, 0.0f, dirZ); // 初始Y方向速度都为1 竖直向上
	velocity = glm::normalize(velocity); // 正交化 是为了将速度值大小设为1
	velocity *= speed; // 方向乘上速度 这样三个方向分量之和 也就是速度的大小就为speed了

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
	cosmicParticleInfo.rotation = 0.0f; //要试试旋转吗
	cosmicParticleInfo.scale = 0.7f; //是多少来着？

	return cosmicParticleInfo;
}

ParticleInfo getDefaultParticleInfo(vec3 center, float speed)
{
	ParticleInfo defaultParticleInfo;
	float dirX = randomFloat() * 2 - 1.0f; // 转换成（-1，1）
	float dirZ = randomFloat() * 2 - 1.0f;
	vec3 velocity(dirX, 0.0f, dirZ); // 初始Y方向速度都为1 竖直向上
	velocity = glm::normalize(velocity); // 正交化 是为了将速度值大小设为1
	velocity *= speed; // 方向乘上速度 这样三个方向分量之和 也就是速度的大小就为speed了

	defaultParticleInfo.position = center;
	defaultParticleInfo.velocity = velocity;
	defaultParticleInfo.rotation = 0.0f;
	defaultParticleInfo.scale = 1.0f;
	return defaultParticleInfo;
}