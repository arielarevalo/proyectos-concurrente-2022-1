// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#include <mpi.h>
#include <omp.h>
#include <unistd.h>
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
	try
	{
		if (argc < 2)
		{
			throw invalid_argument("No path provided.");
		}

	}
	catch (invalid_argument& ia)
	{
		Logger::error("Invalid number of arguments", ia);

		exit(1);
	}

	// TODO(aarevalo): if arg path is relative -> absolute(const
	//  std::filesystem::path& p) and overwrite argv[2]. Else, nothing

	if (MPI_Init(&argc, &argv) == MPI_SUCCESS)
	{
		try
		{
			int numThreads{ static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN)) };

			string path{ argv[1] };

			if (argc > 2)
			{
				try
				{
					numThreads = stoi(argv[2]);
				}
				catch (out_of_range& oor)
				{
					Logger::error("Invalid number of threads", oor);
				}
			}

			omp_set_num_threads(numThreads);

			EnchantedForest::process(path);
		}
		catch (exception& e)
		{
			Logger::error("EnchantedForest has crashed.", e);
		}
	}
	else
	{
		Logger::error("MPI could not initialize.");
	}

	MPI_Finalize();

	exit(0);
}
