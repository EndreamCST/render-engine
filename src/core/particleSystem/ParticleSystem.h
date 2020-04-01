#ifndef PARTILCE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Renderer.hpp"
#include "Engine.hpp"

#include "ParticleMaster.h"
#include "ParticleRenderer.h"
#include "ParticleEmit.h"

class ParticleSystem{
private:
	float pps; // particles per second
	float speed;
	float gravityComplient; // ��������
	float lifeLength;

	ParticleTexture texture;

	ParticleMaster master; //ÿ������ϵͳ��һ���Լ���master
	ParticleRenderer renderer; //ÿ������ϵͳ��һ���Լ���renderer

	void initParticleList();
	void emitFireworksParticles(int count, vec3 systemCenter);
	void emitFireParticles(int count, vec3 systemCenter);
	void emitSmokeParticles(int count, vec3 systemCenter);
	void emitSnowParticles(int count, vec3 systemCenter);
	void emitStarParticles(int count, vec3 systemCenter);
	void emitCosmicParticles(int count, vec3 systemCenter);
	void emitDefaultParticles(int count, vec3 systemCenter);

public:
	//master��renderer�Զ�����Ĭ�ϵĹ��캯��
	ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength, ParticleTexture texture) :
		pps(pps), speed(speed), gravityComplient(gravityComplient), lifeLength(lifeLength), texture(texture){}
	void generateParticles(vec3 systemCenter);
	void setProjection(mat4 projection);
	void render(Camera camera);
};

void ParticleSystem::generateParticles(vec3 systemCenter)
{
	//��������б�Ϊ�� �ͳ�ʼ������б� �����п��ܶ�����Ӷ��Ž�ȥ ��Ĭ������Ϊ������״̬
	if (master.particles.size() == 0)
	{
		initParticleList();
	}
	float deltaTime = Engine::GetInstance().GetRenderer().GetDeltaTime();
	float particlesToCreate = pps * deltaTime; //ÿ֡��Ⱦ(���ɣ���������
	int count = (int)floor(particlesToCreate); // ����ȡ��
	float partialParticle = particlesToCreate - count; //С������
	if (randomFloat() < partialParticle) count++; //����ÿ֡��Ⱦ��������

	Particle_Type type = texture.getParticleType();
	switch (type)
	{
	case FIREWORKS:
		emitFireworksParticles(count, systemCenter);
		break;
	case FIRE:
		emitFireParticles(count, systemCenter);
		break;
	case SMOKE:
		emitSmokeParticles(count, systemCenter);
		break;
	case SNOW:
		emitSnowParticles(count, systemCenter);
		break;
	case STAR:
		emitStarParticles(count, systemCenter);
		break;
	case COSTMIC:
		emitCosmicParticles(count, systemCenter);
		break;
	default:
		emitDefaultParticles(count, systemCenter);
		break;
	}
}

void ParticleSystem::setProjection(mat4 projection)
{
	renderer.setProjection(projection);
}

void ParticleSystem::render(Camera camera)
{
	if (master.particles.size() == 0) return; //���Ŀǰ�����б�Ϊ�� ���ܸ��²���Ⱦ ����������ӵ�pps<=2 һ��ʼ����list�����û������ ��ʱ��Ⱦ�ͻ�ը��
	master.update(camera); //�ȸ����б��е����� ����Ⱦ
	renderer.render(master.particles, camera);
}

void ParticleSystem::initParticleList()
{
	int count = pps * lifeLength; //����ͬ������������� ��Ϊÿ����Ⱦpps������ �������ܳ���LifeTime����
	for (int i = 0; i < count; i++)
	{
		//��ʼ��Ҫ�趨�������ӡ��������ں����� ����ں���������ӵ�ʱ���ǲ����
		Particle newParticle(vec3(0.0), vec3(0.0), gravityComplient, lifeLength, 0, 0, texture);
		master.addParticle(newParticle);
	}
}

void ParticleSystem::emitFireworksParticles(int count, vec3 systemCenter)
{
	ParticleInfo particleInfo;
	for (auto& particle:master.particles) //���������б� �Ѳ���������Ӽ��� ��������Ӧ������ֵ
	{
		if (count == 0) break;
		if (!particle.isAlive()) //�����������ǲ�����״̬ ����Ӧ���ӵķ��䷽���л����������ֵ �������������
		{
			particleInfo = getFireworksParticleInfo(systemCenter, speed); //��ͬ�����в�ͬ�ķ��䷽ʽ ������������ӵķ�ʽ
			particle.setActive(particleInfo.position, particleInfo.velocity, particleInfo.rotation, particleInfo.scale);
			count--;
		}
	}
}
void ParticleSystem::emitFireParticles(int count, vec3 systemCenter)
{
	ParticleInfo particleInfo;
	for (auto& particle : master.particles) //���������б� �Ѳ���������Ӽ��� ��������Ӧ������ֵ
	{
		if (count == 0) break;
		if (!particle.isAlive()) //�����������ǲ�����״̬ ����Ӧ���ӵķ��䷽���л����������ֵ �������������
		{
			particleInfo = getFireParticleInfo(systemCenter, speed); //��ͬ�����в�ͬ�ķ��䷽ʽ ������������ӵķ�ʽ
			particle.setActive(particleInfo.position, particleInfo.velocity, particleInfo.rotation, particleInfo.scale);
			count--;
		}
	}
}
void ParticleSystem::emitSmokeParticles(int count, vec3 systemCenter)
{
	ParticleInfo particleInfo;
	for (auto& particle : master.particles) //���������б� �Ѳ���������Ӽ��� ��������Ӧ������ֵ
	{
		if (count == 0) break;
		if (!particle.isAlive()) //�����������ǲ�����״̬ ����Ӧ���ӵķ��䷽���л����������ֵ �������������
		{
			particleInfo = getSmokeParticleInfo(systemCenter, speed); //��ͬ�����в�ͬ�ķ��䷽ʽ ������������ӵķ�ʽ
			particle.setActive(particleInfo.position, particleInfo.velocity, particleInfo.rotation, particleInfo.scale);
			count--;
		}
	}
}
void ParticleSystem::emitSnowParticles(int count, vec3 systemCenter)
{
	ParticleInfo particleInfo;
	for (auto& particle : master.particles) //���������б� �Ѳ���������Ӽ��� ��������Ӧ������ֵ
	{
		if (count == 0) break;
		if (!particle.isAlive()) //�����������ǲ�����״̬ ����Ӧ���ӵķ��䷽���л����������ֵ �������������
		{
			particleInfo = getSnowParticleInfo(systemCenter, speed); //��ͬ�����в�ͬ�ķ��䷽ʽ ������������ӵķ�ʽ
			particle.setActive(particleInfo.position, particleInfo.velocity, particleInfo.rotation, particleInfo.scale);
			count--;
		}
	}
}
void ParticleSystem::emitStarParticles(int count, vec3 systemCenter)
{
	ParticleInfo particleInfo;
	for (auto& particle : master.particles) //���������б� �Ѳ���������Ӽ��� ��������Ӧ������ֵ
	{
		if (count == 0) break;
		if (!particle.isAlive()) //�����������ǲ�����״̬ ����Ӧ���ӵķ��䷽���л����������ֵ �������������
		{
			particleInfo = getStarParticleInfo(systemCenter, speed); //��ͬ�����в�ͬ�ķ��䷽ʽ ������������ӵķ�ʽ
			particle.setActive(particleInfo.position, particleInfo.velocity, particleInfo.rotation, particleInfo.scale);
			count--;
		}
	}
}
void ParticleSystem::emitCosmicParticles(int count, vec3 systemCenter)
{
	ParticleInfo particleInfo;
	for (auto& particle : master.particles) //���������б� �Ѳ���������Ӽ��� ��������Ӧ������ֵ
	{
		if (count == 0) break;
		if (!particle.isAlive()) //�����������ǲ�����״̬ ����Ӧ���ӵķ��䷽���л����������ֵ �������������
		{
			particleInfo = getCosmicParticleInfo(systemCenter, speed); //��ͬ�����в�ͬ�ķ��䷽ʽ ������������ӵķ�ʽ
			particle.setActive(particleInfo.position, particleInfo.velocity, particleInfo.rotation, particleInfo.scale);
			count--;
		}
	}
}
void ParticleSystem::emitDefaultParticles(int count, vec3 systemCenter)
{
	ParticleInfo particleInfo;
	for (auto& particle : master.particles) //���������б� �Ѳ���������Ӽ��� ��������Ӧ������ֵ
	{
		if (count == 0) break;
		if (!particle.isAlive()) //�����������ǲ�����״̬ ����Ӧ���ӵķ��䷽���л����������ֵ �������������
		{
			particleInfo = getDefaultParticleInfo(systemCenter, speed); //��ͬ�����в�ͬ�ķ��䷽ʽ ������������ӵķ�ʽ
			particle.setActive(particleInfo.position, particleInfo.velocity, particleInfo.rotation, particleInfo.scale);
			count--;
		}
	}
}

#endif