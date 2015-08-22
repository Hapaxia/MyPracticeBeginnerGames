#include <DevSfml.hpp>
#include <PlinthSfml.hpp>
#include <Kairos.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	DEV::KeepConsoleOpen keepConsoleOpen;
	keepConsoleOpen.setKeyRequirement(DEV::KeepConsoleOpen::Escape);

	sf::RenderWindow window(sf::VideoMode(800, 600), "test", sf::Style::Default);
	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (DEV::eventIsDefaultCloseWindow(event))
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::F12)
				{
					DEV::printLine("Forced error!");
					return EXIT_FAILURE;
				}
			}
		}

		window.clear();
		//window.draw();
		window.display();
	}

	keepConsoleOpen.allowToClose();
	return EXIT_SUCCESS;
}
