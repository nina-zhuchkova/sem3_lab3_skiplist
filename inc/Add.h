#pragma once
#include <random>

float probability = 0.5;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

bool Add() {
    return dis(gen) < probability;
};