#include "main.hpp"

int main()
{
	DEV::KeepConsoleOpen keepConsoleOpen;
	keepConsoleOpen.setKeyRequirement(DEV::KeepConsoleOpen::Escape);

	try
	{
		Game game;
		game.run();
	}
	catch (int e)
	{
		DEV::printLine("Exception caught:\n\"" + pl::stringFrom(e) + "\"");
		return EXIT_FAILURE;
	}
	catch (char* e)
	{
		DEV::printLine("Exception caught:\n\"" + std::string(e) + "\"");
		return EXIT_FAILURE;
	}
	catch (...)
	{
		DEV::printLine("Unknown exception caught!");
		return EXIT_FAILURE;
	}

	keepConsoleOpen.allowToClose();
	return EXIT_SUCCESS;
}
