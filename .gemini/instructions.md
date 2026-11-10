# Project Instructions & Context: Quiz Game in C

- **Language**: C (C99 standard).
- **Core Files**:
  - `quiz.c`: Complete console quiz application.
  - `MAINTAINER_MEMO.md`: Architectural documentation and future extension guide.
  - `highscores.txt`: CSV file storing player high scores.
  - `README.md`: User manual and compilation instructions.
- **Compilation**:
  - `gcc -Wall -Wextra -std=c99 -o quiz.exe quiz.c`
- **Rules & Conventions**:
  - Keep `quiz.c` zero-dependency (standard C libraries only + optional Windows VT100 activation).
  - Use `fgets()` for all inputs, never unchecked `scanf()`.
  - Check for existing running processes (`quiz.exe`) before rebuilding if permission errors occur.

