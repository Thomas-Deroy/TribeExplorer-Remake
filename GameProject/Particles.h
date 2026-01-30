#pragma once

#include "Texture.h"
#include "Utils.h"

using namespace utils;

class Particles
{
public:
	explicit Particles(const std::string& texturePath, Point2f position);
	virtual ~Particles();

	//RULE OF 5
	Particles(const Particles& rhs) = delete;
	Particles& operator=(const Particles& rhs) = delete;

	Particles(Particles&& rhs) = delete;
	Particles& operator=(Particles&& rhs) = delete;

	void Draw() const;
	void Update(float elapsedSec);

	void GetStartParticles(bool startParicles);

private:

	Texture* m_Particle;

	Point2f m_Position;
	float m_Counter{ 1.f };

	bool m_StopDrawing{ false };
	bool m_StartParticles{ false };

	bool Reverse{ bool(rand() % 2) };
	bool OnOff{ bool(rand() % 2) };
	bool DownUp{ bool(rand() % 2) };

};

