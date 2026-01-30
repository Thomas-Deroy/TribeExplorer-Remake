#include "pch.h"
#include "Key.h"
#include "utils.h"

using namespace utils;

Key::Key(const std::string& texturePath, Point2f position)
{
	m_Key = new Texture(texturePath);
	m_Position = position;
	m_PickUpSoundEffect = new SoundEffect("PickUpSoundEffect.mp3");

	for (int idx = 0; idx < 20; idx++)
	{
		m_Particles.push_back(new Particles("Particle.png", Point2f(m_Key->GetWidth() + m_Position.x, m_Key->GetHeight() + m_Position.y)));
	}

	m_Size = Point2f(m_Key->GetWidth(), m_Key->GetHeight());
	m_Scaling = 3.f;

	m_KeyRect = Rectf{ m_Position.x, m_Position.y, m_Size.x * m_Scaling, m_Size.y * m_Scaling };
}

Key::~Key() 
{
	delete m_Key;					m_Key = nullptr;
	delete m_PickUpSoundEffect;		m_PickUpSoundEffect = nullptr;
	for (int idx = 0; idx < 20; idx++)
	{
		delete m_Particles[idx];
		m_Particles[idx] = nullptr;
	}
}

void Key::Draw() const
{
	if (!m_HasTakenKey) {
		m_Key->Draw(m_KeyRect);
	}
	else {
		for (int idx = 0; idx < 20; idx++)
		{
			m_Particles[idx]->Draw();
		}
		if (!m_PlayOnce) m_PickUpSoundEffect->Play(false);
	}
}

void Key::KeyCollisions(Rectf playerHitbox, float elapsedSec)
{
	//Player hitbox
	Point2f bottemLeft{ playerHitbox.left, playerHitbox.bottom };
	Point2f topLeft{ playerHitbox.left, playerHitbox.bottom + playerHitbox.height };
	Point2f bottemRight{ playerHitbox.left + playerHitbox.width, playerHitbox.bottom };
	Point2f topRight{ playerHitbox.left + playerHitbox.width,  playerHitbox.bottom + playerHitbox.height };

	if (!m_HasTakenKey) {
		if (IsPointInRect(bottemLeft,	m_KeyRect) || 
			IsPointInRect(topLeft,		m_KeyRect) ||
			IsPointInRect(bottemRight,	m_KeyRect) || 
			IsPointInRect(topRight,		m_KeyRect)) {
			m_HasKey = true;
			m_HasTakenKey = true;
		}
	}
	else {
		for (int idx = 0; idx < m_Particles.size(); idx++)
		{
			m_Particles[idx]->Update(elapsedSec);
		}
		if (!m_PlayOnce) m_PlayOnce = true;
		m_HasKey = false;
	}

}

bool Key::HasKey()
{
	return m_HasKey;
}
