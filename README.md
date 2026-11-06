# Quiz Master Pro (C Edition)

A feature-rich, robust, and interactive console-based Quiz Game written in C (C99 standard).

---

## ✨ Features

- **Topic Categories**:
  1. C Programming & Computer Science
  2. Science & Astronomy
  3. History & World Trivia
  4. Grand Challenge (All 15 questions mixed)
- **50:50 Lifeline**:
  - Eliminates 2 incorrect options at random, giving players a 50% chance.
  - Can be activated once per quiz round by typing `'L'` or `'5'`.
- **Hall of Fame / Leaderboard**:
  - Automatically records player names, chosen category, score, accuracy percentage, and timestamp to `highscores.txt`.
  - In-game leaderboard viewer sorts rankings by percentage.
  - Option to clear/reset the leaderboard from the menu.
- **Vibrant ANSI Colored UI**:
  - Green for correct answers, red for incorrect/eliminated options, yellow for scores/alerts, cyan for questions/menus.
  - Auto-enables Virtual Terminal Processing (`ENABLE_VIRTUAL_TERMINAL_PROCESSING`) on Windows.
- **Robust Input Handling**:
  - Case-insensitive (accepts both `'a'` and `'A'`).
  - Ignores extra whitespace and rejects empty enters or multi-character strings without crashing or infinite loops.
  - Safe against buffer overflows (uses `fgets` throughout).
- **Instant Educational Feedback**:
  - Explains why the correct answer is right after each question.

---

## 📁 Project Structure

```text
quiz-game/
├── quiz.c              # Core source code (game engine, questions, UI, leaderboard)
├── quiz.exe            # Compiled Windows executable
├── highscores.txt      # CSV data file storing player records
├── README.md           # User documentation and guide
├── MAINTAINER_MEMO.md  # Architectural guide and roadmap for future updates
└── .gitignore          # Git exclusion rules
```

---

## 🚀 How to Compile and Run

### Windows (GCC / MinGW)

1. Open PowerShell or Command Prompt in this folder.
2. Compile:
   ```powershell
   gcc -Wall -Wextra -std=c99 -o quiz.exe quiz.c
   ```
3. Run:
   ```powershell
   .\quiz.exe
   ```

### Linux / macOS

1. Open terminal:
   ```bash
   gcc -Wall -Wextra -std=c99 -o quiz quiz.c
   ```
2. Run:
   ```bash
   ./quiz
   ```

---

## 🎮 How to Play

1. Choose a category from the menu (or pick the Grand Challenge).
2. Answer each question by typing `A`, `B`, `C`, or `D`.
3. If you get stuck on a difficult question, type `L` to trigger the **50:50 Lifeline** to eliminate 2 wrong options.
4. Review the instant explanation.
5. Enter your name at the end to save your score to the Hall of Fame!

---

## 🛠️ Adding New Questions

Questions are stored inside the `questionBank` array in [`quiz.c`](quiz.c):

```c
{
    "Category Name",
    "Your Question text here?",
    {
        "Option A",
        "Option B",
        "Option C",
        "Option D"
    },
    'A', // Correct option letter ('A', 'B', 'C', or 'D')
    "Detailed explanation of why this answer is correct."
}
```

To add questions:
1. Increase `#define TOTAL_QUESTIONS_BANK 15` in `quiz.c`.
2. Append your new question struct to `questionBank`.
3. Recompile with `gcc` and run!
