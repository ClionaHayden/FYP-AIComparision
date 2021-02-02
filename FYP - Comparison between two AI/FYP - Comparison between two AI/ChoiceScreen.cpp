#include "ChoiceScreen.h"

ChoiceScreen::ChoiceScreen():
	m_buttons
{
	Button(Vector2f(500.0f,300.0f), "Reinforcement"),
	Button(Vector2f(500.0f,500.0f), "Backpropagation"),
	Button(Vector2f(500.0f,700.0f), "Back")
},
m_clickTimer(Time::Zero)
{
	setup();
}

ChoiceScreen::~ChoiceScreen()
{
}

void ChoiceScreen::update(Time t_deltaTime)
{
	m_clickTimer += t_deltaTime;
	checkButtonPress();
}

void ChoiceScreen::render(RenderWindow& t_window)
{
	t_window.draw(m_titleText);
	t_window.draw(m_discText);
	for (Button& b : m_buttons)
	{
		b.render(t_window);
	}
}

void ChoiceScreen::handleInput(Event& e, RenderWindow& t_window)
{
	for (Button& b : m_buttons)
	{
		b.handleInput(e, t_window);
	}
}

void ChoiceScreen::setup()
{
	if (!m_font.loadFromFile("ASSETS/FONT/font.ttf")) { cout << "Error loading font" << endl; }
	m_titleText.setFont(m_font);
	m_titleText.setString("Machine Learning \n             Demo");
	m_titleText.setFillColor(Color::White);
	m_titleText.setPosition(Vector2f(250.0f, -50.0f));
	m_titleText.setCharacterSize(130);
}

void ChoiceScreen::checkButtonPress()
{
	if (m_clickTimer >= seconds(0.5))
	{
		//Reinforcement
		if (m_buttons[0].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::Reinforcement;
		}
		//Backpropagation
		if (m_buttons[1].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::TrainingDataInstructions;
		}
		//Back
		if (m_buttons[2].getButtonState() == ButtonState::Clicked)
		{
			m_clickTimer = Time::Zero;
			s_gameState = GameState::MainMenu;
		}
	}
}
