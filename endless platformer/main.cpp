#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#include "character.h"
#include "platform.h"
#include "rng.h"
#include "Lava.h"

#define LOG(x) std::cout << x << std::endl;

constexpr int windowWidth = { 800 };
constexpr int windowHeight = { 600 };
constexpr float riseSpeed = { 120 };
constexpr float platformSpacing = { 75 }; 

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "main");
	sf::Event event;
	sf::View view(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
	window.setView(view);
	
	const sf::Vector2f lavaStartPos = { 0.f, windowHeight + 100.f };
	const sf::Vector2f playerStartPos = { windowWidth / 2, windowHeight / 2 };

	sf::Text scoreText;
	sf::Font font;
	if (!font.loadFromFile("PressStart2P.ttf"))
	{
		LOG("error");
	}

	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::White);

	// initializing platforms vector and reserving a space for 19 elements
	std::vector<platform> platforms;
	platforms.reserve(19);
	
	sf::Clock dt;
	float delta = 0.f;
	sf::Vector2f velocity;

	// player movement booleans
	bool right = false, left = false;
	bool inAir = false;
	bool spaceHeld = false;

	// initializing the first big blue platform
	platform plat({ 100.f,100.f }, { windowWidth / 2, windowHeight / 2 + 50.f });
	plat.setColor(sf::Color::Blue);
	platforms.emplace_back(plat);

	// player initialization
	character player({ playerStartPos },150,sf::Color::Green);
	float jumpStrength = 100;
	int score = 0;

	// lava initialization
	Lava lava({ windowWidth, windowHeight }, { lavaStartPos } );
	lava.setRise(riseSpeed);

	float prevOffset = player.getPos().x;
	float goal = player.getPos().y + platformSpacing / 2;
	view.setCenter(player.getPos());

	while (window.isOpen()) {
	// get delta time
	delta = dt.restart().asSeconds();

	// window and ui stuff
	window.setView(view);
	view.setCenter({ windowWidth / 2, player.getPos().y });

	scoreText.setPosition(sf::Vector2f(windowWidth / 2 - scoreText.getGlobalBounds().width / 2, player.getPos().y + scoreText.getGlobalBounds().height - windowHeight / 2)); // math course to center text at top of screen

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed: window.close(); break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::A) { left = true; }
				if (event.key.code == sf::Keyboard::D) { right = true; }
				if (event.key.code == sf::Keyboard::Space) { spaceHeld = true; }
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::A) { left = false; }
				if (event.key.code == sf::Keyboard::D) { right = false; }
				if (event.key.code == sf::Keyboard::Space) { spaceHeld = false; }
				break;
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					sf::Vector2i pos = sf::Mouse::getPosition(window);
					platform plat({ 100.f,10.f }, window.mapPixelToCoords(pos) );
					platforms.emplace_back(plat);
				}
				break;
			}
		}
		window.clear(sf::Color::Black);

		if (spaceHeld)
		{
			if (!player.isJumping() && !inAir) player.jump(jumpStrength);
		}

		if (player.getPos().y <= (goal + platformSpacing * 4)) 
		{
			for (int x = 0; x < 8; x++) {
				float offset = rng::get().floatInRange(-90, 90);
				platform plat({ 100.f,10.f }, { windowWidth / 2 - (prevOffset + offset), goal });
				if (platforms.size() > 16) // if more than 16 platforms, remove the first, maybe better performance wise to just move the oldest 8 platforms up, but how to determine oldest
				{
					platforms.erase(platforms.begin());
				}
				platforms.emplace_back(plat);
				goal -= platformSpacing;
				prevOffset = offset;
			}
			// lava.increaseRise(riseSpeed);
		}

		for (auto plat : platforms)
		{
			// render the platform
			plat.render(window);
		}

		for (auto& plat : platforms)
		{

			// collision checks, fall if not atleast touching one platform
			if (plat.isColliding(player.getSize(), player.getPos()))
			{
				//if (plat.touched) { score += 1; plat.touched = true; scoreText.setString(std::to_string(score)); }
				score += plat.touched;
				plat.touched -= plat.touched;
				scoreText.setString(std::to_string(score));
				inAir = false;
				break;
			}
			inAir = true;
		}
		
		if (lava.getPosition() < player.getPos().y) // dying, should be able to do this in a better way
		{
			platforms.clear();
			player.setPos(playerStartPos);
			lava.setPos(lavaStartPos.y);
			goal = player.getPos().y + platformSpacing / 2;
			platform plat({ 100.f,100.f }, { windowWidth / 2, windowHeight / 2 + 50.f });
			plat.setColor(sf::Color::Blue);
			platforms.emplace_back(plat);
			score = 0;
		}

		// If the player gets too far from the lava, place lava at bottom of the screen for extra tension :)
		if (lava.getPosition() > player.getPos().y + windowHeight / 2)
		{ 
			lava.setPos(player.getPos().y + windowHeight / 2);
		}

		// fall if in the air and not jumping
		if (inAir && !player.isJumping())
		{
			player.fall(delta);
		}

		// using the booleans to mooveleans
		velocity = { (float)right - (float)left, 0.f };

		lava.update(delta);

		lava.render(window);

		player.move(velocity.x * delta);
		
		window.draw(scoreText);

		player.update(delta);
		
		player.render(window);

		window.display();
	
	}

	return 0;
}