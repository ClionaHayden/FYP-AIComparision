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
	void handleInput(Event& e);
private:
	Sprite m_sprite;
	Texture m_tex;

	Vector2f m_pos;
	float m_rotation;
	float m_speed;
	Vector2f m_velocity;
	Vector2f m_forward;

	const float RATE_OF_ROTATION = 1.0f;
	const float MASS = 1.0f;
	const float MAX_ACCELERATION = 20.0f;
	const float RATE_OF_ACCELERATION = 1.0f;

	void setup();
	void move(Time t_deltaTime);
	Vector2f normalise(Vector2f t_vec);
};