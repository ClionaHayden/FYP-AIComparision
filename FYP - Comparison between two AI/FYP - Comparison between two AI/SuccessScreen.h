#pragma once
#include "Button.h" 
#include "GameState.h"

class SuccessScreen
{
public:
	SuccessScreen();
	~SuccessScreen();
	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);
	void handleInput(Event& e, RenderWindow& t_window);
private:
	static const int MAX_BUTTONS = 1;

	Button m_buttons[MAX_BUTTONS];

	Text m_titleText;
	Text m_text;
	Font m_font;

	Time m_clickTimer;

	void setup();
	void setupText();
	void checkButtonPress();
};