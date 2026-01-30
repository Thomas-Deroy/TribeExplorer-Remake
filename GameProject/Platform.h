#pragma once
#include "utils.h"

class Platform
{
public:
	explicit Platform(Point2f position, float width);
	virtual ~Platform();

	//RULE OF 5
	Platform(const Platform& rhs) = delete;
	Platform& operator=(const Platform& rhs) = delete;

	Platform(Platform&& rhs) = delete;
	Platform& operator=(Platform&& rhs) = delete;

	void Draw();
	void PlatformCollisions(Rectf playerHitbox, Vector2f velocity);
	bool IsOnPlatform();
private:
	Point2f m_Position;
	Rectf m_Platform;

	bool m_IsGrounded;
};

