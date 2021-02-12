#include "Track.h"
#define PI 3.14159265359

Track::Track() :
	m_drawBoundry{ false },
	m_car{ Vector2f(100.0f, 450.0f), 50.0f },
	m_inputTimer{ sf::Time::Zero },
	m_lap{ false }
{
	setup();
}

Track::~Track()
{
}

void Track::update(Time t_deltaTime)
{
	m_inputTimer += t_deltaTime;
	m_car.update(t_deltaTime);
	checkCarCollision();
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
		for (std::shared_ptr<Checkpoint> c : m_checkpoints)
		{
			c->draw(t_window);
		}
	}
	m_car.render(t_window, m_drawBoundry);
}

void Track::handleInput(Event& e)
{
	m_car.handleInput(e);
	if (m_inputTimer.asSeconds() > 0.1)
	{
		if (sf::Keyboard::B == e.key.code)
		{
			if (m_drawBoundry) { m_drawBoundry = false; }
			else { m_drawBoundry = true; }
		}
		m_inputTimer = sf::Time::Zero;
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
	setupCheckpoints();
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

void Track::setupCheckpoints()
{
	for (int i = 0; i < CheckpointData::MAX_ENTRIES; i++)
	{
		std::shared_ptr<Checkpoint>temp = std::make_shared<Checkpoint>(m_checkpointData.m_pos[i], m_checkpointData.m_size[i]);
		temp->setOrigin(Vector2f(m_checkpointData.m_size[i].x / 2.0f, m_checkpointData.m_size[i].y / 2.0f));
		m_checkpoints.push_back(temp);
	}
}

void Track::checkCarCollision()
{
	for (Boundry b : m_boundries)
	{
		for (Vector2f c : m_car.getColLines())
		{
			bool left = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x, b.getPos().y + b.getBounds().height);
			bool right = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x + b.getBounds().width, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height);
			bool top = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y);
			bool bottom = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y + b.getBounds().height, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height);
			if (left)
			{
				m_car.collidesBoundary();
				m_car.push(Vector2f(-1, 0));
			}
			if (right)
			{
				m_car.collidesBoundary();
				m_car.push(Vector2f(1, 0));
			}
			if (top)
			{
				m_car.collidesBoundary();
				m_car.push(Vector2f(0, -1));
			}
			if (bottom)
			{
				m_car.collidesBoundary();
				m_car.push(Vector2f(0, 1));
			}
		}
	}
	for (std::shared_ptr<Checkpoint> cp : m_checkpoints)
	{
		for (Vector2f c : m_car.getColLines())
		{
			bool left = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x, cp->getPos().y, cp->getPos().x, cp->getPos().y + cp->getBounds().height);
			bool right = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x + cp->getBounds().width, cp->getPos().y, cp->getPos().x + cp->getBounds().width, cp->getPos().y + cp->getBounds().height);
			bool top = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x, cp->getPos().y, cp->getPos().x + cp->getBounds().width, cp->getPos().y);
			bool bottom = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x, cp->getPos().y + cp->getBounds().height, cp->getPos().x + cp->getBounds().width, cp->getPos().y + cp->getBounds().height);
			if (left || right || top || bottom)
			{
				if ((!cp->getPassed()) /*&& m_checkpoints.size() > m_car.getCpNum()*/)
				{
					m_car.collidesCheckpoint();
					m_car.nextCP();
					cp->setPassed(true);
					if (cp == m_checkpoints[m_checkpoints.size() - 2]) 
					{
						m_lap = true;
					}
				}
				else
				{
					if (s_gameState == GameState::TrainingDataCollection)
					{
						if (cp == m_checkpoints[0] && cp->getPassed() && m_lap == true)
						{
							m_car.saveTrainingDataToFile();
							s_gameState = GameState::Success;
						}
					}
					m_car.collidesPassedCP();
				}
			}
		}
	}
}

bool Track::lineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		return true;
	}
	return false;
}

