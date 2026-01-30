#pragma once
#include "utils.h"
#include "Texture.h"
#include "Text.h"

class Flag
{
public:
	explicit Flag(const std::string& texturePath, Point2f position);
	virtual ~Flag();

	//RULE OF 5
	Flag(const Flag& rhs) = delete;
	Flag& operator=(const Flag& rhs) = delete;

	Flag(Flag&& rhs) = delete;

	Flag& operator=(Flag&& rhs) = delete;

	void Draw() const;
	void FlagCollisions(Rectf playerHitbox);

	bool GameWon();

private:
	Texture* m_Flag;
	Point2f m_Position;

	TTF_Font* m_Font;
	Text* m_ButtonText;
	Texture* m_Button;

	float m_Scaling;
	Point2f m_Size;
	Rectf m_FlagRect;

	bool m_HoverFlag{ false };
	bool m_WonGame{ false };
};

