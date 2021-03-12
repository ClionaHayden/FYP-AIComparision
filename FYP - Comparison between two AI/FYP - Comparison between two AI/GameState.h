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
	int leftLen = 0;
	int topLeftLen = 0;
	int topLen = 0;
	int topRightLen = 0;
	int rightLen = 0;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
};

extern GameState s_gameState;