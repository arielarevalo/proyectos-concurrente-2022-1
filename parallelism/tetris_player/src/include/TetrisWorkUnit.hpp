// Copyright 2022 Ariel Arevalo Alvarado <ariel.arevalo@ucr.ac.cr>
// Copyright 2022 Pablo Madrigal Ramirez <pablo.madrigalramirez@ucr.ac.cr>

#pragma once

class TetrisWorkUnit {
 public:
    int number;
    double* result;

 public:
    explicit TetrisWorkUnit(int number = -1, double* result = NULL):
            number(number), result(result) {}

    inline bool operator==(const TetrisWorkUnit& other) const {
        return this->number == other.number && this->result == other.result;
    }
};
