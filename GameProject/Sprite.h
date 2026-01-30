#pragma once
#include "Texture.h"

class Sprite
{
public:
	explicit Sprite(const std::string& texturePath, int rows, int columns, float frameDelay, float scaler);
	virtual ~Sprite();

	//RULE OF 5
	Sprite(const Sprite& rhs) = delete;
	Sprite& operator=(const Sprite& rhs) = delete;

	Sprite(Sprite&& rhs) = delete;

	Sprite& operator=(Sprite&& rhs) = delete;

	void Update(float elapsedSec);

	void Draw(const Rectf& dstRect, bool flipped = false, bool upsidedown = false) const;
	void Draw(const Point2f& position, bool flipped = false, bool upsidedown = false) const;

	Point2f GetSize() const;
private:
	Texture* m_Texture;

	int		m_Rows, m_Columns;
	float	m_FrameWidth{ -1 }, m_FrameHeight{ -1 };
	float	m_FrameDelay;
	float	m_AccumulatedTime{};
	int		m_CurrentFrame{};

	float	m_Scaler{};

	bool	m_Loop{ true };

};

