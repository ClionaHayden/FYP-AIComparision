#include "Car.h"
#define PI 3.14159265359

Car::Car(Vector2f t_startPos, float t_startSpeed) :
	m_pos(t_startPos),
	m_speed(t_startSpeed),
	m_rotation(10.0f),
	m_velocity(1.0f, 0.0f),
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
			line[0].color = sf::Color::White;
			line[1].position = m_raycasts[i];
			line[1].color = sf::Color::White;
			t_window.draw(line);
		}
		for (int i = 0; i < MAX_COLLISIONS; i++)
		{
			sf::VertexArray line(sf::LinesStrip, 2);
			line[0].position = m_pos;
			line[0].color = sf::Color::Blue;
			line[1].position = m_collisionLines[i];
			line[1].color = sf::Color::Blue;
			t_window.draw(line);
		}
	}
}

void Car::handleInput(vector<shared_ptr<float>> t_inputs)
{
	if (Keyboard::isKeyPressed(Keyboard::R))
	{
		m_replay = true;
	}
	if (s_gameState == GameState::TrainingDataCollection)
	{
		TrainingData temp;
		temp.x1 = *t_inputs.at(0);
		temp.y1 = *t_inputs.at(1);
		temp.x2 = *t_inputs.at(2);
		temp.y2 = *t_inputs.at(3);
		temp.x3 = *t_inputs.at(4);
		temp.y3 = *t_inputs.at(5);
		temp.x4 = *t_inputs.at(6);
		temp.y4 = *t_inputs.at(7);
		temp.x5 = *t_inputs.at(8);
		temp.y5 = *t_inputs.at(9);
		temp.posx = m_sprite.getPosition().x;
		temp.posy = m_sprite.getPosition().y;
		temp.none = true;
		if(Keyboard::isKeyPressed(Keyboard::A))
		{
			turnLeft();
			temp.none = false;
			temp.left = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			turnRight();
			temp.none = false;
			temp.right = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			temp.up = true;
			temp.none = false;
			accelerate();
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			temp.down = true;
			temp.none = false;
			decelerate();
		}
		m_backprop.addTrainingData(temp);
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
		collLineLengths[i] = 0.0f;
		vec *= collLineLengths[i];
		vec = m_pos + vec;
		m_raycasts.push_back(vec);
	}
	for (int i = 0; i < MAX_COLLISIONS; i++)
	{
		float angle = (m_rotation + 180.0 + (45.0 * i)) * PI / 180;
		Vector2f vec = Vector2f(cos(angle), sin(angle));
		vec *= COL_RADIUS;
		vec = m_pos + vec;
		m_collisionLines.push_back(vec);
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
		vec *= collLineLengths[i];
		vec = m_pos + vec;
		m_raycasts[i] = vec;
	}
	for (int i = 0; i < MAX_COLLISIONS; i++)
	{
		float angle = (m_rotation + 180.0 + (45.0 * i)) * PI / 180;
		Vector2f vec = Vector2f(cos(angle), sin(angle));
		vec *= COL_RADIUS;
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
	}
}

void Car::collidesCheckpoint()
{
	if (s_gameState == GameState::Reinforcement)
	{
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
	if (m_speed > MIN_ACCELERATION)
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
		myfile << m_backprop.getTrainingData()[i].x1 << "," << m_backprop.getTrainingData()[i].y1
			<< "," << m_backprop.getTrainingData()[i].x2 << "," << m_backprop.getTrainingData()[i].y2
			<< "," << m_backprop.getTrainingData()[i].x3 << "," << m_backprop.getTrainingData()[i].y3 << ","
			<< m_backprop.getTrainingData()[i].x4 << "," << m_backprop.getTrainingData()[i].y4 << ","
			<< m_backprop.getTrainingData()[i].x5 << "," << m_backprop.getTrainingData()[i].y5 << ","
			<< m_backprop.getTrainingData()[i].posx << "," << m_backprop.getTrainingData()[i].posy << ","
			<< m_backprop.getTrainingData()[i].left << "," << m_backprop.getTrainingData()[i].right<< "," 
			<< m_backprop.getTrainingData()[i].up<< "," << m_backprop.getTrainingData()[i].down << "," 
			<< m_backprop.getTrainingData()[i].none << "\n";
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
