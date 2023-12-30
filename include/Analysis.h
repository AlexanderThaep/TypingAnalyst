typedef struct Analysis
{
    float difficulty;
    int characters;
    int punctuation;
    int numbers;
    int capitals;
    int letters;
    int words;

    float speed;
    float accuracy;
    float score;
} Analysis;

void initAnalysis(Analysis *analysis);
void analysePrompt(char *buffer, Analysis *analysis);
void scoreAttempt(char *input_buffer, char *prompt_buffer, Analysis *analysis);