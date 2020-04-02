#ifndef PARTICLE_MASTER_H
#define PARTICLE_MASTER_H

#include "Particle.h"
#include <list>
using namespace std;

bool cmpDistance (const Particle& p1, const Particle& p2) //���Ӱ�����Զ��˳����Ⱦ 
{ 
	return p1.getDistance() > p2.getDistance();  //�ǵ�particle�����getDistance����Ҫ����Ϊconst ����ᱨ�� ����ԭ���������p1��const���� ����get����ȴ��non const���� ��һ�µ��µĶ�
}

class ParticleMaster{
public:
	list <Particle> particles;
	void update(Camera camera); //�����б��е���������״̬
	void addParticle(Particle particle); //�����µ����� ��ӵ��б���
};

void ParticleMaster::update(Camera camera)
{
	for (auto &particle : particles)
	{
		bool stillAlive = particle.update(camera);
		if (!stillAlive)
		{
			particle.setNotActive();
		}
	}
	if (!particles.front().getTexture().getIsAdditve())
		particles.sort(cmpDistance);
}

void ParticleMaster::addParticle(Particle particle)
{
	particles.push_back(particle);
}

#endif