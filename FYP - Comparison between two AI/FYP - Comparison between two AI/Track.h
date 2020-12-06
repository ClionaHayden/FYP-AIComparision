#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>

#include "Boundry.h"
#include "BoundryData.h"

using namespace sf;

class Track
{
public:
	Track();
	~Track();
	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);
private:
	Sprite m_sprite;
	Texture m_tex;
	std::vector<Boundry> m_boundries;
	boundryData m_boundryData;
	bool m_drawBoundry;

	void setup();
	void setupBoundries();
};