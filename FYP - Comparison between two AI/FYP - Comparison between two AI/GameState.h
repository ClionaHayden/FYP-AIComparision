#pragma once
#include "SFML/Graphics.hpp"
using namespace sf;

enum class GameState
{
	MainMenu,
	ChoiceScreen,
	TrainingDataInstructions,
	TrainingDataCollection,
	Reinforcement,
	Backprop,
	Success,
	Fail,
	Race,
	Results,
	Exit,
	LoadWeights
};

struct Data
{
	float m_rotation;
	sf::Vector2f m_position;
	float m_speed;
};

struct TrainingData
{
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
	float x3 = 0;
	float y3 = 0;
	float x4 = 0;
	float y4 = 0;
	float x5 = 0;
	float y5 = 0;
	float posy = 0;
	float posx = 0;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool none = false;
};

extern GameState s_gameState;