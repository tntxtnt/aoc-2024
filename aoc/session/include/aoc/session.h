#pragma once

#include <cpr/cpr.h>
#include <cpr/session.h>
#include <fmt/format.h>
#include <string>
#include <vector>

class AdventOfCodeSession {
public:
    AdventOfCodeSession(int yearNum, int dayNum);
    bool fetchInput() const;
    bool checkAnswer(int part, const std::string& answer);

private:
    std::vector<std::string> fetchAnswers() const;

public:
    const std::string inputFileName;

private:
    const int yearNum;
    const int dayNum;
    const std::string baseUrl;
    std::string sessionCookie;
};