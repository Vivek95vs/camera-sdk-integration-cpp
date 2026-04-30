
#include "stdafx.h"
#include "Grabber.h"
#include <conio.h>
//#include "GenICam.h"


int main()
{
	// flag to track when an exception has been thrown

	std::cout << "Cpp_SimpleAcquisition\n";
	Grabber grab;
	// flag to track when an exception has been thrown
		bool exceptionThrown = false;

		std::cout << "Cpp_SimpleAcquisition\n";

		try
		{
			// run example
			std::cout << "Commence example\n\n";
			grab.startsystem();
			grab.GrabFrame();
			grab.stopsystem();
			std::cout << "\nExample complete\n";
		}
		catch (GenICam::GenericException& ge)
		{
			std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
			exceptionThrown = true;
		}
		catch (std::exception& ex)
		{
			std::cout << "Standard exception thrown: " << ex.what() << "\n";
			exceptionThrown = true;
		}
		catch (...)
		{
			std::cout << "Unexpected exception thrown\n";
			exceptionThrown = true;
		}

		std::cout << "Press enter to complete\n";
		std::getchar();

		if (exceptionThrown)
			return -1;
		else
			return 0;

}