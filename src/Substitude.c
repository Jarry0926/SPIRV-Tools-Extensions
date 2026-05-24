#include <stdio.h>

#include "Substitude.h"

static uint32_t AfterEqualSign = 0;
static uint32_t RequiredType   = 0;

static char     SubstitudedLine[1024];
static char     File[UINT32_MAX];
static uint32_t FileCursor = 0u;

static inline char*
parseSsaId(char* p_id)
{
    while (*p_id != '`') {
        switch (*p_id) {
            case ' ':
                break;
            case '+':
                sprintf(File + FileCursor, "_01_");
                FileCursor += 4;
                break;
            case '-':
                sprintf(File + FileCursor, "_02_");
                FileCursor += 4;
                break;
            case '*':
                sprintf(File + FileCursor, "_03_");
                FileCursor += 4;
                break;
            case '/':
                sprintf(File + FileCursor, "_04_");
                FileCursor += 4;
                break;
            case '%':
                sprintf(File + FileCursor, "_05_");
                FileCursor += 4;
                break;
            case '<':
                sprintf(File + FileCursor, "_06_");
                FileCursor += 4;
                break;
            case '>':
                sprintf(File + FileCursor, "_07_");
                FileCursor += 4;
                break;
            case '=':
                sprintf(File + FileCursor, "_08_");
                FileCursor += 4;
                break;
            case '&':
                sprintf(File + FileCursor, "_09_");
                FileCursor += 4;
                break;
            case '|':
                sprintf(File + FileCursor, "_0A_");
                FileCursor += 4;
                break;
            case '^':
                sprintf(File + FileCursor, "_0B_");
                FileCursor += 4;
                break;
            case '~':
                sprintf(File + FileCursor, "_0C_");
                FileCursor += 4;
                break;
            case '(':
                sprintf(File + FileCursor, "_0D_");
                FileCursor += 4;
                break;
            case ')':
                sprintf(File + FileCursor, "_0E_");
                FileCursor += 4;
                break;
            case '[':
                sprintf(File + FileCursor, "_0F_");
                FileCursor += 4;
                break;
            case ']':
                sprintf(File + FileCursor, "_10_");
                FileCursor += 4;
                break;
            case '!':
                sprintf(File + FileCursor, "_20_");
                FileCursor += 4;
                break;
            case '.':
                sprintf(File + FileCursor, "_30_");
                FileCursor += 4;
                break;
            case '?':
                sprintf(File + FileCursor, "_40_");
                FileCursor += 4;
                break;
            case ':':
                sprintf(File + FileCursor, "_50_");
                FileCursor += 4;
                break;
            default:
                sprintf(File + FileCursor, "%c", *p_id);
                ++FileCursor;
                break;
        }
        ++p_id;
    }
    return p_id + 1;
}

static inline void
parseLine(char* p_line)
{
    while (*p_line != '\n') {
        if (*p_line == '`') {
            sprintf(File + FileCursor, "%%_");
            FileCursor += 2;
            p_line = parseSsaId(p_line + 1);
        }
        else {
            sprintf(File + FileCursor, "%c", *p_line);
            ++FileCursor;
            ++p_line;
        }
    }
    sprintf(File + FileCursor, "\n");
    ++FileCursor;
}

static inline void
_parseLine(char* p_line)
{
    while (*p_line != '\n') {
        if (*p_line == '`') {
            sprintf(File + FileCursor, "%%_");
            FileCursor += 2;
            printf("%c\n", *p_line);
            p_line = parseSsaId(p_line + 1);
        }
        else {
            sprintf(File + FileCursor, "%c", *p_line);
            printf("%c\n", *p_line);
            ++FileCursor;
            ++p_line;
        }
    }
    sprintf(File + FileCursor, "\n");
    ++FileCursor;
}

inline char*
parseRead(const char* p_path)
{
    FileCursor = 0u;
    FILE* fp = fopen(p_path, "r");
    char  line[1024];
    uint32_t i = 0u;
    while (fgets(line, 1024, fp)) {
        if (i == 87) {
            printf("%u: %s", FileCursor, line);
            _parseLine(line);
        }
        parseLine(line);
        if (++i == 88) break;
    }
    fclose(fp);
    printf("%u\n", FileCursor);
    fflush(stdout);
    File[FileCursor] = '\0';
    return File;
}
