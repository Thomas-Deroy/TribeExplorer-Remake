#include "pch.h"
#include "Map.h"
#include "iostream"
#include "utils.h"

using namespace utils;

Map::Map(const std::string& filePath, const std::string& imagePath)
	: m_Polygons(std::vector<std::vector<Point2f>>()), m_BackgroundPtr(nullptr)
{
	SVGParser::GetVerticesFromSvgFile(filePath, m_Polygons);
	m_BackgroundPtr = new Texture(imagePath);
	m_BackgroundCheck = true;
}

Map::Map(const std::string& filePath) : m_Polygons(std::vector<std::vector<Point2f>>())
{
	SVGParser::GetVerticesFromSvgFile(filePath, m_Polygons);
	m_BackgroundCheck = false;
}

Map::~Map()
{
	if (m_BackgroundCheck) delete m_BackgroundPtr;
	m_BackgroundPtr = nullptr;
}

void Map::Draw(bool debug) const
{
	if(m_BackgroundCheck) m_BackgroundPtr->Draw();

	// Return early if we are not in debug mode, this is called a guard clause
	if (!debug) 
	{
		return;
	}

	// Draw the collision shape
	//for (const std::vector<Point2f> poly : m_Polygons)
	//{
	//	DrawPolygon(poly);
	//}
}

const std::vector<std::vector<Point2f>>& Map::GetPolygons() const
{
	return m_Polygons;
}
