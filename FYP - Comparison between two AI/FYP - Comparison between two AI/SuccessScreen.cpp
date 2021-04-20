#include "SuccessScreen.h"

SuccessScreen::SuccessScreen():
	m_buttons
{
	Button(Vector2f(490.0f,700.0f), "Main Menu")
},
m_clickTimer(Time::Zero)
{
	setup();
}

SuccessScreen::~SuccessScreen()
{
}

void SuccessScreen::update(Time t_deltaTime)
{
	m_clickTimer += t_deltaTime;
	checkButtonPress();
}

void SuccessScreen::render(RenderWindow& t_window)
{
	t_window.draw(m_titleText);
	t_window.draw(m_text);
	for (Button& b : m_buttons)
	{
		b.render(t_window);
	}
}

void SuccessScreen::handleInput(Event& e, RenderWindow& t_window)
{
	for (Button& b : m_buttons)
	{
		b.handleInput(e, t_window);
	}
}

void SuccessScreen::setup()
{
	setupText();
}

void SuccessScreen::setupText()
{
	if (!m_font.loadFromFile("ASSETS/FONT/font.ttf"))
		cout << "Error loading font" << endl;

	m_titleText.setFont(m_font);
	m_titleText.setString("Training Data \n       Saved");
	m_titleText.setFillColor(Color::White);
	m_titleText.setPosition(Vector2f(310.0f, -50.0f));
	m_titleText.setCharacterSize(150);

	m_text.setFont(m_font);
	m_text.setString("Backpropagation algorithm can now be run from \nthe python script.");
	m_text.setFillColor(Color::White);
	m_text.setPosition(Vector2f(100.0f, 300.0f));
	m_text.setCharacterSize(90);
}

void SuccessScreen::checkButtonPress()
{
	if (m_clickTimer >= seconds(0.5))
	{
		if (m_buttons[0].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::MainMenu;
		}
	}
}
