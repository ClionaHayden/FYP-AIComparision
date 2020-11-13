#include "Car.h"

Car::Car(Vector2f t_startPos, float t_startSpeed):
	m_pos(t_startPos),
	m_speed(t_startSpeed)
{
	setup();
}

Car::~Car()
{
}

void Car::update(Time t_deltaTime)
{
}

void Car::render(RenderWindow& t_window)
{
	t_window.draw(m_sprite);
}

void Car::setup()
{
	if (!m_tex.loadFromFile("ASSETS/car.png"))
	{
		std::cout << "ERROR - Unable to laod car sprite" << std::endl;
	}
	m_sprite.setTexture(m_tex);
	m_sprite.setPosition(m_pos);
}
