// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#include <unistd.h>
#include <omp.h>
#include <string>

#include "./include/EnchantedForest.hpp"
#include "./include/Logger.hpp"

using std::stoi;
using std::string;
using std::invalid_argument;
using std::out_of_range;
using std::exception;

/**
 * @brief Entry point.
 */
int main(int argc, char* argv[])
{
	int numThreads{ static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN)) };

	try
	{
		if (argc < 2)
		{
			throw invalid_argument("No path provided.");
		}

		string path{ argv[1] };

		if (argc > 2)
		{
			numThreads = stoi(argv[2]);
		}

		omp_set_num_threads(numThreads);

		EnchantedForest::process(path);

		exit(0);
	}
	catch (out_of_range& oor)
	{
		Logger::error("Invalid number of threads", oor);
	}
	catch (invalid_argument& ia)
	{
		Logger::error("Invalid number of arguments", ia);
	}
	catch (exception& e)
	{
		Logger::error("EnchantedForest has crashed.", e);
	}
	exit(1);
}
