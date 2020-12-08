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

	std::vector<Vector2f> m_collisionLines;
	const int MAX_COLLISIONS = 5;
	const float COLLISION_RADIUS = 40;

	const float RATE_OF_ROTATION = 3.0f;
	const float MASS = 1.0f;
	const float MAX_ACCELERATION = 100.0f;
	const float RATE_OF_ACCELERATION = 5.0f;

	void setup();
	void move(Time t_deltaTime);
	Vector2f normalise(Vector2f t_vec);
};