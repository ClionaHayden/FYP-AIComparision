#include "Car.h"
#define PI 3.14159265359

Car::Car(Vector2f t_startPos, float t_startSpeed) :
	m_pos(t_startPos),
	m_speed(t_startSpeed),
	m_rotation(10.0f),
	m_velocity(1.0f, 0.0f)
{
	setup();
}

Car::~Car()
{
}

void Car::update(Time t_deltaTime)
{
	m_sprite.setRotation(m_rotation);
	move(t_deltaTime);
	m_sprite.setPosition(m_pos);
}

void Car::render(RenderWindow& t_window)
{
	t_window.draw(m_sprite);
}

void Car::handleInput(Event& e)
{
	if(sf::Keyboard::A == e.key.code)
	{
		m_rotation -= RATE_OF_ROTATION;
	}
	if (sf::Keyboard::D == e.key.code)
	{
		m_rotation += RATE_OF_ROTATION;
	}
	if (sf::Keyboard::W == e.key.code)
	{
		if (m_speed < MAX_ACCELERATION)
		{
			m_speed += RATE_OF_ACCELERATION;
		}
	}
	if (sf::Keyboard::S == e.key.code)
	{
		if (m_speed > 0.0f)
		{
			m_speed -= RATE_OF_ACCELERATION;
		}
	}
}

void Car::setup()
{
	if (!m_tex.loadFromFile("ASSETS/car.png"))
	{
		std::cout << "ERROR - Unable to laod car sprite" << std::endl;
	}
	m_sprite.setTexture(m_tex);
	m_sprite.setPosition(m_pos);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
}

void Car::move(Time t_deltaTime)
{
	float angle = (m_rotation - 90) * PI / 180;
	m_velocity = Vector2f(cos(angle), sin(angle));
	m_velocity = normalise(m_velocity);
	m_pos += m_velocity * m_speed * t_deltaTime.asSeconds();
}

Vector2f Car::normalise(Vector2f t_vec)
{
	float len = std::sqrt(t_vec.x * t_vec.x + t_vec.y * t_vec.y);
	return Vector2f(t_vec.x / len, t_vec.y / len);
}
