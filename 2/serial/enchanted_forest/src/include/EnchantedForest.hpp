// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <string>

#include "./Filer.hpp"
#include "./Job.hpp"
#include "./Logger.hpp"
#include "./MapWriter.hpp"

class EnchantedForest
{
public:
	static void process(const std::string& jobPath);
};

void EnchantedForest::process(const std::string& jobPath)
{
	if (!jobPath.empty())
	{
		Logger::info("Processing " + jobPath);

		try
		{
			Job job{ Filer::toJob(jobPath) };
			Logger::info("Successfully read job from file " + jobPath);

			Logger::info("Processing job maps.");
			Logger::setStart();
			for (const Map& m : job.maps)
			{
				MapWriter writer{ m.id, m.area, m.finalTime };

				while (writer.step())
				{
					Filer::toFile(writer.write(), job.outputPath);
				}
				Logger::info(
						"Successfully simulated "
						+ std::to_string(m.finalTime) + " nights for"
						+ " map" + std::to_string(m.id) + ".txt");
			}
		}
		catch (const std::invalid_argument& ia)
		{
			Logger::error("Failed to validate input file values.", ia);
		}
		catch (const std::out_of_range& oor)
		{
			Logger::error("Failed to validate input file dimensions.", oor);
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