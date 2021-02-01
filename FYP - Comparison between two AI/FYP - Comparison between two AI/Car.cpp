#include "Car.h"
#define PI 3.14159265359

Car::Car(Vector2f t_startPos, float t_startSpeed) :
	m_pos(t_startPos),
	m_speed(t_startSpeed),
	m_rotation(10.0f),
	m_velocity(1.0f, 0.0f),
	m_reinforcement(Time::Zero),
	m_pastRscore(-100),
	m_cpNum(0),
	m_replay(false),
	m_current(0),
	m_pastDist(1000),
	m_CPTimer{Time::Zero}
{
	setup();
}

Car::~Car()
{
}

void Car::update(Time t_deltaTime)
{
	m_CPTimer += t_deltaTime;
	m_sprite.setRotation(m_rotation);
	m_sprite.setPosition(m_pos);
	move(t_deltaTime);
	updateColLines();
	m_reinforcement.update(t_deltaTime);
	if (!m_replay)
	{
		reinforcement();
	}
	else
	{
		replayLearning();
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
	if (Keyboard::isKeyPressed(Keyboard::R))
	{
		m_replay = true;
	}
	/*if(sf::Keyboard::A == e.key.code)
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
	}*/
}

void Car::reinforcement()
{
	if (m_reinforcement.getTimer() >= seconds(0.5f))
	{
		m_reinforcement.setTimer(Time::Zero);

		float distToCp = distance(m_pos, m_checkpoints.m_centres[m_cpNum]);
	/*	if (distToCp < m_pastDist)
		{
			m_reinforcement.praise(1000);
		}
		else
		{
			m_reinforcement.punish(1000);
		}*/
		//m_reinforcement.setScore(m_reinforcement.getScore() + (1000 - distToCp));
		if(m_reinforcement.getScore() < m_pastRscore)
		{
			m_pos = m_reinforcement.getData().back().m_position;
			m_rotation = m_reinforcement.getData().back().m_rotation;
			m_speed = m_reinforcement.getData().back().m_speed;
			m_speed = m_speed + ((rand() % (int)MAX_ACCELERATION) - (MAX_ACCELERATION / 2));
			if (m_speed > MAX_ACCELERATION)
				m_speed = MAX_ACCELERATION;
			if (m_speed < MIN_ACCELERATION)
				m_speed = MIN_ACCELERATION;
			m_rotation = (rand() % 180 - 90.0) + m_rotation;
			m_reinforcement.setScore(m_pastRscore);
		}
		else
		{
			m_pastDist = distToCp;
			//m_reinforcement.addData(m_rotation, m_pos, m_speed);
		}
		m_pastRscore = m_reinforcement.getScore();
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
		float angle =  (m_rotation + 180.0 + (45.0 * i)) * PI / 180;
		Vector2f vec = Vector2f(cos(angle), sin(angle));
		vec *= COLLISION_RADIUS;
		vec = m_pos + vec;
		m_collisionLines.push_back(vec);
	}

	m_reinforcement.addData(m_rotation, m_pos, m_speed);
}

void Car::move(Time t_deltaTime)
{
	float angle = (m_rotation - 90.0) * PI / 180;
	m_velocity = Vector2f(cos(angle), sin(angle));
	m_velocity = normalise(m_velocity);
	m_pos += m_velocity * m_speed * t_deltaTime.asSeconds();
	m_sprite.setPosition(m_pos);
}

void Car::updateColLines()
{
	for (int i = 0; i < MAX_COLLISIONS; i++)
	{
		float angle = (m_rotation + 180.0 + (45.0 * i)) * PI / 180;
		Vector2f vec = Vector2f(cos(angle), sin(angle));
		vec *= COLLISION_RADIUS;
		vec = m_pos + vec;
		m_collisionLines[i] = vec;
	}
}

void Car::collidesBoundary()
{
	m_reinforcement.punish(500);
}

void Car::collidesPassedCP()
{
	if (m_CPTimer > sf::seconds(1))
	{
		m_CPTimer = Time::Zero;
		m_reinforcement.punish(500);
	}
}

void Car::collidesCheckpoint()
{
	m_reinforcement.praise(200);
	m_reinforcement.addData(m_rotation, m_pos + Vector2f(m_velocity.x * (m_speed / 2), m_velocity.y * (m_speed / 2)), m_speed);
	m_pastDist = 1000;
}


Vector2f Car::normalise(Vector2f t_vec)
{
	float len = std::sqrt(t_vec.x * t_vec.x + t_vec.y * t_vec.y);
	return Vector2f(t_vec.x / len, t_vec.y / len);
}

float Car::distance(Vector2f t_vec1, Vector2f t_vec2)
{
	return sqrt(pow(t_vec2.x - t_vec1.x, 2) + pow(t_vec2.y - t_vec1.y, 2));
}

void Car::replayLearning()
{
	if (m_reinforcement.getTimer() >= seconds(0.5f))
	{
		m_reinforcement.setTimer(Time::Zero);
		m_pos = m_reinforcement.getData()[m_current].m_position;
		m_speed = m_reinforcement.getData()[m_current].m_speed;
		m_rotation = m_reinforcement.getData()[m_current].m_rotation;
		if (m_current < m_reinforcement.getData().size() - 1)
		{
			m_current++;
		}
	}
}
