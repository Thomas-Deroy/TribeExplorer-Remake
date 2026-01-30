#pragma once
#include "utils.h"
#include "Texture.h"
#include "Particles.h"
#include "SoundEffect.h"
#include <vector>
class Key
{
public:
	explicit Key(const std::string& texturePath, Point2f position);
	virtual ~Key();

	//RULE OF 5
	Key(const Key& rhs) = delete;
	Key& operator=(const Key& rhs) = delete;

	Key(Key&& rhs) = delete;

	Key& operator=(Key&& rhs) = delete;

	void Draw() const;
	void KeyCollisions(Rectf playerHitbox, float elapsedSec);

	bool HasKey();

private:
	Texture* m_Key;
	Point2f m_Position;
	SoundEffect* m_PickUpSoundEffect;
	std::vector<Particles*> m_Particles;

	float m_Scaling;
	Point2f m_Size;
	Rectf m_KeyRect;

	bool m_PlayOnce{ false };
	bool m_HasKey{ false };
	bool m_HasTakenKey{ false };

};

