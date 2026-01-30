#include "pch.h"
#include "Flag.h"

using namespace utils;

Flag::Flag(const std::string& texturePath, Point2f position)
{
	m_Flag = new Texture(texturePath);
	m_Position = position;

	m_Font = TTF_OpenFont("Font.ttf", 30);
	m_ButtonText = new Text("E", Color4f{ 1.0f, 1.0f, 1.0f, 1.0f }, m_Font);
	m_Button = new Texture("Button.png");

	m_Size = Point2f(m_Flag->GetWidth(), m_Flag->GetHeight());
	m_Scaling = 3.f;

	m_FlagRect = Rectf{ m_Position.x, m_Position.y, m_Size.x * m_Scaling, m_Size.y * m_Scaling };
}

Flag::~Flag()
{
	delete m_Flag;			m_Flag = nullptr;
	delete m_ButtonText;	m_ButtonText = nullptr;
	delete m_Button;		m_Button = nullptr;

	if (m_Font != nullptr)
	{
		TTF_CloseFont(m_Font);
		m_Font = nullptr;
	}
}

void Flag::Draw() const
{
	const Rectf button{ m_Position.x + (m_Flag->GetWidth() / 4 * m_Scaling), m_Position.y + 100.f, m_Button->GetWidth() * m_Scaling, m_Button->GetHeight() * m_Scaling };

	m_Flag->Draw(m_FlagRect);

	if (m_HoverFlag) {
		m_Button->Draw(button);
		m_ButtonText->Draw(Point2f{ m_Position.x + 47.f, m_Position.y + 125.f });
	}

}

void Flag::FlagCollisions(Rectf playerHitbox)
{
	//Player hitbox
	Point2f bottemLeft{ playerHitbox.left, playerHitbox.bottom };
	Point2f topLeft{ playerHitbox.left, playerHitbox.bottom + playerHitbox.height };
	Point2f bottemRight{ playerHitbox.left + playerHitbox.width, playerHitbox.bottom };
	Point2f topRight{ playerHitbox.left + playerHitbox.width,  playerHitbox.bottom + playerHitbox.height };

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	bool pressE{ bool(pStates[SDL_SCANCODE_E]) };

	if (IsPointInRect(bottemLeft,	m_FlagRect) || 
		IsPointInRect(topLeft,		m_FlagRect) ||
		IsPointInRect(bottemRight,	m_FlagRect) || 
		IsPointInRect(topRight,		m_FlagRect)) {
		m_HoverFlag = true;
	}
	else m_HoverFlag = false;

	if (m_HoverFlag && pressE) m_WonGame = true;
}

bool Flag::GameWon()
{
	return m_WonGame;
}
