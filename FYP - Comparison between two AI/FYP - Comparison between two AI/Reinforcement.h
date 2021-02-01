#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

struct Data
{
	float m_rotation = 0.0f;
	Vector2f m_position = Vector2f(0.0f,0.0f);
	float m_speed = 0.0f;
};

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