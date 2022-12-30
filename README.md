# advent-of-code-2021

My solutions to Advent of Code 2021 written in C++. The development environment is Visual Studio and CMake on Windows.

## Day 1

| Part | Answer |
|------|--------|
|    1 |   1226 |
|    2 |   1252 |

## Day 2

| Part |   Answer   |
|------|------------|
|    1 |    1746616 |
|    2 | 1741971043 |

## Day 3

| Part |  Answer |
|------|---------|
|    1 |  749376 |
|    2 | 2372923 |

## Day 4

| Part | Answer |
|------|--------|
|    1 |  41668 |
|    2 |  10478 |

## Day 5

| Part | Answer |
|------|--------|
|    1 |   6548 |
|    2 |  19663 |

## Day 6

| Part |     Answer    |
|------|---------------|
|    1 |        365862 |
|    2 | 1653250886439 |

## Day 7

| Part |   Answer  |
|------|-----------|
|    1 |    345197 |
|    2 |  96361606 |

## Day 8

| Part |  Answer  |
|------|----------|
|    1 |      532 |
|    2 |  1011284 |

## Day 9

| Part |  Answer |
|------|---------|
|    1 |     448 |
|    2 | 1417248 |

## Day 10

| Part |   Answer   |
|------|------------|
|    1 |     344193 |
|    2 | 3241238967 |

## Day 11

| Part |  Answer |
|------|---------|
|    1 |    1735 |
|    2 |     400 |

## Day 12

| Part | Answer |
|------|--------|
|    1 |   5178 |
|    2 | 130094 |

## Day 13

| Part |  Answer  |
|------|----------|
|    1 |      610 |
|    2 | PZFJHRFZ |

**Note**: *The answer to part 2 is found visually by inspecting the ascii image output.*

## Day 14

| Part |     Answer    |
|------|---------------|
|    1 |          2915 |
|    2 | 3353146900153 |

## Day 15

| Part |  Answer  |
|------|----------|
|    1 |      604 |
|    2 |     2907 |

## Day 16

| Part |     Answer    |
|------|---------------|
|    1 |           895 |
|    2 | 1148595959144 |

## Day 17

| Part |  Answer  |
|------|----------|
|    1 |    10878 |
|    2 |     4716 |

## Day 18

| Part | Answer |
|------|--------|
|    1 |   4145 |
|    2 |   4855 |

## Day 19

| Part | Answer |
|------|--------|
|    1 |    332 |
|    2 |   8507 |

## Day 20

| Part | Answer |
|------|--------|
|    1 |   5419 |
|    2 |  17325 |

## Day 21

| Part |      Answer     |
|------|-----------------|
|    1 |          734820 |
|    2 | 193170338541590 |

**Note**: *Part 1 is `day21-1.cpp` and part 2 is implemented in `day21-2.cpp`.*

## Day 22

| Part |      Answer      |
|------|------------------|
|    1 |           546724 |
|    2 | 1346544039176841 |

**Note**: *Part 1 is `day22-1.cpp` and part 2 is implemented in `day22-2.cpp`.*

## Day 23

I nearly gave up on this puzzle. I initially used the pathfinding class but it was too slow. I then tried to implement a depth first search with pruning and memoization, but it was even slower. Eventually with the help of Claude, I was able to fix some inefficiencies in the pathfinding implementation, reduce memory allocation costs, and improve the heuristic enough to solve part 2 in an acceptable amount of time.

| Part | Answer |
|------|--------|
|    1 |  10607 |
|    2 |  59071 |

**Note**: *The part flag in the command line is ignored. The part is determined by setting `PART_1` in the source file.*

Also note: *Part 2 takes over 20 seconds to complete when running the optimized version.*

## Day 24

No code is written for day 24. The solution is found only by analysis of the input.

The ALU uses `* 26`, `/ 26`, and `% 26` to manipulate base-26 numbers, represented here as a little-endian vector of digits.

The processing of each of the model number digits is similar. One of the following functions is executed for each model digit, *d_n*. The function is selected by hard-coded constants (not shown). *z_n* is the value of the Z register after processing *d_n*. The constants *a_n*, and *b_n* are hard-coded in the input.

  - `z_n = [d_n + b_n | z_n-1]`
  - `z_n = (z_n-1[0] + a_n != d_n) ? [d_n + b_n | z_n-1[1..]] : z_n-1[1..]`

As a result, there are 7 model number digits that increase the number of digits in *z_n* and 7 that either reduce the number of digits or keep it the same.

```
z_01 = [d_01 + 13]
z_02 = [d_02 + 10 | z_01]     
z_03 = [d_03 +  3 | z_02]     
z_04 = (z_03[0] - 11 != d_04) ? [d_04 +  1 | z_03[1..]] : z_03[1..]
z_05 = [d_05 +  9 | z_04]     
z_06 = (z_05[0] -  4 != d_06) ? [d_06 +  3 | z_05[1..]] : z_05[1..]
z_07 = [d_07 +  5 | z_06]     
z_08 = [d_08 +  1 | z_07]     
z_09 = [d_09 +  0 | z_08]     
z_10 = (z_09[0] -  2 != d_10) ? [d_10 + 13 | z_09[1..]] : z_09[1..]
z_11 = (z_10[0] -  5 != d_11) ? [d_11 +  7 | z_10[1..]] : z_10[1..]
z_12 = (z_11[0] - 11 != d_12) ? [d_12 + 15 | z_11[1..]] : z_11[1..]
z_13 = (z_12[0] - 13 != d_13) ? [d_13 + 12 | z_12[1..]] : z_12[1..]
z_14 = (z_13[0] - 10 != d_14) ? [d_14 +  8 | z_13[1..]] : z_13[1..]
```

The solution requires the final value in the Z register, *z_14*, to be 0. Since *z_14* cannot have a single digit with a value of 0, it must have no digits (which is the same as a  value of 0). Thus, all conditions must be false so that all functions that can reduce the number of digits do so.

```
z_14 =
  (d_01 + 13 - 10 != d_14) ?
    ...
    (d_02 + 10 - 13 != d_13) ?
      ...
      (d_07 +  5 - 11 != d_12) ?
        ...
        (d_08 +  1 -  5 != d_11) ?
          ...
          (d_09 +  0 -  2 != d_10) ?
            ...
            (d_05 +  9 -  4 != d_06) ?
              ...
              (d_03 - 8 != d_04) ?
                ...
                []
```

So, the following must be true:

- d04 = d03 - 8      
- d06 = d05 + 5
- d10 = d09 - 2
- d11 = d08 - 4
- d12 = d07 - 6
- d13 = d02 - 3
- d14 = d01 + 3

Therefore, the ranges of the digits are:

| Digit | Range |
|-------|-------|
|    1  | 1 - 6 |
|    2  | 4 - 9 |
|    3  | 9     |
|    4  | 1     |
|    5  | 1 - 4 |
|    6  | 6 - 9 |
|    7  | 7 - 9 |
|    8  | 5 - 9 |
|    9  | 3 - 9 |
|   10  | 1 - 7 |
|   11  | 1 - 5 |
|   12  | 1 - 3 |
|   13  | 1 - 6 |
|   14  | 4 - 9 |

The highest possible model number is `69914999975369` and the lowest possible model number is `14911675311114`.

| Part |     Answer     |
|------|----------------|
|    1 | 69914999975369 |
|    2 | 14911675311114 |

## Day 25

| Part | Answer |
|------|--------|
|    1 |    530 |
|    2 |    N/A |

**Note**: *Day 25 does not have a part 2.*