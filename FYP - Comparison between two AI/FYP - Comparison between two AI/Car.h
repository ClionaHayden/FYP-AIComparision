#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

using namespace sf;

class Car
{
public:
	Car(Vector2f t_startPos, float t_startSpeed);
	~Car();
	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);
private:
	Sprite m_sprite;
	Texture m_tex;

	Vector2f m_pos;
	float m_speed;

	void setup();
};