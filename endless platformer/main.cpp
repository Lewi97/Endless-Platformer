#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#include "character.h"
#include "platform.h"
#include "rng.h"
#include "Lava.h"
#include "meteor.h"

#define LOG(x) std::cout << x << std::endl;

constexpr int windowWidth = { 800 };
constexpr int windowHeight = { 600 };
constexpr float riseSpeed = { 120 };
constexpr float platformSpacing = { 75 }; 

void initPlatforms(float& goal, std::vector<platform>& platforms)
{
	float prevOffset = 0.f;
	for (int x = 0; x < 11; x++) {
		int mod = rng::get().intInRange(1, 2) > 1 ? -1 : 1;			// create negative or positive number
		float offset = (rng::get().floatInRange(0, 45) + 45) * mod; // platform will now be spaced atleast 45 pixels away 
		platforms[x].setPos({ windowWidth / 2 - (prevOffset + offset), goal });
		platforms[x].touched = true;
		goal -= platformSpacing;
		prevOffset = offset;
	}
}

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
		LOG("Font failed to load!");
	}

	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::White);
	
	sf::Shader shader;
	if (!shader.loadFromFile("lavashader.frag", sf::Shader::Fragment))
	{
		LOG("Shader failed to load!");
	}
	shader.setUniform("u_resolution", sf::Glsl::Vec2(windowWidth,windowHeight));
	sf::Clock shader_clock;

	// initializing player
	character player({ playerStartPos }, 150, sf::Color::Green);

	// initializing the first big blue platform
	platform plat({ 100.f,10.f }, { windowWidth / 2, windowHeight / 2 + 50.f });
	plat.setColor(sf::Color::Blue);

	// initializing platforms vector and reserving a space for 19 elements
	std::vector<platform> platforms;
	platforms.reserve(19);
	platforms.emplace_back(plat);

	float prevOffset = 0.f;
	float goal = player.getPos().y + platformSpacing / 2;

	for (int x = 0; x < 11; x++) {
		int mod = rng::get().intInRange(1, 2) > 1 ? -1 : 1;			// create negative or positive number
		float offset = (rng::get().floatInRange(0, 45) + 45) * mod; // platform will now be spaced atleast 45 pixels away 
		platform plat({ 100.f,10.f }, { windowWidth / 2 - (prevOffset + offset), goal });
		platforms.emplace_back(plat);
		goal -= platformSpacing;
		prevOffset = offset;
	}

	// initializing meteors vector
	std::vector<meteor> meteors;
	platforms.reserve(4);

	sf::Clock dt;
	float delta = 0.f;
	sf::Vector2f velocity;

	// keeping track which 4 platforms are current in line of updating to a higher position
	int current = 0;

	// player movement booleans
	bool right = false, left = false;
	bool inAir = false;
	bool spaceHeld = false;

	// stat initialization
	float jumpStrength = 100;
	int score = 0;

	// lava initialization
	Lava lava({ windowWidth, windowHeight }, { lavaStartPos } );
	lava.setRise(riseSpeed);

	view.setCenter(player.getPos());

	while (window.isOpen()) {
		// get delta time
		delta = dt.restart().asSeconds();
		// LOG(delta);
		// window and ui stuff
		window.setView(view);
		view.setCenter({ windowWidth / 2, player.getPos().y });

		scoreText.setPosition(sf::Vector2f(windowWidth / 2 - scoreText.getGlobalBounds().width / 2, player.getPos().y + scoreText.getGlobalBounds().height - windowHeight / 2)); // math course to center text at top of screen

		shader.setUniform("u_time", shader_clock.getElapsedTime().asSeconds());

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

		if (player.getPos().y <= goal + platformSpacing * 4)
		{
			for (int i = 0; i < 4; i++)
			{
				int mod = rng::get().intInRange(1, 2) > 1 ? -1 : 1;			// create negative or positive number
				float offset = (rng::get().floatInRange(0, 45) + 45) * mod; // platform will now be spaced atleast 45 pixels away 

				platforms[current].setPos({ windowWidth / 2 - (prevOffset + offset), goal });
				platforms[current].touched = true;

				current += 1;
				prevOffset = offset;
				goal -= platformSpacing;
			}
			if (current > 11) { current = 0; }

			int meteor_rad = rng::get().intInRange(10, 25);
			int meteor_speed = rng::get().intInRange(50, 250);
			meteor Meteor(meteor_rad, {player.getPos().x , goal}, meteor_speed);
			if (meteors.size() > 1) meteors.erase(meteors.begin());
			meteors.emplace_back(Meteor);
			// lava.increaseRise(riseSpeed);
		}

		for (auto& plat : platforms)
		{

			// collision checks, fall if not atleast touching one platform
			if (plat.isColliding(player.getSize(), player.getPos()))
			{
				//if (plat.touched) { score += 1; plat.touched = false; scoreText.setString(std::to_string(score)); }
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
			current = 0;
			player.setPos(playerStartPos);
			lava.setPos(lavaStartPos.y);
			goal = player.getPos().y + platformSpacing / 2;
			initPlatforms(goal, platforms);
			platforms[0].setPos({ windowWidth / 2, windowHeight / 2 + 50.f });
			score = 0;
			player.stopJumping();
			meteors.clear();
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
		
		// render the platform and meteors
		for (auto plat : platforms) plat.render(window);
		for (auto& m : meteors)
		{
			m.render(window); m.update(delta); 
			if (m.isColliding(player.getPos(), player.getSize()))
			{ // should really make dying into a function
				current = 0;
				player.setPos(playerStartPos);
				lava.setPos(lavaStartPos.y);
				goal = player.getPos().y + platformSpacing / 2;
				initPlatforms(goal, platforms);
				platforms[0].setPos({ windowWidth / 2, windowHeight / 2 + 50.f });
				score = 0;
				player.stopJumping();
				meteors.clear();
				break;
			}
		}

		lava.update(delta);

		lava.render(window, shader);

		player.move(velocity.x * delta);
		
		window.draw(scoreText);

		player.update(delta);
		
		player.render(window);

		window.display();
	
	}

	return 0;
}