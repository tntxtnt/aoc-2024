#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <iostream>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <imgui.h>
#include <imgui_stdlib.h>
#include <implot.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <aoc_session.h>
#include "constants.h"
#include "day01_visualization.h"

Day1Visualization::Day1Visualization() {
    AdventOfCodeSession session(2024, 1);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            for (int n1, n2; ifs >> n1 >> n2;) {
                list1.push_back(n1);
                list2.push_back(n2);
            }
        } else {
            fmt::println(std::cerr, "{}",
                         fmt::styled("Input file doesn't exists or can't be read", fg(fmt::color::red)));
            std::exit(1);
        }
    }
    sortedList1 = list1;
    std::ranges::sort(sortedList1);
    sortedList2 = list2;
    std::ranges::sort(sortedList2);

    for (auto [i, n] : views::enumerate(list1)) {
        for (auto [j, m] : views::enumerate(list2)) {
            if (n == m) {
                xList1IndexInList2.push_back((int)i);
                yList1IndexInList2.push_back((int)j);
            }
        }
    }
    for (auto [i, n] : views::enumerate(list1)) {
        for (auto [j, m] : views::enumerate(sortedList2)) {
            if (n == m) {
                xList1IndexInSortedList2.push_back((int)i);
                yList1IndexInSortedList2.push_back((int)j);
            }
        }
    }
    for (auto [i, n] : views::enumerate(sortedList1)) {
        for (auto [j, m] : views::enumerate(list2)) {
            if (n == m) {
                xSortedList1IndexInList2.push_back((int)i);
                ySortedList1IndexInList2.push_back((int)j);
            }
        }
    }
    for (auto [i, n] : views::enumerate(sortedList1)) {
        for (auto [j, m] : views::enumerate(sortedList2)) {
            if (n == m) {
                xSortedList1IndexInSortedList2.push_back((int)i);
                ySortedList1IndexInSortedList2.push_back((int)j);
            }
        }
    }
}

void Day1Visualization::drawPart1() {
    if (ImPlot::BeginPlot("Unsorted", {1500, 400})) {
        ImPlot::SetupAxes("point index", "point value");
        ImPlot::SetupAxesLimits(-5, (int)list1.size() + 5, 8000, 102000, ImPlotCond_Once);
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
        ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 2.0f);
        ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, 0);
        ImPlot::PlotScatter("List 1", &list1[0], (int)list1.size());
        ImPlot::PlotScatter("List 2", &list2[0], (int)list2.size());
        ImPlot::EndPlot();
    }
    ImGui::Dummy({1, 12});
    if (ImPlot::BeginPlot("Sorted", {1500, 400})) {
        ImPlot::SetupAxes("point index", "point value");
        ImPlot::SetupAxesLimits(-5, (int)sortedList1.size() + 5, 8000, 102000, ImPlotCond_Once);
        ImPlot::PlotScatter("List 1", &sortedList1[0], (int)sortedList1.size());
        ImPlot::PlotScatter("List 2", &sortedList2[0], (int)sortedList2.size());
        ImPlot::PopStyleVar(3);
        ImPlot::EndPlot();
    }
}

void Day1Visualization::drawPart2() {
    constexpr int axisOffset = 25;
    constexpr int plotW = 420;
    constexpr int plotH = 420;
    if (ImPlot::BeginPlot("Unsorted", {plotW, plotH})) {
        ImPlot::SetupAxes("list 1 index", "list 2 index");
        ImPlot::SetupAxesLimits(-axisOffset, (int)list1.size() + axisOffset, -axisOffset,
                                (int)list1.size() + axisOffset, ImPlotCond_Once);
        ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 2.0f);
        ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, 0);
        ImPlot::PlotScatter("List 1 in list 2", &xList1IndexInList2[0], &yList1IndexInList2[0],
                            (int)xList1IndexInList2.size());
        ImPlot::EndPlot();
    }
    ImGui::SameLine();
    if (ImPlot::BeginPlot("Sorted list 1", {plotW, plotH})) {
        ImPlot::SetupAxes("sorted list 1 index", "list 2 index");
        ImPlot::SetupAxesLimits(-axisOffset, (int)list1.size() + axisOffset, -axisOffset,
                                (int)list1.size() + axisOffset, ImPlotCond_Once);
        ImPlot::PlotScatter("List 1 in list 2", &xSortedList1IndexInList2[0], &ySortedList1IndexInList2[0],
                            (int)xSortedList1IndexInList2.size());
        ImPlot::EndPlot();
    }
    if (ImPlot::BeginPlot("Sorted list 2", {plotW, plotH})) {
        ImPlot::SetupAxes("list 1 index", "sorted list 2 index");
        ImPlot::SetupAxesLimits(-axisOffset, (int)list1.size() + axisOffset, -axisOffset,
                                (int)list1.size() + axisOffset, ImPlotCond_Once);
        ImPlot::PlotScatter("List 1 in list 2", &xList1IndexInSortedList2[0], &yList1IndexInSortedList2[0],
                            (int)xList1IndexInSortedList2.size());
        ImPlot::EndPlot();
    }
    ImGui::SameLine();
    if (ImPlot::BeginPlot("Both sorted", {plotW, plotH})) {
        ImPlot::SetupAxes("sorted list 1 index", "sorted list 2 index");
        ImPlot::SetupAxesLimits(-axisOffset, (int)list1.size() + axisOffset, -axisOffset,
                                (int)list1.size() + axisOffset, ImPlotCond_Once);
        ImPlot::PlotScatter("List 1 in list 2", &xSortedList1IndexInSortedList2[0], &ySortedList1IndexInSortedList2[0],
                            (int)xSortedList1IndexInSortedList2.size());
        ImPlot::PopStyleVar(2);
        ImPlot::EndPlot();
    }
}

void Day1Visualization::draw() {
    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({kWindowW, kWindowH}, ImGuiCond_Always);
    ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
    ImGui::Begin("Advent of Code 2024 - Day 1", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar);
    ImGui::BeginTabBar("#tabs");
    if (ImGui::BeginTabItem("Part 1")) {
        drawPart1();
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Part 2")) {
        drawPart2();
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
    ImGui::End();
}
