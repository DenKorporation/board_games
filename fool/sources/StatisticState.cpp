#include "StatisticState.h"
#include "Button.h"
#include "Label.h"
#include "Utility.h"
#include "ResourceHolder.hpp"
#include "SpriteNode.h"
#include "ShapeNode.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

StatisticState::StatisticState(StateStack &stack, Context context)
	: State(stack, context)
{
	unsigned int winX = context.window->getSize().x, winY = context.window->getSize().y;
	sf::Vector2f blockSize(winX / 2.f, winY * 0.95f);

	mSceneGraph.setPosition(winX / 2.f, 0.f);

	sf::Texture &texture = context.textures->get(Textures::ID(Textures::Global::Background));
	SpriteNode::Ptr backgroundSprite(new SpriteNode(texture));
	backgroundSprite->setScale((float)winX / texture.getSize().x, (float)winY / texture.getSize().y);
	backgroundSprite->setPosition(0.f, winY / 2.f);
	mSceneGraph.attachChild(std::move(backgroundSprite));

	ShapeNode::Ptr statisticBackground(new ShapeNode(blockSize));
	statisticBackground->setPosition(0.f, winY / 2.f);
	statisticBackground->setFillColor(sf::Color(105, 105, 105, 220));
	statisticBackground->setOutlineThickness(3.f);
	statisticBackground->setOutlineColor(sf::Color::Black);

	GUI::Container::Ptr GUIContainer(new GUI::Container);
	GUIContainer->setPosition(0.f, -blockSize.y / 2.f);
	mGUIContainer = GUIContainer.get();
	statisticBackground->attachChild(std::move(GUIContainer));
	mSceneGraph.attachChild(std::move(statisticBackground));

	GUI::Label::Ptr label(new GUI::Label("Statistics", *context.fonts, Fonts::Main));
	label->setFontSize((unsigned int)blockSize.y / 8);
	label->setOutlineThickness(3.f);
	label->setFillColor(sf::Color(200, 200, 200));
	label->setPosition(0.f, blockSize.y * 0.1f);
	mGUIContainer->attachChild(std::move(label));

	GUI::Button::Ptr backButton(new GUI::Button(*context.fonts));
	backButton->setPosition(0.f, blockSize.y * 0.9f);
	backButton->setSize(sf::Vector2f(blockSize.x / 3.f, blockSize.y / 10.f));
	backButton->setText("Back");
	backButton->setNormalStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::White, sf::Color(105, 105, 105),
												  sf::Color::Black, 3.f));
	backButton->setSelectedStyle(GUI::Button::Style((unsigned int)blockSize.y / 20, sf::Color::Red, sf::Color(105, 105, 105),
													sf::Color::Black, 3.f));
	backButton->setCallback([this]()
							{
		requestStackPop();
		requestStackPush(States::Menu); });

	mGUIContainer->attachChild(std::move(backButton));

	std::map<std::string, int> statistics = getStatistics();

	// total games
	GUI::Label::Ptr text(new GUI::Label("total games", *context.fonts, Fonts::Main));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(-0.45f * blockSize.x + text->getBounds().width / 2.f, blockSize.y * 0.3f);
	mGUIContainer->attachChild(std::move(text));

	text = std::move(GUI::Label::Ptr(new GUI::Label(std::to_string(statistics["total"]), *context.fonts, Fonts::Main)));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(0.45f * blockSize.x - text->getBounds().width / 2.f, blockSize.y * 0.3f);
	mGUIContainer->attachChild(std::move(text));

	// victories
	text = std::move(GUI::Label::Ptr(new GUI::Label("victory", *context.fonts, Fonts::Main)));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(-0.45f * blockSize.x + text->getBounds().width / 2.f, blockSize.y * 0.4f);
	mGUIContainer->attachChild(std::move(text));

	text = std::move(GUI::Label::Ptr(new GUI::Label(std::to_string(statistics["victory"]), *context.fonts, Fonts::Main)));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(0.45f * blockSize.x - text->getBounds().width / 2.f, blockSize.y * 0.4f);
	mGUIContainer->attachChild(std::move(text));

	// losses
	text = std::move(GUI::Label::Ptr(new GUI::Label("lose", *context.fonts, Fonts::Main)));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(-0.45f * blockSize.x + text->getBounds().width / 2.f, blockSize.y * 0.5f);
	mGUIContainer->attachChild(std::move(text));

	text = std::move(GUI::Label::Ptr(new GUI::Label(std::to_string(statistics["lose"]), *context.fonts, Fonts::Main)));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(0.45f * blockSize.x - text->getBounds().width / 2.f, blockSize.y * 0.5f);
	mGUIContainer->attachChild(std::move(text));

	// number of draws
	text = std::move(GUI::Label::Ptr(new GUI::Label("draw", *context.fonts, Fonts::Main)));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(-0.45f * blockSize.x + text->getBounds().width / 2.f, blockSize.y * 0.6f);
	mGUIContainer->attachChild(std::move(text));

	text = std::move(GUI::Label::Ptr(new GUI::Label(std::to_string(statistics["draw"]), *context.fonts, Fonts::Main)));
	text->setFontSize((unsigned int)blockSize.y / 15);
	text->setOutlineThickness(1.f);
	text->setFillColor(sf::Color(200, 200, 200));
	text->setPosition(0.45f * blockSize.x - text->getBounds().width / 2.f, blockSize.y * 0.6f);
	mGUIContainer->attachChild(std::move(text));
}

void StatisticState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mSceneGraph);
}

bool StatisticState::update(sf::Time dt)
{
	return true;
}

bool StatisticState::handleEvent(const sf::Event &event)
{
	mGUIContainer->handleEvent(event);
	return true;
}
