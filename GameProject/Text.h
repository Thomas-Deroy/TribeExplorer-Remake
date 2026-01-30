#pragma once
#include "Texture.h"
class Text
{
public:
	explicit Text(std::string text, Color4f textColor, TTF_Font* fontPath);
	virtual ~Text();

	//RULE OF 5
	Text(const Text& rhs) = delete;
	Text& operator=(const Text& rhs) = delete;

	Text(Text&& rhs) = delete;

	Text& operator=(Text&& rhs) = delete;

	void Draw(Point2f position) const;

protected:
	TTF_Font* m_Font;
	Color4f m_TextColor;
	std::string m_Text;
	Texture* m_TextTexture;
};

