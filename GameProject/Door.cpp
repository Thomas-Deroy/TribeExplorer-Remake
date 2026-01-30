#include "pch.h"
#include "Door.h"
#include "iostream"

using namespace utils;

Door::Door(const std::string& texturePath, Point2f position)
{
	m_Door		= new Texture(texturePath);
	m_Position	= position;

	m_Size		= Point2f(m_Door->GetWidth(), m_Door->GetHeight());
	m_Scaling	= 6.f;
	m_Range		= 50.f;

	m_DoorRect		= Rectf{ m_Position.x, m_Position.y, m_Size.x * m_Scaling, m_Size.y * m_Scaling };
	m_DoorRangeRect = Rectf{ m_Position.x - m_Range, m_Position.y, (m_Size.x * m_Scaling) + m_Range * 2, (m_Size.y * m_Scaling) };
}

Door::~Door()
{
	delete m_Door;
}

void Door::Draw() const
{
	m_Door->Draw(m_DoorRect);
}

void Door::DoorCollisions(Rectf playerHitbox, bool key, float elapsedSec)
{
	//Player hitbox
	Point2f bottemLeft{ playerHitbox.left, playerHitbox.bottom };
	Point2f topLeft{ playerHitbox.left, playerHitbox.bottom + playerHitbox.height };
	Point2f bottemRight{ playerHitbox.left + playerHitbox.width, playerHitbox.bottom };
	Point2f topRight{ playerHitbox.left + playerHitbox.width,  playerHitbox.bottom + playerHitbox.height };

	//Checking the right side for collision
	if (IsOverlapping(playerHitbox, m_DoorRect)) m_HitDoor = true;
	else m_HitDoor = false;

	//Only if they have key then open
	if (key) if (IsPointInRect(bottemLeft, m_DoorRangeRect) ||
				IsPointInRect(topLeft, m_DoorRangeRect)		||
				IsPointInRect(bottemRight, m_DoorRangeRect) ||
				IsPointInRect(topRight, m_DoorRangeRect))
	{
		m_DoorFullyOpened = 5.f;
		m_UsedKey = true;
	}
	else m_UsedKey = false;
	//Opening Speed
	if (m_DoorFullyOpened >= 50.f) m_DoorFullyOpened = 50.f;
	m_DoorRect.bottom += m_DoorFullyOpened * (50.f * elapsedSec);
}

bool Door::HitDoor()
{
	return m_HitDoor;
}

bool Door::UsedKey()
{
	return m_UsedKey;
}
