#include "Boundry.h"

Boundry::Boundry(Vector2f t_pos, Vector2f t_size, float t_rot) :
	m_pos(t_pos),
	m_draw(true)
{
	setup(t_size, t_rot);
}

Boundry::~Boundry()
{
}

void Boundry::toggleDraw()
{
	if (m_draw)
	{
		m_draw = false;
	}
	else
	{
		m_draw = true;
	}
}

void Boundry::draw(RenderWindow& t_win)
{
	if (m_draw)
	{
		t_win.draw(m_boundry);
	}
}

void Boundry::setOrigin(sf::Vector2f t_new)
{
	m_boundry.setOrigin(t_new);
}

void Boundry::setup(Vector2f t_size, float t_rot)
{
	m_boundry.setSize(t_size);
	m_boundry.setPosition(m_pos);
	m_boundry.setRotation(t_rot);
	m_boundry.setFillColor(sf::Color::White);
}
