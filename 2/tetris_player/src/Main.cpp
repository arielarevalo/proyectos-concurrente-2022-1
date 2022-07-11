// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.

#include <unistd.h>
#include <omp.h>

#include "./include/TetrisSolverParallel.hpp"
#include "./include/Logger.hpp"

using namespace std;

/**
 * @brief Entry point.
 */
int main(int argc, char* argv[])
{
	long numThreads{ sysconf(_SC_NPROCESSORS_ONLN) };

	if (argc > 1)
	{
		string path{ argv[1] };

		if (argc > 2)
		{
			numThreads = *argv[2] - '0';
			omp_set_num_threads(numThreads);
		}

		try
		{
			TetrisSolverParallel::solve(numThreads, path);
		}
		catch (const std::exception& e)
		{
			Logger::error("TetrisSolverParallel has crashed.", e);
		}
		exit(1);
	}
	else
	{
		Logger::error("No path provided.");
		exit(1);
	}
}
