#include "pch.h"
#include "Sprite.h"
#include "iostream"

Sprite::Sprite(const std::string& texturePath, int rows, int columns, float frameDelay, float scaler) :
	m_Rows{ rows }, m_Columns{ columns }, m_FrameDelay{ frameDelay }, m_Scaler{ scaler }
{
	m_Texture = new Texture(texturePath);
	m_FrameWidth = m_Texture->GetWidth() / m_Columns;
	m_FrameHeight = m_Texture->GetHeight() / m_Rows;
}

Sprite::~Sprite()
{
	delete m_Texture;
	m_Texture = nullptr;
}

//Default drawing function
void Sprite::Draw(const Rectf& dstRect, bool flipped, bool upsidedown) const
{
	const int	rowIndex{ m_CurrentFrame / m_Rows }, colIndex{ m_CurrentFrame % m_Columns };
	const Rectf sourceRect{ float(colIndex * m_FrameWidth), float(rowIndex * m_FrameHeight), m_FrameWidth, m_FrameHeight };

	//Flipping
	glPushMatrix();
		if (flipped) {
			glTranslatef(dstRect.left, dstRect.bottom + sourceRect.height, 0.f);
			glScalef(-1, 1, 1);
			glTranslatef(-dstRect.left - sourceRect.width * m_Scaler, -dstRect.bottom - sourceRect.height, 0.f);
		}

		if (upsidedown) {
			glTranslatef(dstRect.left, dstRect.bottom + sourceRect.height * m_Scaler, 0.f);
			glScalef(1, -1, 1);
			glTranslatef(-dstRect.left, -dstRect.bottom, 0.f);
		}

		m_Texture->Draw(dstRect, sourceRect);
	glPopMatrix();
}

//Converting Point2f to Rect so it can use rect drawing function
void Sprite::Draw(const Point2f& position, bool flipped, bool upsidedown) const
{
	const Rectf dstRect{ position.x, position.y, m_Scaler, m_Scaler};

	Draw(dstRect, flipped);
}

void Sprite::Update(float elapsedSec)
{
	if (m_AccumulatedTime >= m_FrameDelay)
	{
		m_AccumulatedTime -= m_FrameDelay;
		++m_CurrentFrame;
		if (m_CurrentFrame >= m_Rows * m_Columns) m_CurrentFrame = 0;
	}

	m_AccumulatedTime += elapsedSec;
}

Point2f Sprite::GetSize() const
{
	Point2f size{ m_FrameWidth * m_Scaler, m_FrameHeight * m_Scaler };
	return size;
}