#include <aoc_session.h>
#include <fstream>
#include <iostream>
#include <fmt/ostream.h>
#include <fmt/color.h>

static const std::string kAnswerPrefix = "Your puzzle answer was <code>";
static const std::string kAnswerPostfix = "</code>";

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
        fmt::println(std::cerr, "{}", fmt::styled("Error: TLS check failed", fmt::fg(fmt::color::red)));
        return false;
    }
    if (r.status_code >= 400) {
        fmt::println(std::cerr, "{} {}", fmt::styled("Error: GET status code", fmt::fg(fmt::color::red)),
                     fmt::styled(r.status_code, fmt::fg(fmt::color::red)));
        return false;
    }
    return true;
}

AdventOfCodeSession::AdventOfCodeSession(int yearNum, int dayNum)
: yearNum{yearNum},
  dayNum{dayNum},
  inputFileName{fmt::format("day{:02d}.txt", dayNum)},
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
    fmt::println("\n---AoC {} day {} part {}---\n  Your answer is {}", yearNum, dayNum, part,
                 fmt::styled(answer, fmt::fg(fmt::color::yellow)));
    if (part <= cachedAnswers.size()) { // cached
        auto& correctAns = cachedAnswers[part - 1];
        fmt::print("  You have solved this before. The answer was {}. New answer is {}.",
                   fmt::styled(correctAns, fmt::fg(fmt::color::magenta)),
                   answer == correctAns ? fmt::styled("right", fmt::fg(fmt::color::green))
                                        : fmt::styled("wrong", fmt::fg(fmt::color::red)));
    } else {
        cpr::Response r = cpr::Post(cpr::Url{baseUrl + "/answer"}, cpr::Cookies{{"session", sessionCookie}},
                                    cpr::Payload{{"level", std::to_string(part)}, {"answer", answer}});
        if (!isGoodResponse(r)) return false;
        fmt::println("  Reply from AoC: {}", getAnswerResult(r.text));
    }
    fmt::println("------------");
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