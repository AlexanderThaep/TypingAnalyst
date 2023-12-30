#include <stdio.h>
#include <string.h>
#include <Analysis.h>

char *punctuation_table = "`~!@#$^&%*()_ -+=[]{}\\|;:'\"<,.>/?";

void initAnalysis(Analysis *analysis)
{
    analysis->characters = 0;
    analysis->punctuation = 0;
    analysis->numbers = 0;
    analysis->capitals = 0;
    analysis->letters = 0;
    analysis->words = 0;
    analysis->speed = 0;
    analysis->score = 0;
    analysis->difficulty = 0;
    analysis->accuracy = 0;

    return;
}

void analysePrompt(char *buffer, Analysis *analysis)
{
    int i = 0;
    char wordBreak = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] >= 48 && buffer[i] <= 57)
        {
            analysis->numbers++;
            wordBreak = 1;
        }
        else if (buffer[i] >= 65 && buffer[i] <= 90)
        {
            analysis->capitals++;
            wordBreak = 1;
        }
        else if (buffer[i] >= 97 && buffer[i] <= 122)
        {
            wordBreak = 1;
            analysis->letters++;
        }
        else
        {
            if (buffer[i] == '\n')
            {
                if (wordBreak == 1)
                {
                    wordBreak = 0;
                    analysis->words++;
                }
                break;
            }
            for (int ii = 0; ii < strlen(punctuation_table); ii++)
            {
                if (buffer[i] == punctuation_table[ii])
                {
                    analysis->punctuation++;
                    if (wordBreak == 1)
                    {
                        wordBreak = 0;
                        analysis->words++;
                    }
                }
            }
        }
        analysis->characters++;
        i++;
    }
    analysis->difficulty = analysis->numbers +
                           (((analysis->letters +
                              (1.5f * analysis->capitals)) /
                             analysis->words) *
                            analysis->punctuation);

    return;
}

int min(int a, int b, int c)
{
    int min = a;
    if (b < min)
        min = b;
    if (c < min)
        min = c;
    return min;
}

int demerau_levenshtein(char *a, char *b) {
    int len1 = strlen(a) - 1;
    int len2 = strlen(b) - 1;

    int d[len1 + 1][len2 + 1];

    for (int i = 0; i <= len1; i++) {
        d[i][0] = i;
    }
    for (int j = 0; j <= len2; j++) {
        d[0][j] = j;
    }

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost;
            if (a[i] == b[j]) {
                cost = 0;
            } else {
                cost = 1;
            }
            d[i][j] = min(d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + cost);
            if ( i > 1 && j > 1 && a[i] == b[j-1] && a[i-1] == b[j]) {
                d[i][j] = min(d[i][j], d[i-2][j-2] + 1, __INT16_MAX__);
            }
        }
    }

    return d[len1][len2];
}

void scoreAttempt(char *input_buffer, char *prompt_buffer, Analysis *analysis)
{
    int len1 = strlen(input_buffer) - 1; // compensate for that \n at the end
    int len2 = strlen(prompt_buffer) - 1;
    int maxLength = (len1 > len2) ? len1 : len2;
    analysis->accuracy = 1.0f - ((float)demerau_levenshtein(input_buffer, prompt_buffer) / maxLength);

    analysis->speed = analysis->words / analysis->speed * 60;
    analysis->score = (analysis->accuracy * analysis->speed) * analysis->difficulty;

    return;
}