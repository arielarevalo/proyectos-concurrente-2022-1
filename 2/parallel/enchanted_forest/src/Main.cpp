// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#include "./include/EnchantedForest.hpp"
#include "./include/Logger.hpp"

using std::string;

/**
 * @brief Entry point.
 */
int main(int argc, char* argv[])
{
	if (argc > 1 )
	{
		string path{ argv[1] };

		try
		{
			EnchantedForest::process(path);
		}
		catch (const std::exception& e)
		{
			Logger::error("EnchantedForest has crashed.", e);
		}
		exit(1);
	}
	else
	{
		Logger::error("No path provided.");
		exit(1);
	}
}
