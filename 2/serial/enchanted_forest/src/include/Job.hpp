// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <utility>
#include <string>
#include <vector>
#include "./Map.hpp"

class Job {
public :

    Job(/*size_t id,*/ std::string inputPath, std::string outputPath, std::vector<Map> mapFiles)
            : /*id(id),*/ inputPath(inputPath), outputPath(outputPath), mapFiles(mapFiles) {}

    //const size_t id;

    const std::string inputPath;

    const std::string outputPath;

    const std::vector<Map> mapFiles;
};

