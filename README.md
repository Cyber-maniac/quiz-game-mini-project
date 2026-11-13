# Quiz Game

My console-based quiz game, written in C. It has questions about C and computer science, science and astronomy, and history, plus a mixed category.

## Features

- 50:50 lifeline for each round
- Scoreboard saved in `highscores.txt`
- Explanations after each answer
- Colored terminal interface

## Build and run

Compile with GCC:

```sh
gcc -Wall -Wextra -std=c99 -o quiz quiz.c
```

On Windows, use `-o quiz.exe` and run `.\quiz.exe`. On Linux or macOS, run `./quiz`.

## Play

Choose a category from the menu and answer with `A`, `B`, `C`, or `D`. Type `L` to use the 50:50 lifeline. At the end, enter your name to save your score.

---

Created by Aditya.
