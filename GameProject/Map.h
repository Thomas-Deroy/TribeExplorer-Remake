#pragma once
#include "SVGParser.h"
#include "Texture.h"

// The map that should be loaded
class Map
{
public:
	explicit Map(const std::string& filePath, const std::string& imagePath);
	explicit Map(const std::string& filePath);
	virtual ~Map();

	//RULE OF 5
	Map(const Map& rhs) = delete;
	Map& operator=(const Map& rhs) = delete;

	Map(Map&& rhs) = delete;

	Map& operator=(Map&& rhs) = delete;

	// If debug is true, the collision polygons will also be drawn
	void Draw(bool debug = false) const;

	const std::vector<std::vector<Point2f>>& GetPolygons() const;
private:
	// The polygons to collide with
	std::vector<std::vector<Point2f>> m_Polygons;

	const Texture* m_BackgroundPtr;
	bool m_BackgroundCheck;
};

