#include "Checkpoint.h"

Checkpoint::Checkpoint(Vector2f t_pos, Vector2f t_size):
	m_pos(t_pos),
	m_draw(true),
	m_passed(false)
{
	setup(t_size);
}

Checkpoint::~Checkpoint()
{
}

void Checkpoint::toggleDraw()
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

void Checkpoint::draw(RenderWindow& t_win)
{
	if (m_draw)
	{
		if (m_passed)
		{
			m_checkpoint.setFillColor(sf::Color::Blue);
		}
		else
		{
			m_checkpoint.setFillColor(sf::Color::Green);
		}
		t_win.draw(m_checkpoint);
	}
}

void Checkpoint::setOrigin(sf::Vector2f t_new)
{
	m_checkpoint.setOrigin(t_new);
}

void Checkpoint::setup(Vector2f t_size)
{
	m_checkpoint.setSize(t_size);
	m_checkpoint.setPosition(m_pos);
	m_checkpoint.setFillColor(sf::Color::Green);
}
