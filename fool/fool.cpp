#include "Engine/Application.h"

#include <iostream>

int main()
{
	try
	{
		Application app;
		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}