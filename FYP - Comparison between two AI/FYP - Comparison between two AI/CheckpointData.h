#pragma once
#include <SFML/Graphics.hpp>

struct CheckpointData
{
	static const int MAX_ENTRIES = 5;
	sf::Vector2f m_pos[MAX_ENTRIES] =
	{
		{100.0f,300.0f},
		{400.0f,100.0f},
		{800.0f,330.0f},
		{850.0f,600.0f},
		{420.0f,800.0f},
	};
	sf::Vector2f m_size[MAX_ENTRIES] =
	{
		{200.0f,10.0f},
		{10.0f,200.0f},
		{150.0f,10.0f},
		{250.0f,10.0f},
		{200.0f,10.0f},
	};
};