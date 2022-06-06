// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <PABLO.MADRIGALRAMIREZ@ucr.ac.cr>

/*
 * credits -> https://solarianprogrammer.com/2019/01/13/cpp-17-filesystem-write-file-watcher-monitor/
 * https://github.com/sol-prog/cpp17-filewatcher
 */

// Compilation example for GCC (v8 and up), Clang (v7 and up) and MSVC
// g++ -std=c++17 -Wall -pedantic test_fs_watcher.cpp -o test_fs_watcher -lstdc++fs
// clang++ -std=c++17 -stdlib=libc++ -Wall -pedantic test_fs_watcher.cpp -o test_fs_watcher -lc++fs
// cl /W4 /EHsc /std:c++17 /permissive- test_fs_watcher.cpp

#pragma once

#include <iostream>
#include "FileWatcher.h"
static const string path_to_watch{ "./input" };
static const chrono::duration<int, std::milli> delay{ 5000 };
static const string validTxt{ path_to_watch + "/tetris_state.txt" };

class initFileWatcher {
public:
    void listenEvent();
};

void initFileWatcher::listenEvent() {
    // Create a FileWatcher instance that will check the current folder for changes every 5 seconds
    FileWatcher fw{path_to_watch, delay};

    // Start monitoring a folder for changes
    // run a user provided lambda function
    fw.start([] (std::string path_to_watch, FileStatus status) -> void {
        // Process only regular files, all other file types are ignored
        if(!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != FileStatus::erased) {
            return;
        }
            switch(status) {
                case FileStatus::created:
                    std::cout << "File created: " << path_to_watch << '\n';
                    if (path_to_watch.compare(validTxt) == 0) {
                        std::cout << "Aca va el llamado a la funcion del Filer\n";
                    }
                    break;
                case FileStatus::modified:
                    std::cout << "File modified: " << path_to_watch << '\n';
                    break;
                case FileStatus::erased:
                    std::cout << "File erased: " << path_to_watch << '\n';
                    break;
                default:
                    std::cout << "Error! Unknown file status.\n";
            }
    });
}