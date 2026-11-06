#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

// ANSI Color Escape Sequences
#define CLR_RESET   "\033[0m"
#define CLR_BOLD    "\033[1m"
#define CLR_RED     "\033[1;31m"
#define CLR_GREEN   "\033[1;32m"
#define CLR_YELLOW  "\033[1;33m"
#define CLR_BLUE    "\033[1;34m"
#define CLR_MAGENTA "\033[1;35m"
#define CLR_CYAN    "\033[1;36m"
#define CLR_WHITE   "\033[1;37m"

#define MAX_OPTIONS 4
#define TOTAL_QUESTIONS_BANK 15
#define HIGHSCORE_FILE "highscores.txt"
#define MAX_HIGHSCORES 100

// Question structure
typedef struct {
    char category[32];
    char question[256];
    char options[MAX_OPTIONS][128];
    char correctOption; // 'A', 'B', 'C', or 'D'
    char explanation[256];
} Question;

// HighScore record structure
typedef struct {
    char name[32];
    char category[32];
    int score;
    int total;
    double percentage;
    char date[24];
} HighScore;

// Function prototypes
void initConsole(void);
void clearScreen(void);
void pressEnterToContinue(void);
void showMainMenu(void);
void playQuiz(void);
void showInstructions(void);
void showLeaderboard(void);
void clearLeaderboard(void);
void recordHighScore(const char *category, int score, int total);
char getUserChoiceWithLifeline(int *lifelineAvailable, const Question *q, int eliminated[MAX_OPTIONS]);
void getCurrentDate(char *buffer, size_t bufSize);

// Comprehensive Question Bank
static const Question questionBank[TOTAL_QUESTIONS_BANK] = {
    // --- CATEGORY 1: C Programming & Computer Science ---
    {
        "C & Computer Science",
        "What does CPU stand for?",
        {
            "Central Processing Unit",
            "Computer Personal Unit",
            "Central Processor Universal",
            "Control Power Unit"
        },
        'A',
        "The CPU executes program instructions and directs processing tasks."
    },
    {
        "C & Computer Science",
        "Which programming language is celebrated as the 'mother of modern languages'?",
        {
            "Python",
            "C",
            "Java",
            "Assembly"
        },
        'B',
        "C influenced C++, Java, C#, JavaScript, and many modern system languages."
    },
    {
        "C & Computer Science",
        "What is the standard size of an 'int' data type on modern 32-bit and 64-bit platforms?",
        {
            "1 byte",
            "2 bytes",
            "4 bytes",
            "8 bytes"
        },
        'C',
        "Standard modern architectures utilize 4 bytes (32 bits) for standard integer types."
    },
    {
        "C & Computer Science",
        "Which data structure follows the LIFO (Last In, First Out) principle?",
        {
            "Queue",
            "Linked List",
            "Tree",
            "Stack"
        },
        'D',
        "A Stack operates under LIFO, where the last item pushed is the first to be popped."
    },
    {
        "C & Computer Science",
        "In C, which unary operator yields the memory address of a variable?",
        {
            "* (Asterisk)",
            "& (Ampersand)",
            "% (Percent)",
            "# (Hash)"
        },
        'B',
        "The ampersand (&) is the address-of operator in C."
    },

    // --- CATEGORY 2: Science & Astronomy ---
    {
        "Science & Astronomy",
        "Which planet in our solar system is famously known as the 'Red Planet'?",
        {
            "Venus",
            "Jupiter",
            "Mars",
            "Mercury"
        },
        'C',
        "Mars appears reddish due to iron oxide (rust) widespread across its surface."
    },
    {
        "Science & Astronomy",
        "What cellular organelle is nicknamed the 'powerhouse of the cell'?",
        {
            "Ribosome",
            "Mitochondria",
            "Nucleus",
            "Endoplasmic Reticulum"
        },
        'B',
        "Mitochondria generate most of the chemical energy (ATP) needed by cells."
    },
    {
        "Science & Astronomy",
        "Which chemical element corresponds to the chemical symbol 'Au'?",
        {
            "Silver",
            "Gold",
            "Copper",
            "Iron"
        },
        'B',
        "'Au' comes from 'Aurum', the Latin word for Gold, signifying shining dawn."
    },
    {
        "Science & Astronomy",
        "What is the approximate speed of light in a vacuum?",
        {
            "150,000 km/s",
            "300,000 km/s",
            "500,000 km/s",
            "1,000,000 km/s"
        },
        'B',
        "Light travels at roughly 299,792 km/s (approx. 300,000 km per second)."
    },
    {
        "Science & Astronomy",
        "Which gas do green plants primarily absorb from the air during photosynthesis?",
        {
            "Oxygen",
            "Carbon Dioxide",
            "Nitrogen",
            "Hydrogen"
        },
        'B',
        "Plants take in Carbon Dioxide (CO2) and release Oxygen (O2) via photosynthesis."
    },

    // --- CATEGORY 3: History & World Trivia ---
    {
        "History & Trivia",
        "Who created the world-famous portrait painting known as the 'Mona Lisa'?",
        {
            "Vincent van Gogh",
            "Pablo Picasso",
            "Leonardo da Vinci",
            "Michelangelo"
        },
        'C',
        "Leonardo da Vinci painted the Mona Lisa during the Italian Renaissance."
    },
    {
        "History & Trivia",
        "Which ocean is the largest and deepest on planet Earth?",
        {
            "Atlantic Ocean",
            "Indian Ocean",
            "Arctic Ocean",
            "Pacific Ocean"
        },
        'D',
        "The Pacific Ocean covers over 60 million square miles, larger than all Earth's land combined."
    },
    {
        "History & Trivia",
        "In which year did Apollo 11 make the historic first crewed landing on the Moon?",
        {
            "1965",
            "1969",
            "1972",
            "1975"
        },
        'B',
        "Neil Armstrong and Buzz Aldrin landed on the Moon on July 20, 1969."
    },
    {
        "History & Trivia",
        "What is the capital city of Japan?",
        {
            "Kyoto",
            "Osaka",
            "Tokyo",
            "Hiroshima"
        },
        'C',
        "Tokyo is the bustling capital and largest metropolitan area of Japan."
    },
    {
        "History & Trivia",
        "In which country are the iconic Great Pyramids of Giza located?",
        {
            "Egypt",
            "Greece",
            "Mexico",
            "Peru"
        },
        'A',
        "The Great Pyramids of Giza are located on the outskirts of Cairo, Egypt."
    }
};

int main(void) {
    srand((unsigned int)time(NULL));
    initConsole();

    int choice = 0;
    while (1) {
        showMainMenu();
        printf("Enter your choice (1-5): ");

        char line[32];
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        if (sscanf(line, "%d", &choice) != 1) {
            printf(CLR_RED "Invalid input! Please enter a number between 1 and 5.\n" CLR_RESET);
            pressEnterToContinue();
            continue;
        }

        switch (choice) {
            case 1:
                playQuiz();
                break;
            case 2:
                showLeaderboard();
                break;
            case 3:
                showInstructions();
                break;
            case 4:
                clearLeaderboard();
                break;
            case 5:
                printf("\n" CLR_CYAN "Thank you for playing Quiz Master Pro! Have a great day!" CLR_RESET "\n\n");
                return 0;
            default:
                printf(CLR_RED "Please enter a valid menu option (1 to 5).\n" CLR_RESET);
                pressEnterToContinue();
                break;
        }
    }

    return 0;
}

// Enables ANSI escape code processing in Windows console
void initConsole(void) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            SetConsoleMode(hOut, dwMode | 0x0004); // ENABLE_VIRTUAL_TERMINAL_PROCESSING
        }
    }
#endif
}

// Clears terminal screen or scrolls clean
void clearScreen(void) {
    printf("\033[2J\033[H");
}

// Pauses execution until Enter is pressed
void pressEnterToContinue(void) {
    printf("\n" CLR_YELLOW "Press [ENTER] to continue..." CLR_RESET);
    char buf[16];
    fgets(buf, sizeof(buf), stdin);
}

// Main Menu Display
void showMainMenu(void) {
    clearScreen();
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf(CLR_BOLD CLR_MAGENTA "                 QUIZ MASTER PRO (C EDITION)                \n" CLR_RESET);
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf("  " CLR_YELLOW "[1]" CLR_RESET " Start New Quiz\n");
    printf("  " CLR_YELLOW "[2]" CLR_RESET " View Hall of Fame / Leaderboard\n");
    printf("  " CLR_YELLOW "[3]" CLR_RESET " How to Play & Rules (Lifeline Info)\n");
    printf("  " CLR_YELLOW "[4]" CLR_RESET " Reset / Clear Leaderboard\n");
    printf("  " CLR_YELLOW "[5]" CLR_RESET " Exit Game\n");
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
}

// Instructions screen
void showInstructions(void) {
    clearScreen();
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf(CLR_BOLD CLR_YELLOW "                   HOW TO PLAY & RULES                     \n" CLR_RESET);
    printf(CLR_CYAN "============================================================\n\n" CLR_RESET);
    printf(CLR_BOLD "1. Answering Questions:" CLR_RESET "\n");
    printf("   Type " CLR_GREEN "A, B, C, or D" CLR_RESET " to submit your choice (case-insensitive).\n\n");
    printf(CLR_BOLD "2. Lifeline (50:50):" CLR_RESET "\n");
    printf("   Type " CLR_MAGENTA "'L'" CLR_RESET " or " CLR_MAGENTA "'5'" CLR_RESET " during any question to activate your 50:50 lifeline.\n");
    printf("   This will remove " CLR_RED "2 incorrect choices" CLR_RESET ", giving you a 50%% chance!\n");
    printf("   *Note: You only get " CLR_YELLOW "ONE" CLR_RESET " 50:50 lifeline per quiz round.*\n\n");
    printf(CLR_BOLD "3. Scoring:" CLR_RESET "\n");
    printf("   Each correct answer awards " CLR_GREEN "+1 Point" CLR_RESET ".\n");
    printf("   No negative marking for incorrect guesses.\n\n");
    printf(CLR_BOLD "4. Leaderboard:" CLR_RESET "\n");
    printf("   Your high score, category, and date are saved to " CLR_CYAN HIGHSCORE_FILE CLR_RESET ".\n");
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    pressEnterToContinue();
}

// Executes a Quiz session
void playQuiz(void) {
    clearScreen();
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf(CLR_BOLD CLR_YELLOW "                      CHOOSE CATEGORY                       \n" CLR_RESET);
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf("  " CLR_YELLOW "[1]" CLR_RESET " C Programming & Computer Science (5 Questions)\n");
    printf("  " CLR_YELLOW "[2]" CLR_RESET " Science & Astronomy (5 Questions)\n");
    printf("  " CLR_YELLOW "[3]" CLR_RESET " History & World Trivia (5 Questions)\n");
    printf("  " CLR_YELLOW "[4]" CLR_RESET " Grand Challenge (All 15 Questions Mixed)\n");
    printf("  " CLR_YELLOW "[5]" CLR_RESET " Return to Main Menu\n");
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf("Select category (1-5): ");

    char catLine[32];
    if (fgets(catLine, sizeof(catLine), stdin) == NULL) return;

    int catChoice = 0;
    if (sscanf(catLine, "%d", &catChoice) != 1 || catChoice < 1 || catChoice > 5) {
        printf(CLR_RED "Invalid selection! Returning to menu.\n" CLR_RESET);
        pressEnterToContinue();
        return;
    }

    if (catChoice == 5) return;

    const Question *activeQuestions[TOTAL_QUESTIONS_BANK];
    int totalQ = 0;
    char categoryName[48];

    if (catChoice == 1) {
        strcpy(categoryName, "C & Computer Science");
        for (int i = 0; i < 5; i++) activeQuestions[totalQ++] = &questionBank[i];
    } else if (catChoice == 2) {
        strcpy(categoryName, "Science & Astronomy");
        for (int i = 5; i < 10; i++) activeQuestions[totalQ++] = &questionBank[i];
    } else if (catChoice == 3) {
        strcpy(categoryName, "History & Trivia");
        for (int i = 10; i < 15; i++) activeQuestions[totalQ++] = &questionBank[i];
    } else {
        strcpy(categoryName, "Grand Challenge (All)");
        for (int i = 0; i < TOTAL_QUESTIONS_BANK; i++) activeQuestions[totalQ++] = &questionBank[i];
    }

    clearScreen();
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf(CLR_BOLD CLR_GREEN "  Category: %s\n" CLR_RESET, categoryName);
    printf("  Total Questions : %d\n", totalQ);
    printf("  Lifelines       : 1x [50:50] (Press 'L' during question)\n");
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    pressEnterToContinue();

    int score = 0;
    int lifelineAvailable = 1;

    for (int i = 0; i < totalQ; i++) {
        clearScreen();
        const Question *q = activeQuestions[i];

        int eliminated[MAX_OPTIONS] = {0, 0, 0, 0};

        char userAns = getUserChoiceWithLifeline(&lifelineAvailable, q, eliminated);
        if (userAns == '\0') {
            printf("\nQuiz interrupted. Returning to menu.\n");
            pressEnterToContinue();
            return;
        }

        if (userAns == q->correctOption) {
            printf("\n" CLR_BOLD CLR_GREEN " [CORRECT!] Excellent! (+1 Point)" CLR_RESET "\n");
            score++;
        } else {
            printf("\n" CLR_BOLD CLR_RED " [INCORRECT] The correct answer was: [%c] %s" CLR_RESET "\n",
                   q->correctOption, q->options[q->correctOption - 'A']);
        }

        printf(CLR_YELLOW " Explanation: " CLR_RESET "%s\n", q->explanation);
        printf(CLR_CYAN "------------------------------------------------------------\n" CLR_RESET);
        printf("Current Score: %d / %d\n", score, i + 1);
        pressEnterToContinue();
    }

    // Final Scorecard
    clearScreen();
    double percentage = ((double)score / totalQ) * 100.0;

    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf(CLR_BOLD CLR_MAGENTA "                      FINAL SCORECARD                       \n" CLR_RESET);
    printf(CLR_CYAN "============================================================\n" CLR_RESET);
    printf(" Category        : %s\n", categoryName);
    printf(" Total Questions : %d\n", totalQ);
    printf(" Correct Answers : " CLR_GREEN "%d" CLR_RESET "\n", score);
    printf(" Wrong Answers   : " CLR_RED "%d" CLR_RESET "\n", totalQ - score);
    printf(" Accuracy        : " CLR_BOLD CLR_YELLOW "%.1f%%" CLR_RESET "\n", percentage);
    printf(" Rating          : ");

    if (percentage == 100.0) {
        printf(CLR_BOLD CLR_GREEN "PERFECT SCORE! Outstanding Genius!" CLR_RESET "\n");
    } else if (percentage >= 80.0) {
        printf(CLR_BOLD CLR_GREEN "EXCELLENT! Highly knowledgeable!" CLR_RESET "\n");
    } else if (percentage >= 60.0) {
        printf(CLR_BOLD CLR_YELLOW "GOOD WORK! Strong effort!" CLR_RESET "\n");
    } else if (percentage >= 40.0) {
        printf(CLR_BOLD CLR_YELLOW "FAIR. A little more revision will help!" CLR_RESET "\n");
    } else {
        printf(CLR_BOLD CLR_RED "KEEP PRACTICING! Don't give up!" CLR_RESET "\n");
    }
    printf(CLR_CYAN "============================================================\n" CLR_RESET);

    // Save Score
    recordHighScore(categoryName, score, totalQ);
}

// Safely gets user choice and handles the 50:50 Lifeline
char getUserChoiceWithLifeline(int *lifelineAvailable, const Question *q, int eliminated[MAX_OPTIONS]) {
    char input[64];
    char optionLabels[MAX_OPTIONS] = {'A', 'B', 'C', 'D'};

    while (1) {
        printf("\n" CLR_BOLD "%s" CLR_RESET "\n\n", q->question);

        for (int o = 0; o < MAX_OPTIONS; o++) {
            if (eliminated[o]) {
                printf("   " CLR_RED "[%c] ~~ ELIMINATED ~~" CLR_RESET "\n", optionLabels[o]);
            } else {
                printf("   " CLR_CYAN "[%c]" CLR_RESET " %s\n", optionLabels[o], q->options[o]);
            }
        }

        printf("\n");
        if (*lifelineAvailable) {
            printf("Your answer (A, B, C, D) or [" CLR_MAGENTA "L" CLR_RESET " for 50:50 Lifeline]: ");
        } else {
            printf("Your answer (A, B, C, D) [Lifeline Used]: ");
        }

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return '\0';
        }

        int idx = 0;
        while (input[idx] != '\0' && isspace((unsigned char)input[idx])) idx++;

        if (input[idx] == '\0') {
            printf(CLR_RED "Input cannot be empty. Please enter your answer.\n" CLR_RESET);
            continue;
        }

        char choice = (char)toupper((unsigned char)input[idx]);

        // Check if user requested 50:50 Lifeline
        if (choice == 'L' || choice == '5') {
            if (!(*lifelineAvailable)) {
                printf(CLR_RED "\n[!] You have already used your 50:50 lifeline for this round!\n" CLR_RESET);
                continue;
            }

            // Trigger 50:50 Lifeline
            *lifelineAvailable = 0;
            int correctIdx = q->correctOption - 'A';

            // Find 2 wrong options to eliminate
            int wrongIndices[MAX_OPTIONS - 1];
            int wCount = 0;
            for (int k = 0; k < MAX_OPTIONS; k++) {
                if (k != correctIdx) {
                    wrongIndices[wCount++] = k;
                }
            }

            // Shuffle wrong indices
            for (int k = wCount - 1; k > 0; k--) {
                int r = rand() % (k + 1);
                int temp = wrongIndices[k];
                wrongIndices[k] = wrongIndices[r];
                wrongIndices[r] = temp;
            }

            eliminated[wrongIndices[0]] = 1;
            eliminated[wrongIndices[1]] = 1;

            printf("\n" CLR_BOLD CLR_MAGENTA "*** 50:50 LIFELINE ACTIVATED! 2 incorrect options removed! ***" CLR_RESET "\n");
            continue;
        }

        if (choice >= 'A' && choice <= 'D') {
            int selectedIdx = choice - 'A';
            if (eliminated[selectedIdx]) {
                printf(CLR_RED "Option [%c] was already eliminated! Choose from remaining options.\n" CLR_RESET, choice);
                continue;
            }
            return choice;
        }

        printf(CLR_RED "Invalid input '%c'. Please choose A, B, C, or D (or 'L' for lifeline).\n" CLR_RESET, choice);
    }
}

// Retrieves current date as YYYY-MM-DD
void getCurrentDate(char *buffer, size_t bufSize) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (t != NULL) {
        strftime(buffer, bufSize, "%Y-%m-%d %H:%M", t);
    } else {
        snprintf(buffer, bufSize, "N/A");
    }
}

// Appends high score record to file
void recordHighScore(const char *category, int score, int total) {
    printf("\nSave your score to the Hall of Fame!\n");
    printf("Enter your player name (or press ENTER to skip): ");

    char playerName[48];
    if (fgets(playerName, sizeof(playerName), stdin) == NULL) return;

    // Trim newline and whitespace
    size_t len = strlen(playerName);
    while (len > 0 && isspace((unsigned char)playerName[len - 1])) {
        playerName[--len] = '\0';
    }

    if (len == 0) {
        printf("Score was not recorded.\n");
        pressEnterToContinue();
        return;
    }

    // Sanitize commas to preserve clean record format
    for (size_t i = 0; i < len; i++) {
        if (playerName[i] == ',' || playerName[i] == ';') playerName[i] = ' ';
    }

    char dateStr[24];
    getCurrentDate(dateStr, sizeof(dateStr));

    FILE *fp = fopen(HIGHSCORE_FILE, "a");
    if (fp == NULL) {
        printf(CLR_RED "Error saving high score to file!\n" CLR_RESET);
        pressEnterToContinue();
        return;
    }

    double percentage = ((double)score / total) * 100.0;
    fprintf(fp, "%s,%s,%d,%d,%.1f,%s\n", playerName, category, score, total, percentage, dateStr);
    fclose(fp);

    printf(CLR_GREEN "Score successfully recorded in Hall of Fame!" CLR_RESET "\n");
    pressEnterToContinue();
}

// Displays sorted leaderboard from file
void showLeaderboard(void) {
    clearScreen();
    printf(CLR_CYAN "========================================================================================\n" CLR_RESET);
    printf(CLR_BOLD CLR_YELLOW "                               HALL OF FAME / LEADERBOARD                               \n" CLR_RESET);
    printf(CLR_CYAN "========================================================================================\n" CLR_RESET);

    FILE *fp = fopen(HIGHSCORE_FILE, "r");
    if (fp == NULL) {
        printf("\n  No high scores recorded yet! Play a game and be the first on the board.\n\n");
        printf(CLR_CYAN "========================================================================================\n" CLR_RESET);
        pressEnterToContinue();
        return;
    }

    HighScore scores[MAX_HIGHSCORES];
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL && count < MAX_HIGHSCORES) {
        // Format: name,category,score,total,percentage,date
        char *name = strtok(line, ",");
        char *cat = strtok(NULL, ",");
        char *sScore = strtok(NULL, ",");
        char *sTotal = strtok(NULL, ",");
        char *sPct = strtok(NULL, ",");
        char *date = strtok(NULL, "\r\n");

        if (name && cat && sScore && sTotal && sPct && date) {
            strncpy(scores[count].name, name, sizeof(scores[count].name) - 1);
            scores[count].name[sizeof(scores[count].name) - 1] = '\0';

            strncpy(scores[count].category, cat, sizeof(scores[count].category) - 1);
            scores[count].category[sizeof(scores[count].category) - 1] = '\0';

            scores[count].score = atoi(sScore);
            scores[count].total = atoi(sTotal);
            scores[count].percentage = atof(sPct);

            strncpy(scores[count].date, date, sizeof(scores[count].date) - 1);
            scores[count].date[sizeof(scores[count].date) - 1] = '\0';

            count++;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("\n  Leaderboard is currently empty.\n\n");
        printf(CLR_CYAN "========================================================================================\n" CLR_RESET);
        pressEnterToContinue();
        return;
    }

    // Sort by percentage descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j].percentage < scores[j + 1].percentage) {
                HighScore tmp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tmp;
            }
        }
    }

    printf(CLR_BOLD "%-5s %-18s %-26s %-10s %-11s %-16s\n" CLR_RESET,
           "RANK", "PLAYER NAME", "CATEGORY", "SCORE", "ACCURACY", "DATE");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count && i < 15; i++) {
        char medal[8] = "";
        if (i == 0) strcpy(medal, "[1]");
        else if (i == 1) strcpy(medal, "[2]");
        else if (i == 2) strcpy(medal, "[3]");
        else snprintf(medal, sizeof(medal), "%2d.", i + 1);

        char scoreDisplay[16];
        snprintf(scoreDisplay, sizeof(scoreDisplay), "%d / %d", scores[i].score, scores[i].total);

        printf("%-5s %-18.18s %-26.26s %-10s %6.1f%%    %-16s\n",
               medal,
               scores[i].name,
               scores[i].category,
               scoreDisplay,
               scores[i].percentage,
               scores[i].date);
    }

    printf(CLR_CYAN "========================================================================================\n" CLR_RESET);
    pressEnterToContinue();
}

// Clears the high score file
void clearLeaderboard(void) {
    clearScreen();
    printf(CLR_RED "Are you sure you want to clear the entire Leaderboard? (Y/N): " CLR_RESET);
    char buf[16];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (toupper((unsigned char)buf[0]) == 'Y') {
            remove(HIGHSCORE_FILE);
            printf(CLR_GREEN "Leaderboard data successfully cleared!\n" CLR_RESET);
        } else {
            printf("Action canceled.\n");
        }
    }
    pressEnterToContinue();
}
