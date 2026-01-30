#include "pch.h"
#include "Enemy.h"
#include "iostream"
#include "utils.h"
#include <vector>

using namespace utils;

Enemy::Enemy(const std::string& texturePath, int column, Point2f position, float range, bool wavecheck)
{
	m_Enemy = new Sprite(texturePath, 1, column, 0.15f, 2.f);
	m_Position = position;
	m_Scale = 1.f; //does nothing
	m_WaveCheck = wavecheck;
	m_Range = range;
	m_EnemyTextureCheck = true;
}

Enemy::Enemy() { m_EnemyTextureCheck = false; }

Enemy::~Enemy()
{
	if(m_EnemyTextureCheck) delete m_Enemy;
	m_Enemy = nullptr;
}

void Enemy::Draw() const
{
	const Point2f size{ m_Enemy->GetSize() };
	const Rectf dstRect{ m_Position.x + m_VerticalWave, m_Position.y + m_HorizontalWave, size.x* m_Scale, size.y*m_Scale };

	if(m_FacingSwitch) m_Enemy->Draw(dstRect, true);
	else m_Enemy->Draw(dstRect);	
	//Hitboxes
	//DrawRect(dstRect);
}

void Enemy::WaveMovement(float elapsedSec)
{
	const float SPEED{ 5.f };

	m_Enemy->Update(elapsedSec);
	m_TickCounter += elapsedSec;

	//Enemy Hitbox
	const Point2f size{ m_Enemy->GetSize() };
	const Rectf dstRect{ m_Position.x + m_VerticalWave, m_Position.y + m_HorizontalWave, size.x * m_Scale, size.y * m_Scale };
	
	//Horizontal or Vertical way
	if (m_WaveCheck)
	{
		float horizontalWaveProgression{ m_TickCounter / SPEED };
		m_HorizontalWave = m_Range * sin((2 * M_PI) * horizontalWaveProgression);
	}
	else
	{
		float verticalWaveProgression{ m_TickCounter / SPEED };
		m_VerticalWave = m_Range * sin((2 * M_PI) * verticalWaveProgression);
		if (m_VerticalWave >= (m_Range-10)) m_FacingSwitch = false;
		else if (m_VerticalWave <= (- m_Range+10)) m_FacingSwitch = true;
	}
}

void Enemy::EnemyCollisions(float elapsedSec, Rectf playerHitbox)
{
	const float FORCEBACK{ 100.f };

	//Enemy Hitbox
	const Point2f size{ m_Enemy->GetSize() };
	const Rectf dstRect{ m_Position.x + m_VerticalWave, m_Position.y + m_HorizontalWave, size.x * m_Scale, size.y * m_Scale };

	//Player hitbox
	Point2f bottemLeft{ playerHitbox.left, playerHitbox.bottom };
	Point2f topLeft{ playerHitbox.left, playerHitbox.bottom + playerHitbox.height };
	Point2f bottemRight{ playerHitbox.left + playerHitbox.width, playerHitbox.bottom };
	Point2f topRight{ playerHitbox.left + playerHitbox.width,  playerHitbox.bottom + playerHitbox.height };

		if (IsPointInRect(bottemLeft, dstRect) || IsPointInRect(topLeft, dstRect) || IsPointInRect(bottemRight, dstRect) || IsPointInRect(topRight, dstRect))
		{
			if (IsPointInRect(bottemLeft, dstRect)) { m_Force.x += FORCEBACK * elapsedSec; m_Force.y += FORCEBACK * elapsedSec; m_HitPlayer = true; }
			else if (IsPointInRect(topLeft, dstRect)) { m_Force.x += FORCEBACK * elapsedSec; m_Force.y -= FORCEBACK * elapsedSec; m_HitPlayer = true; }
			else if (IsPointInRect(bottemRight, dstRect)) { m_Force.x -= FORCEBACK * elapsedSec; m_Force.y += FORCEBACK * elapsedSec; m_HitPlayer = true; }
			else if (IsPointInRect(topRight, dstRect)) { m_Force.x -= FORCEBACK * elapsedSec; m_Force.y -= FORCEBACK * elapsedSec;  m_HitPlayer = true; }
		}
		else
		{
			m_Force.x = 0;
			m_Force.y = 0;
			m_HitPlayer = false;
		}

}

void Enemy::DeadzoneCollisions(float elapsedSec, const Map* map, Rectf playerHitbox)
{
	const std::vector<std::vector<Point2f>> polygons{ map->GetPolygons() };

	//Player hitbox
	Point2f bottemLeft{ playerHitbox.left, playerHitbox.bottom };
	Point2f topLeft{ playerHitbox.left, playerHitbox.bottom + playerHitbox.height };
	Point2f bottemRight{ playerHitbox.left + playerHitbox.width, playerHitbox.bottom };
	Point2f topRight{ playerHitbox.left + playerHitbox.width,  playerHitbox.bottom + playerHitbox.height };

	m_Dead = false;

	for (const std::vector<Point2f>& polygon : polygons) {
		if (IsPointInPolygon(bottemLeft, polygon) || IsPointInPolygon(topLeft, polygon) ||
			IsPointInPolygon(topRight, polygon) || IsPointInPolygon(bottemRight, polygon)) m_Dead = true;
	}
}

bool Enemy::IsDead()
{
	return m_Dead;
}

Rectf Enemy::GetEnemyHitBox() const
{
	const Point2f size{ m_Enemy->GetSize() };
	const Rectf dstRect{ m_Position.x + m_VerticalWave, m_Position.y + m_HorizontalWave, size.x * m_Scale, size.y * m_Scale };
	return dstRect;
}

Vector2f Enemy::Force()
{
	return m_Force;
}

bool Enemy::IsHit()
{
	return m_HitPlayer;
}