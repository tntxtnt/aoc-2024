#include <aoc/session.h>
#include <fstream>
#include <iostream>
#include <fmt/ostream.h>
#include <fmt/color.h>

constexpr std::string_view kAnswerPrefix = "Your puzzle answer was <code>";
constexpr std::string_view kAnswerPostfix = "</code>";

static auto getContentInsideTag(const std::string& html, const std::string& tag) -> std::string {
    auto openTag = fmt::format("<{}>", tag);
    auto closeTag = fmt::format("</{}>", tag);
    auto i = html.find(openTag) + openTag.size();
    auto j = html.find(closeTag);
    return html.substr(i, j - i);
}

static auto stripTagsAndWhiteSpace(const std::string& html) -> std::string {
    std::string res;
    res.reserve(html.size());
    size_t i = 0;
    while (std::isspace(html[i]) != 0) ++i;
    for (bool ignoringTag = false; i < html.size(); ++i) {
        const char c = html[i];
        if (ignoringTag) {
            ignoringTag = c != '>';
        } else if (c == '<') {
            ignoringTag = true;
        } else {
            res += c;
        }
    }
    while (!res.empty() && (std::isspace(res.back()) != 0)) res.pop_back();
    return res;
}

static auto getAnswerResult(const std::string& html) -> std::string {
    if (html.empty()) return "Failed - text is empty.";
    return stripTagsAndWhiteSpace(getContentInsideTag(html, "main"));
}

static auto isGoodResponse(const cpr::Response& r) -> bool {
    if (r.status_code == 0) { // failed to verify ssl
        fmt::println(std::cerr, "{}", fmt::styled("Error: TLS check failed", fg(fmt::color::red)));
        return false;
    }
    if (r.status_code >= 400) {
        fmt::println(std::cerr, "{} {}", fmt::styled("Error: GET status code", fg(fmt::color::red)),
                     fmt::styled(r.status_code, fg(fmt::color::red)));
        return false;
    }
    return true;
}

AdventOfCodeSession::AdventOfCodeSession(int yearNum, int dayNum)
: inputFileName{fmt::format("day{:02d}.txt", dayNum)},
  yearNum{yearNum},
  dayNum{dayNum},
  baseUrl{fmt::format("https://adventofcode.com/{}/day/{}", yearNum, dayNum)},
  sessionCookie{std::getenv("ADVENTOFCODE_SESSION_COOKIE")} {}

auto AdventOfCodeSession::fetchInput() const -> bool {
    if (std::ifstream ifs{inputFileName}) return true;
    cpr::Response r = cpr::Get(cpr::Url{baseUrl + "/input"}, cpr::Cookies{{"session", sessionCookie}});
    if (!isGoodResponse(r)) return false;
    if (std::ofstream ofs{inputFileName}) {
        ofs << r.text;
        return true;
    }
    return false;
}

auto AdventOfCodeSession::checkAnswer(int part, const std::string& answer) -> bool {
    // check local storage
    const auto cachedAnswersFilename = fmt::format("day{:02d}-answers.txt", dayNum);
    std::vector<std::string> cachedAnswers;
    if (std::ifstream ifs{cachedAnswersFilename}) {
        std::string line;
        for (size_t i = 0; i < 2; ++i)
            if (std::getline(ifs, line) && !line.empty()) cachedAnswers.emplace_back(std::move(line));
    }
    // check solved
    if (cachedAnswers.size() < 2) {
        cachedAnswers = fetchAnswers();
        if (!cachedAnswers.empty()) {
            if (std::ofstream ofs{cachedAnswersFilename})
                for (const auto& s : cachedAnswers) ofs << s << "\n";
        }
    }
    // check for answer
    fmt::println("-------------------------------------------------------------------------------");
    using fmt::color;
    fmt::print("{}", fmt::styled(fmt::format("AoC {} ", yearNum), bg(color::midnight_blue) | fg(color::lime)));
    fmt::print("{}", fmt::styled(fmt::format("day {} ", dayNum), bg(color::midnight_blue) | fg(color::deep_sky_blue)));
    fmt::println("{}", fmt::styled(fmt::format("part {}", part), bg(color::midnight_blue) | fg(color::violet)));
    fmt::println("  Your answer is {}", fmt::styled(answer, fg(color::gold)));
    if (part <= (int)cachedAnswers.size()) { // cached
        auto& correctAns = cachedAnswers[part - 1];
        fmt::println("  You have solved this part before. The answer is {}. New answer is {}.",
                     fmt::styled(correctAns, fg(color::violet)),
                     answer == correctAns ? fmt::styled("right", fg(color::lime))
                                          : fmt::styled("wrong", fg(color::red)));
    } else {
        cpr::Response r = cpr::Post(cpr::Url{baseUrl + "/answer"}, cpr::Cookies{{"session", sessionCookie}},
                                    cpr::Payload{{"level", std::to_string(part)}, {"answer", answer}});
        if (!isGoodResponse(r)) return false;
        fmt::println("  {} {}", fmt::styled("[Reply from AoC]", fg(color::deep_sky_blue)), getAnswerResult(r.text));
    }
    return true;
}

auto AdventOfCodeSession::fetchAnswers() const -> std::vector<std::string> {
    std::vector<std::string> res;
    cpr::Response r = cpr::Get(cpr::Url{baseUrl}, cpr::Cookies{{"session", sessionCookie}});
    if (!isGoodResponse(r)) return res;
    auto content = getContentInsideTag(r.text, "main");
    auto i = content.find(kAnswerPrefix);
    if (i != std::string::npos) {
        i += kAnswerPrefix.size();
        auto j = content.find(kAnswerPostfix, i);
        res.emplace_back(content.substr(i, j - i));
        i = content.find(kAnswerPrefix, j);
        if (i != std::string::npos) {
            i += kAnswerPrefix.size();
            j = content.find(kAnswerPostfix, i);
            res.emplace_back(content.substr(i, j - i));
        }
    }
    return res;
}