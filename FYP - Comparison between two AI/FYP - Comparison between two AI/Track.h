#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>

#include "Boundry.h"
#include "BoundryData.h"
#include "CheckpointData.h"
#include "Checkpoint.h"
#include "Car.h"
#include "Brain.h"

using namespace sf;

class Track
{
public:
	Track();
	~Track();
	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);
	void handleInput(Event& e);
private:
	Sprite m_sprite;
	Texture m_tex;
	std::vector<Boundry> m_boundries;
	boundryData m_boundryData;
	std::vector<std::shared_ptr<Checkpoint>> m_checkpoints;
	std::vector<CircleShape> m_colCirc;
	CheckpointData m_checkpointData;
	bool m_drawBoundry;

	Car m_car;

	shared_ptr<Brain> m_brain;
	vector<shared_ptr<float>> m_reinforcementInputs;

	sf::Time m_inputTimer;

	bool m_lap;

	void setup();
	void setupBoundries();
	void setupCheckpoints();
	void checkCarCollision();
	bool lineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int t_inputNum1, int t_inputNum2, int t_circNum, bool t_addInput);
	float lengthOfLine(Vector2f t_l1, Vector2f t_l2);
};