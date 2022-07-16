// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>.
// Copyright 2022 Pablo Madrigal Ramírez <pablo.madrigalramirez@ucr.ac.cr>.

#pragma once

#include <cstddef>
#include <deque>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <regex>
#include "Map.hpp"

using Task = std::pair<Map, int>;

using Job = std::vector<Task>;

/*
 *  Job Filer::makeJob(std::ifstream& file) <-- job000.txt | (Filer::read())
    Map Filer::toMap(std::ifstream& file) <-- map000.txt | (Filer::read())
    void Filer::toFile(Map map, std::ofstream& file) --> map000-001.txt | (Filer::write())

    Map Filer::toMap(const std::string& path) <-- map000.txt | (Filer::read())
    void Filer::toFile(Map map, const std::string& path) --> map000-001.txt | (Filer::write())

    std::ifstream file{ path };
 * */

/**
 * @brief Reads input file to game state and writes to output file with
 * history of best play state.
 */
class Filer {
public:
    Filer(const std::string &jobPath) :
            jobPath(jobPath), inputPath(findInputPath(jobPath)), outputPath(inputPath + std::string(OUTPUT)) {}

    Job makeJob();

    void toFile(Map map);

private:
    static constexpr size_t ID_START{3};

    static constexpr size_t ID_SIZE{3};

    static constexpr size_t FILENAME_SIZE{10};

    static constexpr char OUTPUT[8]{"output/"};

    static constexpr char JOB_REGEX[16]{"job[0-9]{3}.txt"};

    static std::string findInputPath(const std::string &jobPath);

    Map toMap(const std::string &filename, const int &midnight);

    const std::string jobPath;

    const std::string inputPath;

    const std::string outputPath;
};


Job Filer::makeJob() {

    std::ifstream file{jobPath};

    file.exceptions(
            std::ifstream::badbit | std::ifstream::failbit);

    Job job;
    while (file) {
        if (!file.eof()) {
            std::string filename;

            int midnight{0};

            file >> filename;

            file >> midnight;

            Map map{toMap(filename, midnight)};
            job.emplace_back(map, midnight);
        } else {
            break;
        }
    }

    return {job};
}

Map Filer::toMap(const std::string &filename, const int &midnight) {
    std::ifstream file{filename};

    file.exceptions(
            std::ifstream::badbit | std::ifstream::failbit);

    const int time{midnight};

    size_t id{ std::stoul(filename.substr(ID_START, ID_SIZE))  };
    size_t rows{0};
    size_t cols{0};
    file >> rows;
    file >> cols;

    if (rows <= 0 || cols <= 0) {
        throw std::out_of_range{"Input dimensions too small."};
    }

    Matrix<char> area{rows, cols};

    for (size_t i{0}; i < rows; ++i) {
        area[i] = file.get(char);
    }

    return {id, rows, cols, area};

}

void Filer::toFile(Map map) {
    //std::filesystem::create_directory(INITIAL_PATH);

    std::string filename{"../bin/put/" + map.id + "-"
                         + std::to_string(map.time)
                         + ".txt"};
    file.exceptions(std::ofstream::badbit | std::ifstream::failbit);

    file << map.time << std::endl;
    map.area.print(file);
}

std::string Filer::findInputPath(const std::string &jobPath) {
    std::string jobFile{jobPath.substr(jobPath.size() - FILENAME_SIZE, FILENAME_SIZE)};

    if (!std::regex_match(jobFile, std::regex(JOB_REGEX))) {
        throw std::invalid_argument("Invalid job filename.");
    }

    return jobFile.substr(0, jobPath.size() - FILENAME_SIZE);
}
