#include "pch.h"
#include "Text.h"

Text::Text(std::string text, Color4f textColor, TTF_Font* fontPath)
	:m_TextColor(textColor), m_Text(text), m_Font(fontPath)
{
	m_TextTexture = new Texture(m_Text, m_Font, m_TextColor);
}

Text::~Text()
{
	delete m_TextTexture;
	m_TextTexture = nullptr;
}

void Text::Draw(Point2f position) const
{
	m_TextTexture->Draw(Point2f{ position.x - m_TextTexture->GetWidth() / 2,
									position.y - m_TextTexture->GetHeight() / 2 });
}