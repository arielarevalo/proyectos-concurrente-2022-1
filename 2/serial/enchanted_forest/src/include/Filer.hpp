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
#include "Job.hpp"
#include "Map.hpp"

/*
 *  Job Filer::toJob(std::ifstream& file) <-- job000.txt | (Filer::read())
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
    //static constexpr char OUTPUT_PATH[7]{"./put/"};

    static Job toJob(const std::string &path);

    static Map toMap(const std::string &path, const int &midnight);

    static void toFile(Map map, std::ofstream &file);

};

Job Filer::toJob(const std::string &path) {

    std::ifstream file{path};

    file.exceptions(
            std::ifstream::badbit | std::ifstream::failbit);
    try
    {
        std::string inputPath{path};

        std::string outputPath{path + "/output/"};

        std::vector<Map> mapFiles;
        while (file) {
            if (!file.eof()) {
                std::string path;

                int midnight{0};

                file >> path;
                file >> midnight;

                Map map{ toMap(path, midnight) };
                mapFiles.push_back(map);
            } else {
                break;
            }
        }

        return {inputPath, outputPath, mapFiles};
    }
    catch (const std::exception& e)
    {
        throw;
    }
}

Map Filer::toMap(const std::string &path, const int &midnight) {
    std::ifstream file{path};

    file.exceptions(
            std::ifstream::badbit | std::ifstream::failbit);
    try
    {
        const int time{midnight};

        std::string id{path.substr(0, 5)};
        size_t rows{0};
        size_t cols{0};
        file >> rows;
        file >> cols;

        if (rows < 0 || cols < 0) {
            throw std::out_of_range{"Input dimensions too small."};
        }

        Matrix<char> area{rows, cols};

        for (size_t i{0}; i < rows; ++i) {
            file >> area[i];
        }

        return {id, rows, cols, area};
    }
    catch (const std::exception& e)
    {
        throw;
    }
}

void Filer::toFile(Map map, std::ofstream &file)
{
    //std::filesystem::create_directory(INITIAL_PATH);

    std::string filename{ "../bin/put/" + map.id + "-"
                          + std::to_string(map.time)
                          + ".txt" };
    file.exceptions(std::ofstream::badbit | std::ifstream::failbit);

    file << map.time << std::endl;
    map.area.print(file);
}