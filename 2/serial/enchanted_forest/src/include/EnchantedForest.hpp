// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <string>

#include "./Logger.hpp"
#include "./MapFiler.hpp"
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
			MapFiler filer{ jobPath };
			Job job{ filer.parseJob() };
			Logger::info("Successfully read job from file " + jobPath);

			Logger::info("Processing job maps.");
			Logger::setStart();
			for (const Map& m : job)
			{
				MapWriter writer{ m.id, m.area, m.isTraced, m.finalTime };

				while (writer.step())
				{
					filer.file(writer.write());
				}
				Logger::info(
						"Successfully simulated " + std::to_string(m.finalTime)
								+ " nights for map " + std::to_string(m.id));
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