#pragma once
#include "Sprite.h"
#include "utils.h"
#include "Map.h"

class Character
{
public:
	explicit Character(const Point2f& position);
	virtual ~Character();

	//RULE OF 5
	Character(const Character& rhs) = delete;
	Character& operator=(const Character& rhs) = delete;

	Character(Character&& rhs) = delete;

	Character& operator=(Character&& rhs) = delete;

	void Draw(bool debug = false) const;
	void Update(float elapsedSec);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

	//Handles collision for the player, takes the map as the input
	void HandleCollision(float elapsedSec, const Map* map);

	//Different inputs
	void ApplyForce(Vector2f force);
	void SetPosition(Point2f position);
	void SetVelocity(Vector2f velocity);
	void SetIsGrounded(bool isgrounded);

	//Getting different variables for other classes
	Vector2f GetVelocity() const;
	Point2f GetPosition() const;
	Rectf GetPlayerHitBox() const;
	bool GetIsGrounded() const;

	void IsHitAnimation(bool hit); //For the damage animation
	void DeadAnimation(bool dead); //Removes collisions for few seconds

	//Gets the x center but not the y center
	Point2f GetCenter() const;

	//Interactbles
	void CollisionWall(bool hit, bool left);
	void GetWorldSwitch(bool worldswitch);
private:
	Sprite* m_CharacterIdle;
	Sprite* m_CharacterRunning;
	Sprite* m_CharacterWall;
	Sprite* m_CharacterJump;

	Sprite* m_CharacterDamageIdle;
	Sprite* m_CharacterDamageRunning;
	Sprite* m_CharacterDamageJump;
	Sprite* m_CharacterDamageWall;

	Point2f m_Position;
	Point2f m_PreviousPosition;
	Vector2f m_Velocity;

	//The collision rectangle
	Rectf m_Collider;

	bool m_Flipped, m_Running{ false }, m_UpsideDown{ true };
	bool m_IsGrounded{ false };
	bool m_IsCeiling{ false };
	bool m_HitWall{ false };

	bool m_CollisionCheck{ false };
	bool m_CollisionRightWall{ false };
	bool m_CollisionLeftWall{ false };
	
	//D Jump
	bool m_JumpCheck{true};
	float m_PressedSpace{ 0 };
	float m_DoubleJumpLimit{ 0 };

	float m_WallJumpTimer{ 0 };

	//Gravity switch
	bool m_WorldSwitch{ false };

	//Animation
	bool m_DeadAnimation{ false };
	bool m_HitAnimationOnOff{ false };
	bool m_HitAnimation{ false };
	float m_HitAnimationCounter{};
};

