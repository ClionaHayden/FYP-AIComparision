#include "Track.h"
#define PI 3.14159265359

Track::Track() :
	m_drawBoundry{ false },
	m_car{ Vector2f(100.0f, 450.0f), 50.0f },
	m_ReinforcementCar{ Vector2f(150.0f, 450.0f), 50.0f },
	m_inputTimer{ sf::Time::Zero },
	m_lap{ false },
	m_lapNum{ 0 }
{
	setup();
}

Track::~Track()
{
}

void Track::update(Time t_deltaTime)
{
	if (m_inputTimer > seconds(0.2))
	{
		raycast();
		m_inputTimer = Time::Zero;
	}
	m_car.handleInput(m_inputs);
	m_car.update(t_deltaTime);

	m_inputTimer += t_deltaTime;
	if (s_gameState == GameState::LoadWeights)
	{
		m_ReinforcementCar.handleInput(m_inputs);
		m_ReinforcementCar.update(t_deltaTime);
		pair<vector<shared_ptr<float>>, bool> eval = m_brain->Evaluate(m_inputs,false);
		if (!eval.second)
		{
			vector<shared_ptr<float>> outputs = eval.first;
			m_car.processOutputs(outputs);
			m_car.update(t_deltaTime);
		}
		eval = m_brain->Evaluate(m_inputs, true);
		if (!eval.second)
		{
			vector<shared_ptr<float>> outputs = eval.first;
			m_ReinforcementCar.processOutputs(outputs);
			m_ReinforcementCar.update(t_deltaTime);
		}
	}
	else if (s_gameState == GameState::Reinforcement)
	{
		if (m_car.getCpNum() == 1)
		{
			int x;
			x = 1;
		}
		pair<vector<shared_ptr<float>>, bool> eval = m_brain->Evaluate(m_inputs,false);
		vector<shared_ptr<float>> outputs = eval.first;
		m_car.processOutputs(outputs);
		m_car.update(t_deltaTime);
		m_brain->m_PastReinforcementscore = m_brain->m_Reinforcementscore;
		m_brain->m_Reinforcementscore = calculateReinforcmentScore();
		if(m_brain->m_PastReinforcementscore < m_brain->m_Reinforcementscore)
		{
			m_brain->adjustWeights(outputs);
			m_car.reset();
			m_brain->m_Reinforcementscore = 10000;
			m_brain->m_PastReinforcementscore = 10000;
		}
	}
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
		for (CircleShape c : m_colCirc)
		{
			t_window.draw(c);
		}
		for (CircleShape c : m_colCircReinforcment)
		{
			t_window.draw(c);
		}
	}
	m_car.render(t_window, m_drawBoundry);

	sf::VertexArray line(sf::LinesStrip, 2);
	line[0].position = m_car.getPos();
	line[0].color = sf::Color::Black;
	line[1].position = m_checkpoints.at(m_car.getCpNum())->getPos();
	line[1].color = sf::Color::Black;
	t_window.draw(line);
	if (s_gameState == GameState::LoadWeights)
	{
		m_ReinforcementCar.render(t_window, m_drawBoundry);
	}
}

void Track::handleInput(Event& e)
{
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
	if (!m_tex.loadFromFile("ASSETS/track2.png"))
	{
		std::cout << "ERROR - Unable to laod track sprite" << std::endl;
	}
	m_sprite.setTexture(m_tex);
	m_car.changeColor(Color::Cyan);
	m_ReinforcementCar.changeColor(Color::Red);

	setupBoundries();
	setupCheckpoints();

	m_brain = make_shared<Brain>();

	for (int i = 0; i < m_brain->numInputs; i++)
	{
		m_inputs.push_back(make_shared<float>(0));
		m_ReinforcementInputs.push_back(make_shared<float>(0));
	}
	for (int i = 0; i < 5; i++)
	{
		CircleShape c;
		c.setRadius(5);
		c.setFillColor(Color::Blue);
		c.setOrigin(c.getRadius(), c.getRadius());
		m_colCirc.push_back(c);
		m_colCircReinforcment.push_back(c);
	}
}

int Track::calculateReinforcmentScore()
{
	m_brain->m_PastReinforcementscore = m_brain->m_Reinforcementscore;
	int score = 0;
	score = lengthOfLine(m_checkpoints[m_car.getCpNum()]->getPos(), m_car.getPos());
	int nextcp = m_car.getCpNum() + 1;
	for (int i = m_car.getCpNum(); i < m_checkpoints.size(); i++)
	{
		score += lengthOfLine(m_checkpoints[i]->getPos(), m_checkpoints[nextcp]->getPos());
		nextcp++;
		if (nextcp >= m_checkpoints.size())
			break;
	}
	return score;
}

void Track::setupBoundries()
{
	for (int i = 0; i < boundryData2::MAX_ENTRIES; i++)
	{
		Boundry temp(m_boundryData2.m_pos[i], m_boundryData2.m_size[i], m_boundryData2.m_rotation[i]);
		if (m_boundryData2.m_rotation[i] != 0.0f)
		{
			temp.setOrigin(Vector2f(m_boundryData2.m_size[i].x / 2.0f, m_boundryData2.m_size[i].y / 2.0f));
		}
		m_boundries.push_back(temp);
	}
}

void Track::setupCheckpoints()
{
	for (int i = 0; i < CheckpointData2::MAX_ENTRIES; i++)
	{
		std::shared_ptr<Checkpoint>temp = std::make_shared<Checkpoint>(m_checkpointData2.m_pos[i], m_checkpointData2.m_size[i]);
		temp->setOrigin(Vector2f(m_checkpointData2.m_size[i].x / 2.0f, m_checkpointData2.m_size[i].y / 2.0f));
		m_checkpoints.push_back(temp);
	}
}

void Track::checkCarCollision()
{
	//--------------------
	// Backprop Car
	//--------------------
	for (Boundry b : m_boundries)
	{
		int inputNum = 0;
		for (int i = 0; i < m_car.getColLines().size(); i++)
		{
			// add inputs to neural network
			Vector2f c = m_car.getColLines().at(i);
			bool left = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, true);
			bool right = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x + b.getBounds().width, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, true);
			bool top = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y, inputNum, inputNum + 1, i, false, true);
			bool bottom = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y + b.getBounds().height, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, true);
			inputNum += 2;
			if (left/*m_car.getSprite().getGlobalBounds().intersects(b.getBounds())*/)//left
			{
				m_car.collidesBoundary(m_brain);
				m_car.push(Vector2f(-1, 0));
			}
			if (right)
			{
				m_car.collidesBoundary(m_brain);
				m_car.push(Vector2f(1, 0));
			}
			if (top)
			{
				m_car.collidesBoundary(m_brain);
				m_car.push(Vector2f(0, -1));
			}
			if (bottom)
			{
				m_car.collidesBoundary(m_brain);
				m_car.push(Vector2f(0, 1));
			}
		}
	}
	if (m_car.getBounds().intersects(m_checkpoints.at(m_car.getCpNum())->getBounds())/*rectCollision(m_car.getBounds(), m_checkpoints.at(m_car.getCpNum())->getBounds(), m_car.getPos(), m_checkpoints.at(m_car.getCpNum())->getPos())*/)
	{
		m_checkpoints.at(m_car.getCpNum())->setPassed(true);
		m_car.nextCP();
		if (m_car.getCpNum() == m_checkpoints.size())
		{
			for (std::shared_ptr<Checkpoint> c : m_checkpoints)
			{
				c->setPassed(false);
			}
			m_car.setCPNum(0);
			m_lapNum++;
		}
		if (m_lapNum == 3)
		{
			if (s_gameState == GameState::TrainingDataCollection)
			{
				m_car.saveTrainingDataToFile();
				s_gameState = GameState::Success;

			}
		}
	}

	//--------------------
	// Reinforcement Car
	//--------------------
	if (s_gameState == GameState::LoadWeights)
	{
		for (Boundry b : m_boundries)
		{
			int inputNum = 0;
			for (int i = 0; i < m_ReinforcementCar.getColLines().size(); i++)
			{
				// add inputs to neural network
				Vector2f c = m_ReinforcementCar.getColLines().at(i);
				bool left = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, false);
				bool right = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x + b.getBounds().width, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, false);
				bool top = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y, inputNum, inputNum + 1, i, false, false);
				bool bottom = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y + b.getBounds().height, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, false);
				inputNum += 2;
				if (left/*m_car.getSprite().getGlobalBounds().intersects(b.getBounds())*/)//left
				{
					m_ReinforcementCar.collidesBoundary(m_brain);
					m_ReinforcementCar.push(Vector2f(-1, 0));
				}
				if (right)
				{
					m_ReinforcementCar.collidesBoundary(m_brain);
					m_ReinforcementCar.push(Vector2f(1, 0));
				}
				if (top)
				{
					m_ReinforcementCar.collidesBoundary(m_brain);
					m_ReinforcementCar.push(Vector2f(0, -1));
				}
				if (bottom)
				{
					m_ReinforcementCar.collidesBoundary(m_brain);
					m_ReinforcementCar.push(Vector2f(0, 1));
				}
			}
		}
	}
}

void Track::raycast()
{
	//--------------------
	// Backprop Car
	//--------------------
	int inputNum = 0;
	for (int i = 0; i < m_car.getRays().size(); i++)
	{
		m_car.collLineLengths[i] = 5.0f;
		bool left = false;
		bool right = false;
		bool top = false;
		bool bottom = false;
		Boundry bound;
		while (!left && !right && !top && !bottom)
		{
				m_car.collLineLengths[i] += 5.0f;
				m_car.updateColLines();
				for (Boundry b : m_boundries)
				{
					Vector2f c = m_car.getRays().at(i);
					left = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false,true);
					right = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x + b.getBounds().width, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, true);
					top = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y, inputNum, inputNum + 1, i, false, true);
					bottom = lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y + b.getBounds().height, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, true);
					if (left || right || top || bottom)
					{
						if (m_car.collLineLengths[i] < 200.0f)
						{
							bound = b;
							break;
						}
						else
						{
							m_car.collLineLengths[i] = 200.0f;
							bound = b;
							break;
						}
					}
				
			}
		}
		Vector2f c = m_car.getRays().at(i);
		lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, bound.getPos().x, bound.getPos().y, bound.getPos().x, bound.getPos().y + bound.getBounds().height, inputNum, inputNum + 1, i, true, true);
		lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, bound.getPos().x + bound.getBounds().width, bound.getPos().y, bound.getPos().x + bound.getBounds().width, bound.getPos().y + bound.getBounds().height, inputNum, inputNum + 1, i, true, true);
		lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, bound.getPos().x, bound.getPos().y, bound.getPos().x + bound.getBounds().width, bound.getPos().y, inputNum, inputNum + 1, i, true, true);
		lineCollision(m_car.getPos().x, m_car.getPos().y, c.x, c.y, bound.getPos().x, bound.getPos().y + bound.getBounds().height, bound.getPos().x + bound.getBounds().width, bound.getPos().y + bound.getBounds().height, inputNum, inputNum + 1, i, true, true);
		inputNum += 2;
	}
	//--------------------
	// Reinforcement Car
	//--------------------
	if (s_gameState == GameState::LoadWeights)
	{
		inputNum = 0;
		for (int i = 0; i < m_ReinforcementCar.getRays().size(); i++)
		{
			m_ReinforcementCar.collLineLengths[i] = 5.0f;
			bool left = false;
			bool right = false;
			bool top = false;
			bool bottom = false;
			Boundry bound;
			while (!left && !right && !top && !bottom)
			{
				m_ReinforcementCar.collLineLengths[i] += 5.0f;
				m_ReinforcementCar.updateColLines();
				for (Boundry b : m_boundries)
				{
					Vector2f c = m_ReinforcementCar.getRays().at(i);
					left = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, false);
					right = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x + b.getBounds().width, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, false);
					top = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y, b.getPos().x + b.getBounds().width, b.getPos().y, inputNum, inputNum + 1, i, false, false);
					bottom = lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, b.getPos().x, b.getPos().y + b.getBounds().height, b.getPos().x + b.getBounds().width, b.getPos().y + b.getBounds().height, inputNum, inputNum + 1, i, false, false);
					if (left || right || top || bottom)
					{
						if (m_ReinforcementCar.collLineLengths[i] < 200.0f)
						{
							bound = b;
							break;
						}
						else
						{
							m_ReinforcementCar.collLineLengths[i] = 200.0f;
							bound = b;
							break;
						}
					}

				}
			}
			Vector2f c = m_ReinforcementCar.getRays().at(i);
			lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, bound.getPos().x, bound.getPos().y, bound.getPos().x, bound.getPos().y + bound.getBounds().height, inputNum, inputNum + 1, i, true, false);
			lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, bound.getPos().x + bound.getBounds().width, bound.getPos().y, bound.getPos().x + bound.getBounds().width, bound.getPos().y + bound.getBounds().height, inputNum, inputNum + 1, i, true, false);
			lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, bound.getPos().x, bound.getPos().y, bound.getPos().x + bound.getBounds().width, bound.getPos().y, inputNum, inputNum + 1, i, true, false);
			lineCollision(m_ReinforcementCar.getPos().x, m_ReinforcementCar.getPos().y, c.x, c.y, bound.getPos().x, bound.getPos().y + bound.getBounds().height, bound.getPos().x + bound.getBounds().width, bound.getPos().y + bound.getBounds().height, inputNum, inputNum + 1, i, true, false);
			inputNum += 2;
		}
	}
}

bool Track::lineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int t_inputNum1, int t_inputNum2, int t_circNum, bool t_addInput,bool t_backprop)
{
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		if (t_addInput)
		{
			float intersectionX = x1 + (uA * (x2 - x1));
			float intersectionY = y1 + (uA * (y2 - y1));
			float input = lengthOfLine(Vector2f(x1, y1), Vector2f(intersectionX, intersectionY));
			if (t_backprop)
			{
				m_colCirc.at(t_circNum).setPosition(intersectionX, intersectionY);
				m_inputs.at(t_inputNum1) = make_shared<float>(intersectionX);
				m_inputs.at(t_inputNum2) = make_shared<float>(intersectionY);
			}
			else
			{
				m_colCircReinforcment.at(t_circNum).setPosition(intersectionX, intersectionY);
				m_ReinforcementInputs.at(t_inputNum1) = make_shared<float>(intersectionX);
				m_ReinforcementInputs.at(t_inputNum2) = make_shared<float>(intersectionY);
			}
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

bool Track::rectCollision(FloatRect t_rect1, FloatRect t_rect2, Vector2f t_pos1, Vector2f t_pos2)
{
	if (t_pos1.x - (t_rect1.width / 2.0f) > t_pos2.x - (t_rect2.width / 2.0f) && t_pos1.x  + (t_rect1.width / 2.0f) < t_pos2.x + (t_rect2.width / 2.0f) &&
		t_pos1.y - (t_rect1.height / 2.0f) > t_pos2.y - (t_rect2.height / 2.0f) && t_pos1.y + (t_rect1.width / 2.0f) < t_pos2.y + (t_rect2.height / 2.0f))
		return true;
	return false;
}

