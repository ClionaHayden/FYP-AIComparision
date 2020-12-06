#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Boundry
{
public:
	Boundry(Vector2f t_pos, Vector2f t_size, float t_rot);
	~Boundry();
	void toggleDraw();
	void draw(RenderWindow& t_win);
	void setOrigin(sf::Vector2f t_new);
private:
	sf::RectangleShape m_boundry;
	bool m_draw;
	Vector2f m_pos;	

	void setup(Vector2f t_size, float t_rot);
};