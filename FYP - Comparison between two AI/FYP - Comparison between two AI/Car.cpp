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
	for (int i = 0; i < MAX_COLLISIONS; i++)
	{
		float angle = (m_rotation + 180 + (45 * i)) * PI / 180;
		Vector2f vec = Vector2f(cos(angle), sin(angle));
		vec *= COLLISION_RADIUS;
		vec = m_pos + vec;
		m_collisionLines[i] = vec;
	}
}

void Car::render(RenderWindow& t_window, bool t_debug)
{
	t_window.draw(m_sprite);
	if (t_debug)
	{
		for (int i = 0; i < MAX_COLLISIONS; i++)
		{
			sf::VertexArray line(sf::LinesStrip, 2);
			line[0].position = m_pos;
			line[0].color = sf::Color::Black;
			line[1].position = m_collisionLines[i];
			line[1].color = sf::Color::Black;
			t_window.draw(line);
		}
	}
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

	for (int i = 0; i < MAX_COLLISIONS; i++)
	{
		float angle =  (m_rotation + 180 + (45 * i)) * PI / 180;
		Vector2f vec = Vector2f(cos(angle), sin(angle));
		vec *= COLLISION_RADIUS;
		vec = m_pos + vec;
		m_collisionLines.push_back(vec);
	}
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
