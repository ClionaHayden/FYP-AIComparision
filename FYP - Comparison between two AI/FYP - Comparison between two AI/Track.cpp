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
	m_brain->m_PastReinforcementscore = m_brain->m_Reinforcementscore;
	checkCarCollision();
	if (s_gameState == GameState::LoadWeights || s_gameState == GameState::Reinforcement)
	{
		pair<vector<shared_ptr<float>>, bool> eval = m_brain->Evaluate(m_reinforcementInputs);
		if (!eval.second)
		{
			vector<shared_ptr<float>> outputs = eval.first;
			m_car.processOutputs(outputs);
		}
		else
		{
			m_car.reset();
		}
	}
	m_car.update(t_deltaTime);
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
		for (CircleShape c : m_colCirc)
		{
			t_window.draw(c);
		}
	}
	m_car.render(t_window, m_drawBoundry);
}

void Track::handleInput(Event& e)
{
	m_car.handleInput(e,m_reinforcementInputs);
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

	m_brain = make_shared<Brain>();

	for (int i = 0; i < m_brain->numInputs; i++)
	{
		m_reinforcementInputs.push_back(make_shared<float>(0));
	}
	for (int i = 0; i < 5; i++)
	{
		CircleShape c;
		c.setRadius(5);
		c.setFillColor(Color::Blue);
		c.setOrigin(c.getRadius(), c.getRadius());
		m_colCirc.push_back(c);
	}
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
		int inputNum = 0;
		for (int i = 0; i < m_car.getColLines().size(); i++)
		{
			// add inputs to neural network
			Vector2f c = m_car.getColLines().at(i);
			bool left = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, true);
			bool right = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x + b.getBounds().width, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, true);
			bool top = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y, inputNum, inputNum + 1, i, true);
			bool bottom = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y + b.getBounds().height, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, true);
			inputNum += 2;
			if (m_car.getSprite().getGlobalBounds().intersects(b.getBounds()))//left
			{
				m_car.collidesBoundary(m_brain);
				//m_car.push(Vector2f(-1, 0));
			}
			if (right)
			{
				//m_car.collidesBoundary(make_shared<Brain>(m_brain));
				//m_car.push(Vector2f(1, 0));
			}
			if (top)
			{
				//m_car.collidesBoundary(make_shared<Brain>(m_brain));
				//m_car.push(Vector2f(0, -1));
			}
			if (bottom)
			{
				//m_car.collidesBoundary(make_shared<Brain>(m_brain));
				//m_car.push(Vector2f(0, 1));
			}
		}
	}
	for (std::shared_ptr<Checkpoint> cp : m_checkpoints)
	{
		int inputNum = 0;
		for (int i = 0; i < m_car.getColLines().size(); i++)
		{
			Vector2f c = m_car.getColLines().at(i);
			bool left = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x, cp->getPos().y, cp->getPos().x, cp->getPos().y + cp->getBounds().height, inputNum, inputNum+1, i, false);
			bool right = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x + cp->getBounds().width, cp->getPos().y, cp->getPos().x + cp->getBounds().width, cp->getPos().y + cp->getBounds().height, inputNum, inputNum + 1, i, false);
			bool top = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x, cp->getPos().y, cp->getPos().x + cp->getBounds().width, cp->getPos().y, inputNum, inputNum + 1, i, false);
			bool bottom = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, cp->getPos().x, cp->getPos().y + cp->getBounds().height, cp->getPos().x + cp->getBounds().width, cp->getPos().y + cp->getBounds().height, inputNum, inputNum + 1, i, false);
			inputNum += 2;
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

bool Track::lineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int t_inputNum1, int t_inputNum2, int t_circNum, bool t_addInput)
{
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		if (t_addInput)
		{
			float intersectionX = x1 + (uA * (x2 - x1));
			float intersectionY = y1 + (uA * (y2 - y1));
			float input = lengthOfLine(Vector2f(x1, y1), Vector2f(intersectionX, intersectionY));
			m_colCirc.at(t_circNum).setPosition(intersectionX, intersectionY);
			m_reinforcementInputs.at(t_inputNum1) = make_shared<float>(intersectionX);
			m_reinforcementInputs.at(t_inputNum2) = make_shared<float>(intersectionY);
		}
		return true;
	}
	return false;
}

float Track::lengthOfLine(Vector2f t_l1, Vector2f t_l2)
{
	float sq = pow(t_l2.x - t_l1.x, 2) + pow(t_l2.y - t_l1.y, 2);
	return sqrt(sq);
}

