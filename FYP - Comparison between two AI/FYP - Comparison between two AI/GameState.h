#pragma once


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

extern GameState s_gameState;