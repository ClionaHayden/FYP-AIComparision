#pragma once
#include <SFML/Graphics.hpp>

struct boundryData1
{
	static const int MAX_ENTRIES = 18;
	sf::Vector2f m_pos[MAX_ENTRIES] =
	{
		{30.0f,10.0f},
		{460.0f,445.0f},
		{300.0f, 570.0f},
		{505.0f, 750.0f},
		{270.0f, 315.0f},
		{40.0f, 930.0f},
		{160.0f, 800.0f},
		{35.0f, 20.0f},
		{155.0f, 180.0f},
		{155.0f, 165.0f},
		{935.0f, 440.0f},
		{750.0f, 180.0f},
		{870.0f, 200.0f},
		{750.0f, 30.0f},
		{575.0f, 200.0f},
		{500.0f, 750.0f},
		{300.0f, 300.0f},
		{340.0f, 600.0f}
	};
	sf::Vector2f m_size[MAX_ENTRIES] =
	{
		{750.0f, 20.0f},
		{500.0f, 20.0f},
		{450.0f, 70.0f},
		{460.0f, 45.0f},
		{480.0f, 20.0f},
		{500.0f, 20.0f},
		{190.0f, 20.0f},
		{15.0f, 1000.0f},
		{15.0f, 640.0f},
		{430.0f, 40.0f},
		{20.0f, 340.0f},
		{150.0f, 20.0f},
		{10.0f, 260.0f},
		{10.0f, 150.0f},
		{10.0f, 120.0f},
		{10.0f, 300.0f},
		{10.0f, 300.0f},
		{10.0f, 200.0f}
	};
	float m_rotation[MAX_ENTRIES] =
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0, 
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	};
};
struct boundryData2
{
	static const int MAX_ENTRIES = 8;
	sf::Vector2f m_pos[MAX_ENTRIES] =
	{
		{30.0f,10.0f},
		{30.0f,10.0f},
		{900.0f,10.0f},
		{10.0f,920.0f},
		{180.0f,150.0f},
		{730.0f,150.0f},
		{180.0f,800.0f},
		{180.0f,150.0f}
	};
	sf::Vector2f m_size[MAX_ENTRIES] =
	{
		{10.0f,1000.0f},
		{1000.0f,10.0f},
		{10.0f,1000.0f},
		{1000.0f,10.0f},
		{550.0f,10.0f},
		{10.0f,650.0f},
		{560.0f,10.0f},
		{10.0f,650.0f}
	};
	float m_rotation[MAX_ENTRIES] =
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
	};
};