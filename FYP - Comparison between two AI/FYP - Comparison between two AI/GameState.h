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
	Exit
};

struct Data
{
	float m_rotation;
	sf::Vector2f m_position;
	float m_speed;
};

struct TrainingData
{
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
};

extern GameState s_gameState;