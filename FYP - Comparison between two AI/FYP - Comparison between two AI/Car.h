#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>

#include "CheckpointData.h"
#include "BackProp.h"
#include "Brain.h"

using namespace sf;

class Car
{
public:
	Car(Vector2f t_startPos, float t_startSpeed);
	~Car();
	void update(Time t_deltaTime,Vector2f t_cpPos);
	void render(RenderWindow& t_window,bool t_debug);
	void handleInput(vector<float> t_inputs);
	Vector2f getPos() { return m_pos; };
	std::vector<Vector2f> getColLines() { return m_collisionLines; };
	std::vector<Vector2f> getRays() { return m_raycasts; };
	void push(Vector2f t_dir) { m_pos += t_dir; };
	Sprite getSprite() { return m_sprite; }
	FloatRect getBounds() { return m_sprite.getGlobalBounds(); };
	float getRotation() { return m_rotation; }
	void setRotation(float t_new) { m_rotation = t_new; }

	void collidesBoundary(shared_ptr<Brain> t_brain);
	void collidesPassedCP();
	void collidesCheckpoint();
	void saveTrainingDataToFile();
	void nextCP() { m_cpNum++; };
	void setCPNum(int t_new) { m_cpNum = t_new; };
	int getCpNum() { return m_cpNum; };
	void processOutputs(vector<float> t_outputs);
	void reset();
	void setColLineLength(vector<float> t_lengths);
	void updateColLines();
	void changeColor(Color t_new) { m_sprite.setColor(t_new); }

	Time m_CPTimer;
	float collLineLengths[5];
	bool m_test = false;

private:
	Sprite m_sprite;
	Texture m_tex;

	Vector2f m_pos;
	Vector2f m_restartPos;
	float m_restartSpeed;
	double m_rotation;
	float m_speed;
	const float MAX_SPEED = 10.0f;
	Vector2f m_velocity;
	Vector2f m_forward;

	std::vector<Vector2f> m_raycasts;
	const int MAX_COLLISIONS = 5;
	const float RAY_RADIUS = 200;

	std::vector<Vector2f> m_collisionLines;
	const float COL_RADIUS = 25;

	const float RATE_OF_ROTATION = 5.0f;
	const float MASS = 1.0f;
	const float MAX_ACCELERATION = 20.0f;
	const float MIN_ACCELERATION = 10.0f;
	const float RATE_OF_ACCELERATION = 5.0f;

	int m_pastRscore;
	float m_pastDist;

	CheckpointData1 m_checkpoints;
	int m_cpNum;

	bool m_replay;
	int m_current;

	Backprop m_backprop;

	void setup();
	void move(Time t_deltaTime, Vector2f t_cpPos);
	Vector2f normalise(Vector2f t_vec);
	float distance(Vector2f t_vec1, Vector2f t_vec2);
	void replayLearning();
	void turnLeft();
	void turnRight();
	void accelerate();
	void decelerate();
	void moveTowardsCheckpoint(Time t_deltaTime, Vector2f t_CPpos);
};