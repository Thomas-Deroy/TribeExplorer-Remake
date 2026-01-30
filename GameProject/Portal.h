#pragma once
#include "utils.h"
#include "Texture.h"

class Portal
{
public:
	explicit Portal(const std::string& texturePath1, Point2f position1, const std::string& texturePath2, Point2f position2);
	virtual ~Portal();

	//RULE OF 5
	Portal(const Portal& rhs) = delete;
	Portal& operator=(const Portal& rhs) = delete;

	Portal(Portal&& rhs) = delete;

	Portal& operator=(Portal&& rhs) = delete;

	void Draw() const;
	void PortalCollisions(Rectf playerHitbox, float elapedSec);

	Vector2f VelocityChange();

	bool GravitySwitch();
	void GravitySwitchChange(bool gravityswitch);
	bool HitPortal();

private:
	Texture* m_Portal1;
	Point2f m_Position1;
	Texture* m_Portal2;
	Point2f m_Position2;

	float m_RotationSpeed;
	float m_Scaling;
	Point2f m_SizePortal1, m_SizePortal2;
	Rectf m_Portal1Rect, m_Portal2Rect;
	float m_VelocityChangeTimer{ 0.f };
	
	//Returns
	Vector2f m_VelocityChange;
	bool m_GravitySwitch{ false };
	bool m_HitPortal{ false };
};

