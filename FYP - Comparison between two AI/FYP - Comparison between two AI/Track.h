#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>

#include "Boundry.h"
#include "BoundryData.h"
#include "Car.h"

using namespace sf;

class Track
{
public:
	Track();
	~Track();
	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);
	void handleInput(Event& e);
private:
	Sprite m_sprite;
	Texture m_tex;
	std::vector<Boundry> m_boundries;
	boundryData m_boundryData;
	bool m_drawBoundry;

	Car m_car;

	void setup();
	void setupBoundries();
	void checkCarCollision();
	bool lineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
};