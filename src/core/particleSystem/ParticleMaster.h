#ifndef PARTICLE_MASTER_H
#define PARTICLE_MASTER_H

#include "Particle.h"
#include <list>
using namespace std;

bool cmpDistance (const Particle& p1, const Particle& p2) //粒子按距离远近顺序渲染 
{ 
	return p1.getDistance() > p2.getDistance();  //记得particle里面的getDistance方法要声明为const 否则会报错 报错原因是这里的p1是const对象 但是get方法却是non const方法 不一致导致的额
}

class ParticleMaster{
public:
	list <Particle> particles;
	void update(Camera camera); //更新列表中的所有粒子状态
	void addParticle(Particle particle); //增加新的粒子 添加到列表中
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