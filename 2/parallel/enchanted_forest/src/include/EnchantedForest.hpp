// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <string>

#include "./Logger.hpp"
#include "./MapFiler.hpp"
#include "./MapWriter.hpp"

/**
 * @brief Entry point for the Enchanted Forest algorithm.
 */
class EnchantedForest
{
public:
	/**
	 * @brief Enchanted Forest algorithm. Executes the given job.
	 * @param jobPath Path to job to execute.
	 */
	static void process(const std::string& jobPath);
};

void EnchantedForest::process(const std::string& jobPath)
{
    Logger::setStart();
	if (!jobPath.empty())
	{
		Logger::info("Processing " + jobPath);

		try
		{
            MapFiler filer{ jobPath };
            Job job{ filer.parseJob() };
            Logger::info("Successfully read job from file " + jobPath);

            Logger::info("Processing job maps.");
            for (const std::string& task : job)
            {
                Map map{ filer.parseMap(task) };

                MapWriter writer{ map };

                while (writer.step())
                {
                    filer.file(writer.write());
                }
                Logger::info(
                        "Successfully simulated "
                        + std::to_string(map.finalTime)
                        + " nights for map " + map.id);
            }
		}
		catch (const std::invalid_argument& ia)
		{
			Logger::error("Failed to validate input file.", ia);
		}
		catch (const std::out_of_range& oor)
		{
			Logger::error("Failed to validate input file contents.", oor);
		}
		catch (const std::domain_error& de)
		{
			Logger::error("Failed to find best moves.", de);
		}
		catch (const std::ios::failure& iof)
		{
			Logger::error("Failed to open/close file: "
					+ std::string(std::strerror(errno)), iof);
		}
		catch (const std::exception& e)
		{
			Logger::error("MapWriter has crashed.", e);
		}
	}
}