#include "Track.h"

Track::Track() :
	m_drawBoundry{ true }
{
	setup();
}

Track::~Track()
{
}

void Track::update(Time t_deltaTime)
{
}

void Track::render(RenderWindow& t_window)
{
	t_window.draw(m_sprite);
	if (m_drawBoundry)
	{
		for (Boundry b : m_boundries)
		{
			b.draw(t_window);
		}
	}
}

void Track::setup()
{
	if (!m_tex.loadFromFile("ASSETS/track.png"))
	{
		std::cout << "ERROR - Unable to laod track sprite" << std::endl;
	}
	m_sprite.setTexture(m_tex);

	setupBoundries();
}

void Track::setupBoundries()
{
	for (int i = 0; i < boundryData::MAX_ENTRIES; i++)
	{
		Boundry temp(m_boundryData.m_pos[i], m_boundryData.m_size[i], m_boundryData.m_rotation[i]);
		if (m_boundryData.m_rotation[i] != 0.0f)
		{
			temp.setOrigin(Vector2f(m_boundryData.m_size[i].x / 2.0f, m_boundryData.m_size[i].y / 2.0f));
		}
		m_boundries.push_back(temp);
	}
}
