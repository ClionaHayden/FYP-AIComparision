#pragma once
#include <SFML/Graphics.hpp>

struct CheckpointData
{
	static const int MAX_ENTRIES = 20;
	sf::Vector2f m_pos[MAX_ENTRIES] =
	{
		{100.0f,330.0f},
		{100.0f,170.0f},
		{200.0f,100.0f},
		{400.0f,100.0f},
		{580.0f,100.0f},
		{650.0f,180.0f},
		{800.0f,330.0f},
		{730.0f,390.0f}, 
		{465.0f,390.0f}, 
		{400.0f,455.0f},
		{480.0f,520.0f},
		{740.0f,520.0f}, 
		{850.0f,600.0f},
		{740.0f,700.0f}, 
		{510.0f,700.0f}, 
		{420.0f,800.0f},
		{340.0f,870.0f}, 
		{170.0f,870.0f}, 
		{100.0f,810.0f},
		{100.0f,600.0f}
	};
	sf::Vector2f m_size[MAX_ENTRIES] =
	{
		{200.0f,10.0f},
		{200.0f,10.0f},
		{10.0f,150.0f},
		{10.0f,150.0f},
		{10.0f,150.0f},
		{200.0f,10.0f},
		{150.0f,10.0f},
		{10.0f,120.0f}, 
		{10.0f,120.0f}, 
		{200.0f,10.0f},
		{10.0f,120.0f}, 
		{10.0f,120.0f}, 
		{250.0f,10.0f}, 
		{10.0f,120.0f}, 
		{10.0f,120.0f}, 
		{200.0f,10.0f},
		{10.0f,120.0f}, 
		{10.0f,120.0f}, 
		{200.0f,10.0f},
		{200.0f,10.0f}
	};
	sf::Vector2f m_centres[MAX_ENTRIES] =
	{
		{200.0f,335.0f},
		{200.0f,175.0f},
		{205.0f,175.0f},
		{405.0f,175.0f},
		{585.0f,175.0f}, 
		{750.0f,185.0f},
		{875.0f,335.0f},
		{735.0f,450.0f},
		{470.0f,450.0f},
		{500.0f,460.0f},
		{485.0f,580.0f}, 
		{745.0f,580.0f}, 
		{975.0f,605.0f}, 
		{745.0f,580.0f}, 
		{515.0f,760.0f}, 
		{320.0f,805.0f},
		{345.0f,930.0f}, 
		{175.0f,930.0f}, 
		{200.0f,815.0f},
		{200.0f,605.0f}
	};
};