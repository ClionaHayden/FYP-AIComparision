#pragma once
#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
class Checkpoint
{
public:
	Checkpoint(Vector2f t_pos, Vector2f t_size);
	~Checkpoint();
	void toggleDraw();
	void draw(RenderWindow& t_win);
	void setOrigin(sf::Vector2f t_new);
	void setPassed(bool t_new) { m_passed = t_new; };
	bool getPassed() { return m_passed; };
	FloatRect getBounds() { return m_checkpoint.getGlobalBounds(); };
	Vector2f getPos() { return m_pos; };
	Vector2f getOrigin() { return m_checkpoint.getOrigin(); };
private:
	sf::RectangleShape m_checkpoint;
	bool m_draw;
	Vector2f m_pos;
	bool m_passed;

	void setup(Vector2f t_size);
};