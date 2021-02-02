#pragma once
#include "Button.h" 
#include "GameState.h"

class TainingInstructionsScreen
{
public:
	TainingInstructionsScreen();
	~TainingInstructionsScreen();
	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);
	void handleInput(Event& e, RenderWindow& t_window);
private:
	static const int MAX_BUTTONS = 2;

	Button m_buttons[MAX_BUTTONS];

	Text m_titleText;
	Text m_instructions;
	Font m_font;

	Sprite m_keys;
	Texture m_keyTex;

	Time m_clickTimer;

	void setup();
	void setupSprite();
	void setupText();
	void checkButtonPress();
};