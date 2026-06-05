#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <utils/Str.h>

#include "Alnum.h"
#include "Check.h"
#include "StrMap.h"

#define STRING(LEN) struct Str##LEN\
					{\
						char  str[(int)LEN];\
						char* it;\
					}
#define STRING_INIT(VAR) VAR.it = VAR.str

static uint32_t g_status;

STRING(1e6);
STRING(1e4);
STRING(1024);
STRING(256);

// Buffer storing output texts
static struct Str1e6 g_out;

// Varialble names as debug info
static struct Str1e4 g_varNames;
static char* NameInsert;

// The current string
static struct Str256 g_word;

static struct StrMap globalVar;
static struct StrMap varNames;

static inline void
alnumInit()
{
	g_status = 0u;
	STRING_INIT(g_out);
	STRING_INIT(g_word);
	STRING_INIT(g_varNames);
	strmapInit(&globalVar);
	strmapInit(&varNames);
}

// return the new pos to be assigned to p_line at the caller
static inline char*
alnumParseSSA(char* p_lineIt)
{
    alignas(64) char stripped[64]; // SSA id with white spaces removed
    char*            ret   = strStrip(p_lineIt + 1, stripped, '`');
	const uint32_t   hashv = strmapHash(stripped);
	if (!strmapFind(&globalVar, stripped, val)) {
		if (g_status & STATUS_IN_FN_BIT) {
			// Append '_' FnCnt times to each string of local variable
			char* it = &stripped[strlen(stripped)];
			for (uint32_t i = 0u; i < FnCnt; ++i) {
				*it++ = '_';
			}
			*it = '\0';
		}
		else {
			strmapPush(&globalVar, stripped, val);
		}
	}

	// Write to `g_out` the string of hexidecimal hashed from `stripped`
	g_out.it[0] = '%', g_out.it[1] = '_'; // write leading "%_"
	g_out.it[10] = '\0';                  // write '\0' terminator
    strFromHex64(strmapHash(stripped), &g_out.it[2]);
	// Record the SSA name without duplicates
	if (OptDebugInfo) {
		if (!strmapFind(&varNames, stripped, val)) {
			strmapPush(&varNames, stripped, val);
			sprintf(g_varNames.it, "OpName %s \"%s\"\n", g_out.it, stripped);
			g_varNames.it += strlen(stripped) + 21;
		}
	}
    g_out.it += 10;

	return ret;
}

/**
 * @brief Check whether or not `g_word` represents a type
 *
 * @return 1 if it represents a type, 0 otherwise
 */
static inline int
alnumIsType()
{
	return strncmp("_t",  *(g_word.it - 2), 3) == 0 ||
		   strncmp("_ft", *(g_word.it - 3), 4) == 0 ||
		   strncmp("void", g_word.str,      5) == 0 ||
		   strncmp("bool", g_word.str,      5) == 0;
}

/**
 * @brief Parse string `p_line` and append `g_out`
 *
 * @param p_line: a '\n' terminated string being parsed
 * @param p_lineCnt: current line number
 */
static inline void
alnumParseLine(char*          p_line,
			   const uint32_t p_lineCnt)
{
    while (1) {
		if (p_line[0] == ';') {
			for ( ; p_line[0] != '\n'; ++p_line);
			break;
		}
		else if (p_line[0] == '`') {
			// Parse and advance p_line to the char after ending backtick
      		p_line = alnumParseSSA(p_line);
			g_word.it = g_word.str; // reset recording string
		}
        else if (isspace(p_line[0])) {
            *g_word.it = '\0'; // wrap up
			if (alnumCheckPragma(&p_line)) {
				break;
			}
			alnumCheckFnScope();
            if (alnumIsType()) {
                *g_out.it++ = '%';
            }
			
			// Append Str to Out
			const uint32_t n = g_word.it - g_word.str;
            memcpy(g_out.it, g_word.str, n * sizeof(char));
            g_out.it += n;
            g_word.it = g_word.str; // reset recording string
            if (*p_line == '\n') {
                break;
            }
            *g_out.it++ = *p_line++;
        }
        else {
            *g_wprd.it++ = *p_line++;
        }
    }
    *g_out.it++ = '\n';
}	
inline char*
spvAlnum(const char* p_path)
{
	alnumInit();
    FILE* fp = fopen(p_path, "r");
    if (fp == nullptr) {
        fprintf(stderr, "Failed to open file\n");
        return nullptr;
    }
    char     line[1024];
	uint32_t lineCnt = 1u;
    while (fgets(line, 1024, fp)) {
        alnumParseLine(line, lineCnt);
		++lineCnt;
    }
    fclose(fp);
    *++g_out.it = '\0';
    return g_out.str;
}

