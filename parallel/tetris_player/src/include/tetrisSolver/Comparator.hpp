// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

class Comparator {
public:

    /**
     * @brief Score the current play state and compare with previous top
     * score to replace it if higher.
     * @param current Play state being scored.
     * @return Whether first is higher score than second.
     */
    static bool compare(const PlayState& first, const PlayState& second);
};

bool Comparator::compare(const PlayState& first, const PlayState& second)
{
    bool isHighScore{ false };

    if (history.empty())
    {
        history.push_back(current);
        isHighScore = true;
    }
    else
    {
        unsigned int currentScore{ current.score() };
        unsigned int topScore{ history.front().score() };

        if (currentScore > topScore)
        {
            Logger::info("New High Score:");
            std::cout << "\t" << currentScore << std::endl;
            current.getPlayArea().print();
            Logger::info("Old High Score:");
            std::cout << "\t" << topScore << std::endl;
            history.front().getPlayArea().print();

            history.clear();
            history.push_back(current);
            isHighScore = true;
        }
    }
    return isHighScore;
}
