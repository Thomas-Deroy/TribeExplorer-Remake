#pragma once
#include "Sprite.h"
#include "utils.h"
#include <math.h>
#include "Character.h"

class Enemy
{
public:
	//Constructor
	explicit Enemy(const std::string& texturePath, int column, Point2f position, float range = 200, bool wavecheck = true);
	explicit Enemy();
	virtual ~Enemy();

	//RULE OF 5
	Enemy(const Enemy& rhs) = delete;
	Enemy& operator=(const Enemy& rhs) = delete;

	Enemy(Enemy&& rhs) = delete;

	Enemy& operator=(Enemy&& rhs) = delete;

	void Draw() const;

	//Enemy Entity
	void WaveMovement(float elapsedSec);
	void EnemyCollisions(float elapsedSec, Rectf playerHitbox);

	//Enemy Map
	void DeadzoneCollisions(float elapsedSec, const Map* map, Rectf playerHitbox);

	//Return functions
	Vector2f Force();
	Rectf GetEnemyHitBox() const;
	bool IsHit();
	bool IsDead();
private:
	Sprite* m_Enemy;
	bool m_EnemyTextureCheck;
	Point2f m_Position;

	float m_Scale;

	//Wave movement
	float m_HorizontalWave{ 0.f };
	float m_VerticalWave{ 0.f };
	float m_TickCounter{ 0 };
	bool m_WaveCheck{ false };
	float m_Range{ 0.f };
	bool m_FacingSwitch{ false };

	Vector2f m_Force{0,0};
	bool m_HitPlayer{ false };
	bool m_Dead{ false };
};

