#ifndef PARTICLE_SYSTEM_MASTER_H
#define PARTICLE_SYSTEM_MASTER_H

#include "ParticleSystem.h"
#include <stb_image.h>
#include <vector>

class ParticleSystemMaster{
private:
	static unsigned int loadTexture(const char* path);
	static vector<ParticleSystem> particleSystems;
	static vector<vec3> particleSystemsPositions;
	static bool startRender;
public:
	static void LoadParticlesInfo();
	static void GenerateParticles();
	static void Render(Camera camera);
	static void ChangeRenderState();
};

unsigned int ParticleSystemMaster::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

vector<ParticleSystem> ParticleSystemMaster::particleSystems;
vector<vec3> ParticleSystemMaster::particleSystemsPositions;
bool ParticleSystemMaster::startRender = false;

void ParticleSystemMaster::LoadParticlesInfo()
{
	unsigned int textureID_0, textureID_1, textureID_2, textureID_3, textureID_4, textureID_5;
	textureID_0 = loadTexture("asset/texture/ParticleTexture/particleAtlas.png");
	ParticleTexture fireWorksTexture(textureID_0, 4, Particle_Type::FIREWORKS, false);
	textureID_1 = loadTexture("asset/texture/ParticleTexture/particleFire.png");
	ParticleTexture fireTexture(textureID_1, 8, Particle_Type::FIRE, true);
	textureID_2 = loadTexture("asset/texture/ParticleTexture/particleSmoke.png");
	ParticleTexture smokeTexture(textureID_2, 8, Particle_Type::SMOKE, false);
	textureID_3 = loadTexture("asset/texture/ParticleTexture/snow.bmp");
	ParticleTexture snowTexture(textureID_3, 1, Particle_Type::SNOW, true);
	textureID_4 = loadTexture("asset/texture/ParticleTexture/particleStar.png");
	ParticleTexture starTexture(textureID_4, 1, Particle_Type::STAR, true);
	textureID_5 = loadTexture("asset/texture/ParticleTexture/particleCosmic.png");
	ParticleTexture cosmicTexture(textureID_5, 4, Particle_Type::COSTMIC, true);

	ParticleSystem fireWorks(120, 3.0, -1.5, 5, fireWorksTexture);
	ParticleSystem fire(150, 3.6, 0.15, 2.2, fireTexture);
	ParticleSystem smoke(80, 1.0, 0, 12, smokeTexture);
	ParticleSystem snow(100, 1, -1, 8, snowTexture);
	ParticleSystem star(10, 4, -3, 8, starTexture); 
	ParticleSystem galaxy(200, 3, -2, 4, cosmicTexture);

	particleSystems.push_back(fireWorks);
	particleSystems.push_back(fire);
	particleSystems.push_back(smoke);
	particleSystems.push_back(snow);
	particleSystems.push_back(star);
	particleSystems.push_back(galaxy);

	vec3 fireWorksPos = vec3(-10.0f, 0.0f, -0.0f);
	vec3 firePos = vec3(-10.0f, -2.0f, -10.0f);
	vec3 smokePos = vec3(-10.0, -2.0, -20.0);
	vec3 snowPos = vec3(15.0f, 20.0f, -2.0f);
	vec3 starPos = vec3(-5.0f, 40.0f, -20.0f);
	vec3 galaxyPos = vec3(0.0f, 8.0f, -30.0f);

	particleSystemsPositions.push_back(fireWorksPos);
	particleSystemsPositions.push_back(firePos);
	particleSystemsPositions.push_back(smokePos);
	particleSystemsPositions.push_back(snowPos);
	particleSystemsPositions.push_back(starPos);
	particleSystemsPositions.push_back(galaxyPos);
}

void ParticleSystemMaster::GenerateParticles()
{
	for (int i = 0; i < particleSystems.size(); i++)
	{
		particleSystems[i].generateParticles(particleSystemsPositions[i]);
	}
}

void ParticleSystemMaster::Render(Camera camera)
{
	if (startRender)
	{
		for (int i = 0; i < particleSystems.size(); i++)
		{
			particleSystems[i].render(camera);
		}
	}	
}

void ParticleSystemMaster::ChangeRenderState()
{
	startRender = !startRender;
}
#endif