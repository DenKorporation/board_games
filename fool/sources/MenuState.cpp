#include "MenuState.h"
#include "Button.h"
#include "Label.h"
#include "Utility.h"
#include "ResourceHolder.hpp"
#include "SpriteNode.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack &stack, Context context)
	: State(stack, context),
	  mGUIContainer()
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;

	sf::Texture &texture = context.textures->get(Textures::ID(Textures::Global::Background));
	SpriteNode::Ptr backgroundSprite(new SpriteNode(texture));
	backgroundSprite->setScale((float)winX / texture.getSize().x, (float)winY / texture.getSize().y);
	mSceneGraph.attachChild(std::move(backgroundSprite));

	GUI::Container::Ptr GUIContainer(new GUI::Container);
	mGUIContainer = GUIContainer.get();
	mSceneGraph.attachChild(std::move(GUIContainer));

	mGUIContainer->setPosition(winX / 2.f, 0.f);

	GUI::Label::Ptr logo(new GUI::Label("Fool", *context.fonts, Fonts::Label));
	logo->setFontSize(winY / 3);
	logo->setOutlineThickness(7.f);
	logo->setLetterSpacing(5);
	logo->setFillColor(sf::Color(200, 200, 200));
	logo->setPosition(0.f, winY / 5.f);
	mGUIContainer->attachChild(std::move(logo));

	GUI::Container::Ptr GUIMenu(new GUI::Container);
	GUIMenu->setPosition(0.f, winY * 3.f / 5.f);

	GUI::Button::Style normalStyle(winY / 20, sf::Color(255, 255, 255), sf::Color(105, 105, 105), sf::Color::Black, 1.f);
	GUI::Button::Style selectedStyle(winY / 20, sf::Color::Red, sf::Color(105, 105, 105), sf::Color::Black, 1.f);

	GUI::Button::Ptr playButton(new GUI::Button(*context.fonts));
	playButton->setSize(sf::Vector2f(winX / 5.f, winY / 10.f));
	playButton->setText("Play");
	playButton->setNormalStyle(normalStyle);
	playButton->setSelectedStyle(selectedStyle);
	playButton->setCallback([this]()
							{
								requestStackPop(); 
								requestStackPush(States::Game); });
	GUIMenu->attachChild(std::move(playButton));

	GUI::Button::Ptr statisticButton(new GUI::Button(*context.fonts));
	statisticButton->setPosition(0.f, (winY / 10.f) * 1.2f);
	statisticButton->setSize(sf::Vector2f(winX / 5.f, winY / 10.f));
	statisticButton->setText("Statistic");
	statisticButton->setNormalStyle(normalStyle);
	statisticButton->setSelectedStyle(selectedStyle);
	statisticButton->setCallback([this]()
								 {
								requestStackPop();
								requestStackPush(States::Statistic); });
	GUIMenu->attachChild(std::move(statisticButton));

	GUI::Button::Ptr exitButton(new GUI::Button(*context.fonts));
	exitButton->setPosition(0.f, 2 * (winY / 10.f) * 1.2f);
	exitButton->setSize(sf::Vector2f(winX / 5.f, winY / 10.f));
	exitButton->setText("Exit");
	exitButton->setNormalStyle(normalStyle);
	exitButton->setSelectedStyle(selectedStyle);
	exitButton->setCallback([this]()
							{ requestStackClear(); });

	GUIMenu->attachChild(std::move(exitButton));

	mGUIContainer->attachChild(std::move(GUIMenu));
}

void MenuState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mSceneGraph);
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event &event)
{
	mGUIContainer->handleEvent(event);
	return true;
}
