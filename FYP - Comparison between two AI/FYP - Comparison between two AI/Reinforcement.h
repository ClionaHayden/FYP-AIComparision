#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "GameState.h"

using namespace sf;

class ReinforcementLearning
{
public: 
	ReinforcementLearning(Time t_timer);
	~ReinforcementLearning();
	void update(Time t_deltaTime);

	void praise(int t_amount);
	void punish(int t_amount);

	int getScore() { return m_score; };
	void setScore(int t_new) { m_score = t_new; };
	Time getTimer() { return m_runTimer; };
	void setTimer(Time t_new) { m_runTimer = t_new; };
	void addData(float t_rotation, Vector2f t_pos, float t_speed);
	std::vector<Data> getData() { return m_data; };

private:
	int m_score;
	Time m_runTimer;
	std::vector<Data> m_data;
};