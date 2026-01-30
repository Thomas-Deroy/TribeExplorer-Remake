#pragma once
#include "utils.h"
#include "Text.h"
using namespace utils;

class Button
{
public:
	explicit Button(Point2f position, std::string text, Color4f textColor, TTF_Font* fontPath);
	virtual ~Button();

	//RULE OF 5
	Button(const Button& rhs) = delete;
	Button& operator=(const Button& rhs) = delete;

	Button(Button&& rhs) = delete;

	Button& operator=(Button&& rhs) = delete;

	void Draw();
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
	bool IsPressed();
private:
	bool m_Hover{ false };
	bool m_Pressed{ false };
	Rectf	m_MyBounds;
	Point2f m_Position;

	TTF_Font* m_Font;
	Color4f m_TextColor;
	std::string m_Text;
	Texture* m_TextTexture;
};

