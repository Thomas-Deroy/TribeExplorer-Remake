#include "pch.h"
#include "Portal.h"
#include "iostream"

using namespace utils;

Portal::Portal(const std::string& texturePath1, Point2f position1, const std::string& texturePath2, Point2f position2)
{
	m_Portal1 = new Texture(texturePath1);
	m_Position1 = position1;
	m_Portal2 = new Texture(texturePath2);
	m_Position2 = position2;

	m_SizePortal1 = Point2f(m_Portal1->GetWidth(), m_Portal1->GetHeight());
	m_SizePortal2 = Point2f(m_Portal2->GetWidth(), m_Portal2->GetHeight());
	m_Scaling = 2.f;

	m_RotationSpeed = 0.f;
	m_Portal1Rect = Rectf{ m_Position1.x, m_Position1.y, m_SizePortal1.x * m_Scaling, m_SizePortal1.y * m_Scaling };
	m_Portal2Rect = Rectf{ m_Position2.x, m_Position2.y, m_SizePortal2.x * m_Scaling, m_SizePortal2.y * m_Scaling };

}
Portal::~Portal()
{
	delete m_Portal1; m_Portal1 = nullptr;
	delete m_Portal2; m_Portal2 = nullptr;
}

void Portal::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Position1.x + m_SizePortal1.x * m_Scaling / 2, m_Position1.y + m_SizePortal1.y * m_Scaling / 2, 0.f);
		glRotatef(m_RotationSpeed, 0.f, 0.f, 1.f);
		glTranslatef(-m_Position1.x + -m_SizePortal1.x * m_Scaling / 2, -m_Position1.y + -m_SizePortal1.y * m_Scaling / 2, 0.f);

		m_Portal1->Draw(m_Portal1Rect);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(m_Position2.x + m_SizePortal2.x * m_Scaling / 2, m_Position2.y + m_SizePortal2.y * m_Scaling / 2, 0.f);
		glRotatef(m_RotationSpeed, 0.f, 0.f, 1.f);
		glTranslatef(-m_Position2.x + -m_SizePortal2.x * m_Scaling / 2, -m_Position2.y + -m_SizePortal2.y * m_Scaling / 2, 0.f);

		m_Portal2->Draw(m_Portal2Rect);
	}
	glPopMatrix();

}

void Portal::PortalCollisions(Rectf playerHitbox, float elapedSec)
{
	const float FORCE{ 200.f };

	Point2f bottemLeft{ playerHitbox.left, playerHitbox.bottom };
	Point2f topLeft{ playerHitbox.left, playerHitbox.bottom + playerHitbox.height };
	Point2f bottemRight{ playerHitbox.left + playerHitbox.width, playerHitbox.bottom };
	Point2f topRight{ playerHitbox.left + playerHitbox.width,  playerHitbox.bottom + playerHitbox.height };

	m_RotationSpeed += 100.f * elapedSec;

	if (IsPointInRect(bottemLeft, m_Portal1Rect) ||
		IsPointInRect(topLeft, m_Portal1Rect) ||
		IsPointInRect(bottemRight, m_Portal1Rect) ||
		IsPointInRect(topRight, m_Portal1Rect)) {
		m_GravitySwitch = true;
		m_VelocityChange.y = 0;

		if (m_VelocityChangeTimer == 0.f) { m_HitPortal = true; m_VelocityChangeTimer = 1.f; }
		else m_HitPortal = false;
	}
	else if (IsPointInRect(bottemLeft, m_Portal2Rect) ||
		IsPointInRect(topLeft, m_Portal2Rect) ||
		IsPointInRect(bottemRight, m_Portal2Rect) ||
		IsPointInRect(topRight, m_Portal2Rect)) {
		m_GravitySwitch = false;
		m_VelocityChange.y = 0;

		if (m_VelocityChangeTimer == 0.f) { m_HitPortal = true; m_VelocityChangeTimer = 1.f; }
		else m_HitPortal = false;
	}

	if (m_VelocityChangeTimer >= 1.f) m_VelocityChangeTimer += 2 * elapedSec;
	if (m_VelocityChangeTimer >= 2.f) { m_HitPortal = false;  m_VelocityChangeTimer = 0.f; }
}

bool Portal::GravitySwitch()
{
	return m_GravitySwitch;
}

void Portal::GravitySwitchChange(bool gravityswitch)
{
	m_GravitySwitch = gravityswitch;
}

Vector2f Portal::VelocityChange()
{
	return m_VelocityChange;
}

bool Portal::HitPortal()
{
	return m_HitPortal;
}