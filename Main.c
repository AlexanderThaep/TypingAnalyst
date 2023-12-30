#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <Analysis.h>

#define ESC         0x27
#define COUNTDOWN   0x03
#define BUFFER_SIZE 1024
#define PROMPTS     25

static char INPUT_BUFFER[BUFFER_SIZE];
static char PROMPT_BUFFER[BUFFER_SIZE];

static Analysis *currentAnalysis_ptr;

void readFileIntoBuffer(const char *file_name, char *buffer, int line_number)
{

    FILE *file = fopen(file_name, "r");

    for (int i = 0; i < line_number; i++)
    {
        fgets(PROMPT_BUFFER, BUFFER_SIZE, file);
    }

    rewind(file);

    return;
}

void prompt()
{
    int n = (rand() % PROMPTS) + 1;
    readFileIntoBuffer("prompts.txt", PROMPT_BUFFER, n);
    analysePrompt(PROMPT_BUFFER, currentAnalysis_ptr);
    printf("Prompt: %s\n", PROMPT_BUFFER);

    return;
}

void input()
{
    struct timeval tv1;
    struct timeval tv2;
    unsigned long long timeElapsed;

    printf("Type: ");

    gettimeofday(&tv1, NULL);
    fgets(INPUT_BUFFER, 2047, stdin);
    gettimeofday(&tv2, NULL);

    timeElapsed =
        tv2.tv_sec * 1000 + tv2.tv_usec / 1000 -
        tv1.tv_sec * 1000 + tv2.tv_usec / 1000;

    currentAnalysis_ptr->speed = (float)timeElapsed / 1000;

    printf("Elapsed: [%fs]\nTyped: %s\n", (float)timeElapsed / 1000, INPUT_BUFFER);

    return;
}

int main(void)
{
    srand(time(NULL));

    int rounds = 0;
    char string[5];
    printf("How many rounds? ");
    fgets(string, 4, stdin);
    rounds = atoi(string);

    printf("Get ready!");
    fflush(stdout);
    sleep(1);
    for (int i = 1; i <= COUNTDOWN; i++)
    {
        printf(" %d", i);
        fflush(stdout);
        sleep(1);
    }
    printf(" Go!\n");

    int elapsedRounds = 0;
    Analysis cumulative;
    initAnalysis(&cumulative);

    while (rounds > 0)
    {
        Analysis analysis;
        initAnalysis(&analysis);
        currentAnalysis_ptr = &analysis;

        prompt();
        input();
        scoreAttempt(INPUT_BUFFER, PROMPT_BUFFER, currentAnalysis_ptr);

        cumulative.accuracy += analysis.accuracy;
        cumulative.score += analysis.score;
        cumulative.speed += analysis.speed;

        printf("Speed: %f, Accuracy: %f, Score: %f\n",
               analysis.speed, analysis.accuracy, analysis.score);

        printf("Characters: %d, Words: %d, Difficulty: %f\n\n",
               analysis.characters, analysis.words, analysis.difficulty);

        rounds--;
        elapsedRounds++;
    }

    cumulative.accuracy /= elapsedRounds;
    cumulative.score /= elapsedRounds;
    cumulative.speed /= elapsedRounds;

    printf("Average Speed: %f, Average Accuracy: %f, Average Score: %f\n",
           cumulative.speed, cumulative.accuracy, cumulative.score);

    return 0;
}