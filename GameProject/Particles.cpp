#include "pch.h"
#include "Particles.h"
#include "iostream"

Particles::Particles(const std::string& texturePath, Point2f position) : m_Position{ position }
{
	m_Particle = new Texture(texturePath);
}
Particles::~Particles()
{
	delete m_Particle;
	m_Particle = nullptr;
}

void Particles::Draw() const
{
	Rectf dstRect(m_Position.x, m_Position.y, 50.f, 50);

	if (!m_StopDrawing) m_Particle->Draw(dstRect);
}
void Particles::Update(float elapsedSec)
{
	if (!m_StopDrawing) m_Counter += elapsedSec;
	if (m_Counter >= 2.f) {
		m_Counter = 0.f;
		m_StopDrawing = true;
	}
	else if (m_Counter >= 0.2f) {
		if (Reverse) {
			if (OnOff) m_Position.x += -((rand() % 50)) * elapsedSec;
			else m_Position.x += ((rand() % 50)) * elapsedSec;
			m_Position.y += ((rand() % 100) + 100) * elapsedSec;
		}
		else {
			if (OnOff) m_Position.x += ((rand() % 100)) * elapsedSec;
			else m_Position.x += -((rand() % 100)) * elapsedSec;
			m_Position.y += ((rand() % 150) + 100) * elapsedSec;
		}

	}
	else if (m_Counter >= 0.1f) {

		if (OnOff) m_Position.x += ((rand() % 100)) * elapsedSec;
		else m_Position.x += -((rand() % 100)) * elapsedSec;
		if (DownUp) m_Position.y += -((rand() % 400) + (rand() % 400)) * elapsedSec;
		else m_Position.y += ((rand() % 100) - 50) * elapsedSec;
		m_StopDrawing = false;
	}
}

void Particles::GetStartParticles(bool startParicles)
{
	m_StartParticles = startParicles;
}
