Build and run day ${DAY}.

Rules:
- Use CMake + Ninja from repo root.
- Configure if needed: cmake -S . -B build -G Ninja
- Build only target day${DAY}: cmake --build build --target day${DAY}
- Run executable for that target for parts 1 and 2.
- The command line arguments for the executable are `-i .\day${DAY}\day${DAY}-input.txt -p ${PART}`.
- Show exact commands and then execute them in __View > Terminal__.
- If build fails, stop and show first error only.
- Show the output of each part in the terminal and then summarize the answers in a table like this:
  ```
  | Part | Answer |
  |------|--------|
  |    1 |   ???  |
  |    2 |   ???  |
  ```

