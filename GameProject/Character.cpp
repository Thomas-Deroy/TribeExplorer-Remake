#include "pch.h"
#include "Character.h"
#include "iostream"
#include "structs.h"
#include "utils.h"
#include <vector>

using namespace utils;

Character::Character(const Point2f& position) : m_Position(position), m_PreviousPosition(position), m_Velocity(Vector2f()), m_Flipped(false),  m_DoubleJumpLimit(0.f)
{
	m_CharacterIdle		= new Sprite("IdleCharactarSprite.png", 1, 11, 0.03f, 1.6f);
	m_CharacterRunning	= new Sprite("RunCharactarSprite.png", 1, 12, 0.03f, 1.6f);
	m_CharacterWall		= new Sprite("WallJumpCharactarSprite.png", 1, 5, 0.03f, 1.6f);
	m_CharacterJump		= new Sprite("JumpCharactarSprite.png", 1, 3, 0.20f, 1.6f);

	m_CharacterDamageIdle		= new Sprite("IdleCharactarDamageSprite.png", 1, 11, 0.03f, 1.6f);
	m_CharacterDamageRunning	= new Sprite("RunCharactarDamageSprite.png", 1, 12, 0.03f, 1.6f);
	m_CharacterDamageJump		= new Sprite("JumpCharactarDamageSprite.png", 1, 3, 0.20f, 1.6f);
	m_CharacterDamageWall		= new Sprite("WallJumpCharactarDamage.png", 1, 5, 0.03f, 1.6f);

	const Point2f spriteSize{ m_CharacterIdle->GetSize() };

	// Create the character collider
	m_Collider = Rectf{ m_Position.x, m_Position.y, spriteSize.x, spriteSize.y};
}

Character::~Character()
{
	delete m_CharacterIdle;
	delete m_CharacterRunning;
	delete m_CharacterWall;
	delete m_CharacterJump;
	m_CharacterIdle = nullptr;
	m_CharacterRunning = nullptr;
	m_CharacterWall = nullptr;
	m_CharacterJump = nullptr;
	delete m_CharacterDamageIdle;
	delete m_CharacterDamageRunning;
	delete m_CharacterDamageJump;
	delete m_CharacterDamageWall;
	m_CharacterDamageIdle = nullptr;
	m_CharacterDamageRunning = nullptr;
	m_CharacterDamageJump = nullptr;
	m_CharacterDamageWall = nullptr;
}

void Character::Draw(bool debug) const
{
	const Point2f size{ m_CharacterIdle->GetSize() };
	const Rectf dstRect{ m_Position.x, m_Position.y, size.x, size.y };
	Rectf dstRectWall{ m_Position.x + 10, m_Position.y, size.x, size.y };
	Rectf dstRectWallFlipped{ m_Position.x - 10, m_Position.y, size.x , size.y };

	//Animation sprite
	if (m_WorldSwitch) {
		if (m_HitWall)
		{
			if (m_Flipped) { m_CharacterWall->Draw(dstRectWallFlipped, m_Flipped, true); }
			else { m_CharacterWall->Draw(dstRectWall, false, true); }
		}
		else
		{
			if (m_Velocity.y <= 20.f)
			{
				if (m_Running)
				{
					if (m_Flipped) { m_CharacterRunning->Draw(dstRect, m_Flipped, true); }
					else { m_CharacterRunning->Draw(dstRect, false, true); }
				}
				else
				{
					if (m_Flipped) { m_CharacterIdle->Draw(dstRect, m_Flipped, true); }
					else m_CharacterIdle->Draw(dstRect, false, true);
				}
			}
			else if (m_Velocity.y >= 20.f) m_CharacterJump->Draw(dstRect, m_Flipped, true);
		}
	}
	else {
		if (m_HitWall && !m_DeadAnimation)
		{
			if (m_Flipped) { if (m_HitAnimationOnOff) m_CharacterDamageWall->Draw(dstRectWallFlipped, m_Flipped); else m_CharacterWall->Draw(dstRectWallFlipped, m_Flipped); }
			else { if (m_HitAnimationOnOff) m_CharacterDamageWall->Draw(dstRectWall); else m_CharacterWall->Draw(dstRectWall); }
		}
		else
		{
			if (m_Velocity.y <= 20.f)
			{
				if (m_Running)
				{
					if (m_Flipped) { if (m_HitAnimationOnOff) m_CharacterDamageRunning->Draw(dstRect, m_Flipped); else m_CharacterRunning->Draw(dstRect, m_Flipped); }
					else { if (m_HitAnimationOnOff) m_CharacterDamageRunning->Draw(dstRect); else m_CharacterRunning->Draw(dstRect); }
				}
				else
				{
					if (m_Flipped) { if (m_HitAnimationOnOff) m_CharacterDamageIdle->Draw(dstRect, m_Flipped); else m_CharacterIdle->Draw(dstRect, m_Flipped); }
					else { if (m_HitAnimationOnOff) m_CharacterDamageIdle->Draw(dstRect); else m_CharacterIdle->Draw(dstRect); }
				}
			}
			else if (m_Velocity.y >= 20.f) if (m_HitAnimationOnOff) m_CharacterDamageJump->Draw(dstRect, m_Flipped); else m_CharacterJump->Draw(dstRect, m_Flipped);
		}
	}

	//if (debug) DrawRect(m_Collider);
}

void Character::Update(float elapsedSec)
{
	const float COUNTERFORCE{ 0.90f };
	float GRAVITY{ 1000.f };
	if (m_WorldSwitch) GRAVITY = -GRAVITY;
	//Update sprites
	m_CharacterIdle->Update(elapsedSec);
	m_CharacterRunning->Update(elapsedSec);
	m_CharacterJump->Update(elapsedSec);
	m_CharacterWall->Update(elapsedSec);

	m_CharacterDamageIdle->Update(elapsedSec);
	m_CharacterDamageRunning->Update(elapsedSec);
	m_CharacterDamageJump->Update(elapsedSec);

	//Apply the velocity
	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;

	m_Collider.left = m_Position.x;
	m_Collider.bottom = m_Position.y;

	m_Velocity.x *= COUNTERFORCE * (1 - elapsedSec);

	//Controls
	const float JUMP_VELOCITY{ 500.f };
	const float WALK_VELOCITY{ 250.f };
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	bool isLeft{ bool(pStates[SDL_SCANCODE_A]) };
	bool isRight{ bool(pStates[SDL_SCANCODE_D]) };
	bool isJump{ bool(pStates[SDL_SCANCODE_SPACE]) };

	if (m_IsGrounded && m_DoubleJumpLimit > 0) m_DoubleJumpLimit = 0;
	if (m_PressedSpace == 2) m_PressedSpace = 0;

	if (m_WallJumpTimer >= 5.f) m_WallJumpTimer = 0.f;
	else if (m_WallJumpTimer >= 1.f) m_WallJumpTimer += (elapsedSec * 50);

	if (!m_DeadAnimation) {
		//Wall jump left
		if (isLeft && m_HitWall && isJump && m_JumpCheck) { m_Running = true; m_Flipped = false; isLeft = false; }
		else if (isLeft && m_WallJumpTimer >= 1.f) { m_Velocity.x += (WALK_VELOCITY * 0.25f); m_Velocity.y += (JUMP_VELOCITY * 0.025f); } //Wall jump motion left
		//Normal left
		else if (isLeft) { m_Running = true; m_Flipped = true; m_Velocity.x = -WALK_VELOCITY; isLeft = false; }

		//Wall jump right
		if (isRight && m_HitWall && isJump && m_JumpCheck) { m_Running = true; m_Flipped = true; isRight = false; }
		else if (isRight && m_WallJumpTimer >= 1.f) { m_Velocity.x += (-WALK_VELOCITY * 0.25f); m_Velocity.y += (JUMP_VELOCITY * 0.025f); } //Wall jump motion right
		//Normal left
		else if (isRight) { m_Running = true; m_Flipped = false; m_Velocity.x = WALK_VELOCITY; isRight = false; }

		if (isJump) {
			
			//Wall jump
			if (m_HitWall && m_JumpCheck) {
				m_WallJumpTimer = 1;
				m_JumpCheck = false;
				if (m_WorldSwitch) m_Velocity.y += -JUMP_VELOCITY;
				else {
					if (m_Flipped) m_Velocity.x += -WALK_VELOCITY;
					else m_Velocity.x += WALK_VELOCITY;
					m_Velocity.y += JUMP_VELOCITY;
				}
				isJump = false;
			}
			if (m_JumpCheck) { m_PressedSpace++; m_JumpCheck = false; }
			//Normal jump
			if (m_WorldSwitch) {
				if (m_IsCeiling && !m_HitWall && m_Velocity.y <= 0)
				{
					m_Velocity.y = -JUMP_VELOCITY;
					m_DoubleJumpLimit++;
					m_PressedSpace = 0;
					isJump = false;
				}
			}
			else {
				if (m_IsGrounded && !m_HitWall && m_Velocity.y <= 0)
				{
					m_Velocity.y = JUMP_VELOCITY;
					m_DoubleJumpLimit++;
					m_PressedSpace = 0;
					isJump = false;
				}
			}
			//Double jump
			if (m_PressedSpace == 1 && m_DoubleJumpLimit == 1 && !m_HitWall || m_Velocity.y <= -1 && m_DoubleJumpLimit == 0)
			{
				m_Velocity.y = 0.f;
				if (m_WorldSwitch) m_Velocity.y = -JUMP_VELOCITY;
				else m_Velocity.y = JUMP_VELOCITY;
				m_DoubleJumpLimit++;
				isJump = false;
			}
		}
	}

	//Gravity
	if (m_WorldSwitch) {
		if (!m_IsCeiling) {
			if (m_HitWall) m_Velocity.y -= (GRAVITY / 10) * elapsedSec;
			else m_Velocity.y -= GRAVITY * elapsedSec;
		}
	}
	else {
		if (!m_IsGrounded) {
			if (m_HitWall) m_Velocity.y -= (GRAVITY / 10) * elapsedSec;
			else m_Velocity.y -= GRAVITY * elapsedSec;
		}
	}

	//Hit animation
	if (m_HitAnimationCounter >= 1) {
		m_HitAnimationCounter += (10 * elapsedSec);
		if (m_HitAnimationCounter >= 7) {
			m_HitAnimationOnOff = false;
			m_HitAnimationCounter = 0;
		}
		else if (m_HitAnimationCounter >= 6) m_HitAnimationOnOff = true;
		else if (m_HitAnimationCounter >= 5) m_HitAnimationOnOff = false;
		else if (m_HitAnimationCounter >= 4) m_HitAnimationOnOff = true;
		else if (m_HitAnimationCounter >= 3) m_HitAnimationOnOff = false;
		else if (m_HitAnimationCounter >= 2) m_HitAnimationOnOff = true;
	}
}

void Character::HandleCollision(float elapsedSec, const Map* map)
{
	const Point2f nextPosition{ m_Position.x + m_Velocity.x * elapsedSec, m_Position.y + m_Velocity.y * elapsedSec };

	const Rectf nextCollider{ m_Collider.left + m_Velocity.x * elapsedSec, m_Collider.bottom + m_Velocity.y * elapsedSec,  m_Collider.width, m_Collider.height };
	//Points from player
	const Point2f bottomLeft{ m_Collider.left, m_Collider.bottom };
	const Point2f topLeft{ m_Collider.left, m_Collider.bottom + m_Collider.height };
	const Point2f topRight{ m_Collider.left + m_Collider.width, m_Collider.bottom + m_Collider.height };
	const Point2f bottomRight{ m_Collider.left + m_Collider.width, m_Collider.bottom };

	//Next points from player
	const Point2f nextBottomLeft{ nextCollider.left, nextCollider.bottom };
	const Point2f nextTopLeft{ nextCollider.left, nextCollider.bottom + nextCollider.height };
	const Point2f nextTopRight{ nextCollider.left + nextCollider.width, nextCollider.bottom + nextCollider.height };
	const Point2f nextBottomRight{ nextCollider.left + nextCollider.width, nextCollider.bottom };

	HitInfo infoBottomLeft{};
	HitInfo infoBottomRight{};
	HitInfo infoTopLeft{};
	HitInfo infoTopRight{};
	
	if (!m_DeadAnimation) {
		// Check collision for each polygon
		const std::vector<std::vector<Point2f>> polygons{ map->GetPolygons() };

		m_HitWall = false;
		if(m_CollisionCheck) m_IsGrounded = true;
		else m_IsGrounded = false;
		m_IsCeiling = false;

		//World collisions
		for (const std::vector<Point2f> polygon : polygons) {
			const bool hitBottomLeft{ Raycast(polygon, bottomLeft, nextBottomLeft, infoBottomLeft) };
			const bool hitBottomRight{ Raycast(polygon, bottomRight, nextBottomRight, infoBottomRight) };
			const bool hitTopLeft{ Raycast(polygon, topLeft, nextTopLeft, infoTopLeft) };
			const bool hitTopRight{ Raycast(polygon, topRight, nextTopRight, infoTopRight) };

			m_IsGrounded = m_IsGrounded || infoBottomLeft.normal.y > 0.f || infoBottomRight.normal.y > 0.f;
			m_IsCeiling = m_IsCeiling || infoTopLeft.normal.y > 0.f || infoTopRight.normal.y > 0.f;

			// Adjustment of the collision because the svg is not pixel-perfect
			if (IsPointInPolygon(bottomLeft, polygon) || IsPointInPolygon(topLeft, polygon) || IsPointInPolygon(topRight, polygon) || IsPointInPolygon(bottomRight, polygon)) {
				m_Position = m_PreviousPosition;
				if(IsPointInPolygon(topLeft, polygon) || IsPointInPolygon(topRight, polygon)) m_Velocity.y = -1.f;
				if(IsPointInPolygon(bottomLeft, polygon) || IsPointInPolygon(bottomRight, polygon)) m_Velocity.y = 0.f;
			}

			//Wall collisions
			if ((hitBottomLeft && !m_IsGrounded) || (hitTopLeft && !m_IsCeiling)) m_Velocity.x = 1.f;
			else if ((hitBottomRight && !m_IsGrounded) || (hitTopRight && !m_IsCeiling)) m_Velocity.x = -1.f;

			//Sliding collision
			if ((hitBottomLeft && !m_IsGrounded && m_Velocity.x >= 0.f) || (hitTopLeft && m_Velocity.x >= 0.f && !m_IsCeiling)) { m_HitWall = true, m_Flipped = true; }
			if ((hitBottomRight && !m_IsGrounded && m_Velocity.x <= 0.f) || (hitTopRight && m_Velocity.x <= 0.f && !m_IsCeiling)) { m_HitWall = true; }
			
			//Can't sliding up
			if (m_HitWall && ((m_Velocity.y > 0 && hitBottomLeft) || m_HitWall && (m_Velocity.y > 0 && hitBottomRight))) {
				m_Velocity.y = 1.f;
			}
			if (m_HitWall && m_Velocity.y <= -100.f) { //To prevent going to fast down
				m_Velocity.y += 20.f;
			}

			//std::cout << m_IsCeiling << std::endl;
			if (m_IsGrounded) m_Velocity.y = 0.f;
			if (m_IsCeiling) m_Velocity.y = -1.f;
		}
		m_PreviousPosition = m_Position;
	}
}

void Character::CollisionWall(bool hit, bool left)
{
	if (left) {
		if (hit) m_Velocity.x = 8.f;
	}
	else {
		if (hit) m_Velocity.x = -8.f;
	}	
}

void Character::GetWorldSwitch(bool worldswitch)
{
	m_WorldSwitch = worldswitch;
}

//Different inputs
void Character::ApplyForce(Vector2f force)
{
	m_Velocity += force;
}

void Character::SetPosition(Point2f position)
{
	m_Position = position;
	m_Velocity = Vector2f(0.f, 0.f);
}

void Character::SetVelocity(Vector2f velocity)
{
	m_Velocity = velocity;
}

void Character::SetIsGrounded(bool isgrounded)
{
	m_CollisionCheck = isgrounded;
}

//Getting different variables for other classes
Vector2f Character::GetVelocity() const
{
	return m_Velocity;
}

Point2f Character::GetPosition() const
{
	return m_Position;
}

Rectf Character::GetPlayerHitBox() const
{
	const Point2f size{ m_CharacterIdle->GetSize() };
	const Rectf dstRect{ m_Position.x, m_Position.y, size.x, size.y };
	return dstRect;
}

bool Character::GetIsGrounded() const
{
	return m_IsGrounded;
}

Point2f Character::GetCenter() const
{
	const Point2f size{ m_CharacterIdle->GetSize() };

	return Point2f{
	  m_Position.x + size.x / 2.f,
	  m_Position.y + size.y / 2.f
	};
}

void Character::DeadAnimation(bool dead)
{
	m_DeadAnimation = dead;
}
void Character::IsHitAnimation(bool hit)
{
	if (hit) m_HitAnimationCounter = 1;
}

void Character::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_a:
		m_Running = false;
		break;
	case SDLK_q:
		m_Running = false;
		break;
	case SDLK_d:
		m_Running = false;
		break;
	case SDLK_SPACE:
		m_JumpCheck = true;
		break;
	}
}
