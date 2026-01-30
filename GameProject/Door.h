#pragma once
#include "utils.h"
#include "Texture.h"
class Door
{
public:
	explicit Door(const std::string& texturePath, Point2f position);
	virtual ~Door();

	//RULE OF 5
	Door(const Door& rhs) = delete;
	Door& operator=(const Door& rhs) = delete;

	Door(Door&& rhs) = delete;

	Door& operator=(Door&& rhs) = delete;

	void Draw() const;
	void DoorCollisions(Rectf playerHitbox, bool key, float elapsedSec);

	bool HitDoor();
	bool UsedKey();
private:

	Texture* m_Door;
	Point2f m_Position;

	float m_Scaling;
	float m_Range;
	Point2f m_Size;
	Rectf m_DoorRect;
	Rectf m_DoorRangeRect;

	bool m_HitDoor{ false };
	bool m_UsedKey{ false };

	float m_DoorFullyOpened{ 0 };
};

