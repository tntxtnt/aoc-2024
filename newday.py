import sys, shutil, os

def replace_day_num(filename, daynum):
    with open(filename, mode="r", encoding="utf-8") as f:
        lines = f.readlines()

    newlines = []
    for line in lines:
        line = line.replace('day01', f'day{daynum:02d}')
        line = line.replace('Day1', f'Day{daynum}')
        newlines.append(line)

    with open(filename, mode="w", encoding="utf-8") as f:
        f.writelines(newlines)


if __name__ == '__main__':
    if len(sys.argv) == 2:
        daynum = int(sys.argv[1])
    else:
        daynum = int(input("Enter day number: "))

    day = f'day{daynum:02d}'
    shutil.copytree('template', day)
    os.rename(f'{day}/day01.txt', f'{day}/{day}.txt')
    os.rename(f'{day}/main/src/day01_main.cpp', f'{day}/main/src/{day}_main.cpp')
    os.rename(f'{day}/solution/src/day01_solution.cpp', f'{day}/solution/src/{day}_solution.cpp')
    os.rename(f'{day}/solution/include/day01_solution.h', f'{day}/solution/include/{day}_solution.h')

    replace_day_num(f'{day}/CMakeLists.txt', daynum)
    replace_day_num(f'{day}/main/src/{day}_main.cpp', daynum)
    replace_day_num(f'{day}/solution/src/{day}_solution.cpp', daynum)
    replace_day_num(f'{day}/solution/include/{day}_solution.h', daynum)

    with open("CMakeLists.txt", "a") as f:
        f.write(f'add_subdirectory({day})\n')