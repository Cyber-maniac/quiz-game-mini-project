# Maintainer Memo & Future Update Guide

> **Note to Future Maintainers & AI Assistants**:  
> This file is a persistent memory guide documenting the architecture, design choices, common pitfalls, and step-by-step instructions for extending and modifying this C Quiz Game project.

---

## 1. System Architecture & Core Modules

The project is designed as a standalone, zero-dependency C99 console application.

### Key Data Structures

1. **`Question` struct (`quiz.c`)**:
   ```c
   typedef struct {
       char category[32];          // Category name (e.g. "C & Computer Science")
       char question[256];         // Question prompt
       char options[MAX_OPTIONS][128]; // Array of 4 answer choices
       char correctOption;         // 'A', 'B', 'C', or 'D'
       char explanation[256];      // Educational explanation shown after answering
   } Question;
   ```

2. **`HighScore` struct (`quiz.c`)**:
   ```c
   typedef struct {
       char name[32];
       char category[32];
       int score;
       int total;
       double percentage;
       char date[24];
   } HighScore;
   ```

### Major Subsystems

- **Windows Console ANSI Driver (`initConsole`)**:  
  Windows Command Prompt / PowerShell consoles do not enable virtual terminal processing (VT100 / ANSI colors) by default. `initConsole()` queries `GetStdHandle(STD_OUTPUT_HANDLE)` and sets `ENABLE_VIRTUAL_TERMINAL_PROCESSING` (`0x0004`). On non-Windows platforms, ANSI escapes work natively.
- **Safe Input Engine (`getUserChoiceWithLifeline`)**:  
  Never use raw `scanf("%c")` or `scanf("%s")`. `fgets` with a 64-byte buffer is used to prevent buffer overflow and eliminate dangling `\n` characters in `stdin`. Leading whitespace is skipped, input is uppercased via `toupper()`, and trailing characters are validated. Handles `EOF` cleanly to avoid infinite loops during automated piping/testing.
- **50:50 Lifeline Algorithm**:  
  When player presses `L` or `5`, the engine collects the indices of the 3 incorrect options, uses Fisher-Yates shuffle with `rand()`, and marks 2 of them in an `eliminated[4]` boolean array. The question is re-rendered showing `~~ ELIMINATED ~~` in red.
- **Leaderboard Engine (`recordHighScore` / `showLeaderboard`)**:  
  Persists records as comma-separated values (CSV) in `highscores.txt`. User input is sanitized to replace commas with spaces. `showLeaderboard` parses entries using `strtok`, loads them into memory, and sorts descending by accuracy percentage before displaying the Top 15.

---

## 2. Important Gotchas & Pitfalls to Remember

1. **Windows Executable Locking (`Permission denied`)**:
   - If `gcc -o quiz.exe quiz.c` fails with `Permission denied` / `collect2.exe: error: ld returned 1 exit status`, a previously run `quiz.exe` instance is still active in memory.
   - Run in PowerShell:
     ```powershell
     Get-Process -Name quiz -ErrorAction SilentlyContinue | Stop-Process -Force
     ```
2. **Infinite Loops on EOF**:
   - Always ensure every `fgets()` and `getchar()` checks for `NULL` or `EOF`. Piped test inputs will hang if loops don't terminate when `stdin` closes.
3. **Array Bounds**:
   - Whenever increasing the number of questions in `questionBank`, update `#define TOTAL_QUESTIONS_BANK <N>`.

---

## 3. Step-by-Step Guide for Future Updates

### A. How to Add an External File Question Loader (`questions.txt`)
Instead of hardcoding questions into `questionBank`, you can add a dynamic loader:
1. Define a text format:
   ```text
   Category|Question Text|Option A|Option B|Option C|Option D|CorrectLetter|Explanation
   ```
2. In `quiz.c`, write `int loadQuestionsFromFile(const char *filename, Question **outList)`:
   - Use `fopen(filename, "r")`.
   - Read lines with `fgets`, parse with `strtok_r` or `sscanf`.
   - Dynamically allocate with `malloc` or fill a static buffer.

### B. How to Add a Countdown Timer (e.g., 15 Seconds per Question)
- **Windows implementation**:
  - Use `_kbhit()` and `_getch()` from `<conio.h>`.
  - Loop with `Sleep(100)` checking elapsed time using `time(NULL)` or `clock()`.
- **POSIX implementation**:
  - Use `select()` or `poll()` on `STDIN_FILENO` with a `struct timeval` timeout.

### C. How to Add Audio / Beep Feedback
- In C on Windows, you can include `<windows.h>` and call:
  - Correct answer sound: `Beep(1000, 150); Beep(1500, 200);`
  - Wrong answer sound: `Beep(400, 300);`
- Across generic terminals: `printf("\a");` triggers the system bell.

### D. Adding More Lifelines
- **"Ask the Audience" / Hint**:
  - Add a `char hint[128]` field to `Question`.
  - Let player press `'H'` to view the hint.
- **"Skip Question"**:
  - Replace the current question without deducting points.

---

## 4. Build & Test Commands

- **Compile**:
  ```powershell
  gcc -Wall -Wextra -std=c99 -o quiz.exe quiz.c
  ```
- **Automated Smoke Test (PowerShell)**:
  ```powershell
  "1`n1`n`nA`n`nB`n`nC`n`nD`n`nB`n`nTester`n`n5`n" | .\quiz.exe
  ```
