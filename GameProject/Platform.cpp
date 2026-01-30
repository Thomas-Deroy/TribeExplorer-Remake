#include "pch.h"
#include "Platform.h"

using namespace utils;

Platform::Platform(Point2f position, float width) : m_Position{ position }
{
	m_Platform = Rectf{ position.x, position.y, width, 1.f };
}
Platform::~Platform()
{

}

void Platform::Draw()
{
	DrawRect(m_Platform);
}
void Platform::PlatformCollisions(Rectf playerHitbox, Vector2f velocity)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	bool fallThrough{ bool(pStates[SDL_SCANCODE_S]) };
	Rectf playerBottomHitbox{ playerHitbox.left, playerHitbox.bottom, playerHitbox.width, 5.f };

	if (IsOverlapping(playerBottomHitbox, m_Platform) && velocity.y <= -1.f && !fallThrough) m_IsGrounded = true;
	else m_IsGrounded = false;
}

bool Platform::IsOnPlatform()
{
	return m_IsGrounded;
}