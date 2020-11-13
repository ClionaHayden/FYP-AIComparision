#include "Track.h"

Track::Track()
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
}

void Track::setup()
{
	if (!m_tex.loadFromFile("ASSETS/track.png"))
	{
		std::cout << "ERROR - Unable to laod track sprite" << std::endl;
	}
	m_sprite.setTexture(m_tex);
}
