#include <vector>

struct Day1Visualization {
    std::vector<int> list1;
    std::vector<int> list2;
    std::vector<int> sortedList1;
    std::vector<int> sortedList2;
    std::vector<int> xList1IndexInList2;
    std::vector<int> yList1IndexInList2;
    std::vector<int> xList1IndexInSortedList2;
    std::vector<int> yList1IndexInSortedList2;
    std::vector<int> xSortedList1IndexInList2;
    std::vector<int> ySortedList1IndexInList2;
    std::vector<int> xSortedList1IndexInSortedList2;
    std::vector<int> ySortedList1IndexInSortedList2;

    Day1Visualization();
    void drawPart1();
    void drawPart2();
    void draw();
};
