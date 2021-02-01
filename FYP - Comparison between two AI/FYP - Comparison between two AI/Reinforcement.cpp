#include "Reinforcement.h"

ReinforcementLearning::ReinforcementLearning(Time t_timer):
	m_score(-100),
	m_runTimer(t_timer)
{
}

ReinforcementLearning::~ReinforcementLearning()
{
}

void ReinforcementLearning::update(Time t_deltaTime)
{
	m_runTimer += t_deltaTime;
}

void ReinforcementLearning::praise(int t_amount)
{
	m_score += t_amount;
}

void ReinforcementLearning::punish(int t_amount)
{
	m_score -= t_amount;
}

void ReinforcementLearning::addData(float t_rotation, Vector2f t_pos, float t_speed)
{
	Data temp;
	temp.m_rotation = t_rotation;
	temp.m_position = t_pos;
	temp.m_speed = t_speed;
	m_data.push_back(temp);
}
