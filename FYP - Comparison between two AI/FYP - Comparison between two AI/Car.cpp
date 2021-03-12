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
	m_CPTimer(Time::Zero),
	m_restartPos(t_startPos),
	m_restartSpeed(t_startSpeed)
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
	move(t_deltaTime);
	updateColLines();
	if (s_gameState == GameState::Reinforcement)
	{
		m_reinforcement.update(t_deltaTime);
		if (!m_replay)
		{
			//reinforcement();
		}
		else
		{
			replayLearning();
		}
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

void Car::handleInput(Event& e, vector<shared_ptr<float>> t_inputs)
{
	if (Keyboard::isKeyPressed(Keyboard::R))
	{
		m_replay = true;
	}
	if (s_gameState == GameState::TrainingDataCollection)
	{
		TrainingData temp;
		temp.leftLen = *t_inputs.at(0);
		temp.topLeftLen = *t_inputs.at(1);
		temp.topLen = *t_inputs.at(2);
		temp.topRightLen = *t_inputs.at(3);
		temp.rightLen = *t_inputs.at(4);
		if(sf::Keyboard::A == e.key.code)
		{
			turnLeft();
			temp.left = true;
		}
		if (sf::Keyboard::D == e.key.code)
		{
			turnRight();
			temp.right = true;
		}
		if (sf::Keyboard::W == e.key.code)
		{
			temp.up = true;
			accelerate();
		}
		if (sf::Keyboard::S == e.key.code)
		{
			temp.down = true;
			decelerate();
		}
		m_backprop.addTrainingData(temp);
	}
}

void Car::reinforcement()
{
	if (m_reinforcement.getTimer() >= seconds(0.5f))
	{
		m_reinforcement.setTimer(Time::Zero);

		float distToCp = distance(m_pos, m_checkpoints.m_centres[m_cpNum]);
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
	if (s_gameState == GameState::Reinforcement)
	{
		m_reinforcement.addData(m_rotation, m_pos, m_speed);
	}
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

void Car::collidesBoundary(shared_ptr<Brain> t_brain)
{
	if (s_gameState == GameState::Reinforcement)
	{
		t_brain->m_Reinforcementscore -= 10000;
	}
}

void Car::collidesPassedCP()
{
	if (m_CPTimer > sf::seconds(1))
	{
		m_CPTimer = Time::Zero;
		if (s_gameState == GameState::Reinforcement)
		{
			m_reinforcement.punish(500);
		}
	}
}

void Car::collidesCheckpoint()
{
	if (s_gameState == GameState::Reinforcement)
	{
		m_reinforcement.praise(200);
		m_reinforcement.addData(m_rotation, m_pos + Vector2f(m_velocity.x * (m_speed / 2), m_velocity.y * (m_speed / 2)), m_speed);
		m_pastDist = 1000;
	}
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

void Car::turnLeft()
{
	m_rotation -= RATE_OF_ROTATION;
}

void Car::turnRight()
{
	m_rotation += RATE_OF_ROTATION;
}

void Car::accelerate()
{
	if (m_speed < MAX_ACCELERATION)
	{
		m_speed += RATE_OF_ACCELERATION;
	}
}

void Car::decelerate()
{
	if (m_speed > 0.0f)
	{
		m_speed -= RATE_OF_ACCELERATION;
	}
}

void Car::saveTrainingDataToFile()
{
	std::ofstream myfile;
	myfile.open("DATA/TrainingData.csv");
	for (int i = 0; i < m_backprop.getTrainingData().size(); i++)
	{
		myfile << m_backprop.getTrainingData()[i].leftLen << "," <<  m_backprop.getTrainingData()[i].topLeftLen << "," << m_backprop.getTrainingData()[i].topLen << ","
			<< m_backprop.getTrainingData()[i].topRightLen << "," << m_backprop.getTrainingData()[i].rightLen << "," << m_backprop.getTrainingData()[i].left << ","
			<< m_backprop.getTrainingData()[i].right<< "," << m_backprop.getTrainingData()[i].up<< "," << m_backprop.getTrainingData()[i].down  << "\n";
	}
	myfile.close();
}

void Car::processOutputs(vector<shared_ptr<float>> t_outputs)
{
	if (*t_outputs.at(0) == 1.0f)
	{
		turnLeft();
	}
	if (*t_outputs.at(1) == 1.0f)
	{
		turnRight();
	}
	if (*t_outputs.at(2) == 1.0f)
	{
		accelerate();
	}
	if (*t_outputs.at(3) == 1.0f)
	{
		decelerate();
	}
}

void Car::reset()
{
	m_pos = m_restartPos;
	m_rotation = 10.0f;
	m_speed = m_restartSpeed;
	m_velocity = Vector2f(1.0f, 0.0f);
	m_cpNum = 0;
}

void Car::setColLineLength(vector<float> t_lengths)
{
	for (int i = 0; i < 5; i++)
	{
		collLineLengths[i] = t_lengths.at(i);
	}
}
