#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>

#include "Reinforcement.h"
#include "CheckpointData.h"
#include "BackProp.h"
#include "Brain.h"

using namespace sf;

class Car
{
public:
	Car(Vector2f t_startPos, float t_startSpeed);
	~Car();
	void update(Time t_deltaTime);
	void render(RenderWindow& t_window,bool t_debug);
	void handleInput(Event& e, vector<shared_ptr<float>> t_inputs);
	Vector2f getPos() { return m_pos; };
	std::vector<Vector2f> getColLines() { return m_collisionLines; };
	void push(Vector2f t_dir) { m_pos += t_dir; };
	Sprite getSprite() { return m_sprite; }

	void collidesBoundary(shared_ptr<Brain> t_brain);
	void collidesPassedCP();
	void collidesCheckpoint();
	void saveTrainingDataToFile();
	void nextCP() { m_cpNum++; };
	int getCpNum() { return m_cpNum; };
	void processOutputs(vector<shared_ptr<float>> t_outputs);
	void reset();
	void setColLineLength(vector<float> t_lengths);

	Time m_CPTimer;

private:
	Sprite m_sprite;
	Texture m_tex;

	Vector2f m_pos;
	Vector2f m_restartPos;
	float m_restartSpeed;
	double m_rotation;
	float m_speed;
	Vector2f m_velocity;
	Vector2f m_forward;

	std::vector<Vector2f> m_collisionLines;
	const int MAX_COLLISIONS = 5;
	const float COLLISION_RADIUS = 200;
	float collLineLengths[5];

	const float RATE_OF_ROTATION = 5.0f;
	const float MASS = 1.0f;
	const float MAX_ACCELERATION = 100.0f;
	const float MIN_ACCELERATION = 60.0f;
	const float RATE_OF_ACCELERATION = 5.0f;

	int m_pastRscore;
	float m_pastDist;

	CheckpointData m_checkpoints;
	int m_cpNum;

	ReinforcementLearning m_reinforcement;
	void reinforcement();
	bool m_replay;
	int m_current;

	Backprop m_backprop;

	void setup();
	void move(Time t_deltaTime);
	void updateColLines();
	Vector2f normalise(Vector2f t_vec);
	float distance(Vector2f t_vec1, Vector2f t_vec2);
	void replayLearning();
	void turnLeft();
	void turnRight();
	void accelerate();
	void decelerate();
};