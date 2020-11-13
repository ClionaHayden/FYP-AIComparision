#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

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

	void setup();
};