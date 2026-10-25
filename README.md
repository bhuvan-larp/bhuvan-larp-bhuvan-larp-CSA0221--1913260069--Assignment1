# CSA02-<RegisterNumber>-Assignment1

**Student name:** <your name>
**Register number:** <your register number>
**Course:** C Programming, Assignment 1 (CO1): Grama Jyothi Rural Electricity Co-operative billing program
**Compiler:** gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0 (GCC 13)

## Compile command used for every program

```
gcc -Wall -Wextra -std=c11 <file>.c -o <name>
```

Run each program from inside its task folder (some read input files from the current folder).

## Steps to execute and results

| Task | Folder / files | How to run | Result |
|---|---|---|---|
| 1 | `Task01/task01.c` | `gcc -Wall -Wextra -std=c11 task01.c -o task01 && ./task01` | Data inventory table, 5 rule checks all pass, text vs integer reading comparison |
| 2 | `task02_complete.c`, `task02_types.c` | compile each, run | TODO 1 gives 330 and 256 as expected; float total off by Rs 4.88 over 4,000 bills, paise total exact |
| 3 | `task03_complete.c`, `task03_const_enum.c` | compile each, run | Bills Rs 50.00 / 50.00 / 162.50 / 275.00 / 950.00 match expected; const/enum version agrees |
| 4 | `task04.c` | run; also `gcc -E task04.c` | `BAD_SQUARE(a+1)` = 9 vs `SQUARE` = 25; `BAD_MAX(i++, j)` increments twice |
| 5 | `tariff.h`, `task05.c`, `task05_runtime_debug.c` | `gcc ... task05.c` and `gcc -DDEBUG ... task05.c` | Include guard works; debug lines appear only with `-DDEBUG` |
| 6 | `task06.c`, `task06_fgets.c`, `input.txt` | `./task06 < input.txt` | scanf return value catches letters; `%d` with a long prints a wrong value |
| 7 | `task07.c` | run | Five conversion surprises explained; float error grows from Rs 0.02 to Rs 645.84 |
| 8 | `build_stages.sh` and error files | `sh build_stages.sh` | Output of -E, -S, -c, link; one compile, link and run-time error; warnings |
| 9 | `task09.c`, `test_input.txt` | `./task09 < test_input.txt` | 7 valid bills, 4 invalid lines; array/loop design agrees with the if design |
| 10 | `task10.c` | run | Float total off by Rs 140,424.00 at 800,000 bills; double and paise agree; int overflows at 60,000 bills |
| 11 | `task11.c`, `tariff.cfg` | run in the folder | Tariff revised (RATE2 230) from file with no recompile |
| 12 | `task12.c`, `tariff.txt`, `tariff_no_peak.txt` | `./task12` or `./task12 tariff_no_peak.txt` | Decision MODIFY: 32-bit Wh register, 64-bit sums, tariff from file, peak rule; bill Rs 1984.84 for the simulated month |

Each folder contains `output.txt` with the program output. Screenshots of the terminal run go in the same folder (add them yourself).

## Assumptions

- A drop in reading (`current < previous`) is a 16-bit roll-over only if the implied consumption is at most 10,000 units; otherwise it is invalid input.
- Money is stored as integer paise; the rounding rule is half up, applied once.
- Task 12 peak rule: the peak share of the month's energy charge is charged 20% more.
