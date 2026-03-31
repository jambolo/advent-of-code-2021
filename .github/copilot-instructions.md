# Project Custom Instructions (Advent of Code 2021)

## Token Discipline

- Be concise by default.
- No explanations unless explicitly requested.
- No restating the question.
- No summaries at the end.
- Use bullet points only when clarity improves.
- Prefer short sentences.
- Assume reader is expert.

## Output Rules

- Answer the question directly.
- Do not add context, background, or alternatives unless asked.
- If uncertain, say "unknown" or ask one clarifying question.

## Code

- Output code only, no commentary.
- Prefer minimal, idiomatic solutions.
- Limit comments to very brief descriptions of what the code does, without describing why changes were made.

## Interaction

- Ask at most one clarifying question.
- Never suggest next steps unless requested.

## Project Conventions

- Target language: C++17.
- Build system: CMake (minimum 3.10), generator: Ninja.
- Prefer solution-focused implementations per day/part.
- Keep solutions deterministic and fast enough for full input execution.
- Avoid adding third-party dependencies unless explicitly requested.
- Reuse existing project helpers/utilities before introducing new abstractions.
- Follow existing naming/style conventions in nearby files.
- Make minimal changes; do not refactor unrelated code.
- Preserve current folder structure and CMake layout.
- When adding new source files, update CMakeLists.txt accordingly.
- Prefer standard library algorithms/containers over custom infrastructure.
- Handle input parsing robustly (trim lines, handle trailing newline/empty lines).
- Keep I/O simple: read puzzle input from existing project input conventions.
- For each puzzle solution, keep part 1 and part 2 logic clearly separated.
- If optimizing, prioritize readability first, then targeted performance improvements.
- Validate changes by building with:
  - cmake -S . -B build -G Ninja
  - cmake --build build
- Do not change compiler standard, generator assumptions, or global build options unless requested.