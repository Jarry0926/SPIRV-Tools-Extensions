#include "Alnum.h"

#define STRING(LEN)\
	struct Str##LEN\
	{\
		char  str[(int)LEN];\
		char* it;\
	}
#define STRING_INIT(VAR) VAR.it = VAR.str

// InFnScope: bool indicates whether or not the current processing is between OpFunction and OpFunctionEnd
// FnCnt: number of function scope encountered so far, 0 for the first one
static int      InFnScope;
static uint32_t FnCnt;
static int      PostAnnotation;
static int      DebugInfo;

STRING(1e6);
STRING(1e4);
STRING(1024);
STRING(256);

// Out: buffer storing output texts to be flushed when program terminates
// OutIt: iterator of Out[]
static char  Out[(int)1e6];
static char* OutIt;

// Varialble names as debug info
static struct Str1e4 Names;
static char* NameInsert;

// Str: the current string
// StrIt: iterator of Str[]
static char Str[256];
static char* StrIt;

static struct StrMap gv;
static struct StrMap dbgNames;

static inline void
alnumInit()
{
    InFnScope      = 0;
    FnCnt          = 0u;
	PostAnnotation = 0;
	DebugInfo      = 1;
	OutIt          = Out;
	StrIt          = Str;
	STRING_INIT(Names);
	strMapInit(&gv);
	strMapInit(&dbgNames);
}

// Convert a uint32_t value to a hexidecimal string
static inline void
alnumHexStr(uint32_t p_u32,
		    char*    p_out)
{
    for (char* it = p_out + 7; it >= p_out; --it) {
        const uint32_t digit = p_u32 & 15u;
        *it = (digit < 10 ? '0' : 'A' - 10) + digit;
        p_u32 >>= 4; // p_u32 /= 16
    }
}

// Remove spaces inside p_str, producing p_out
static inline char*
alnumStripStr(char* p_str,
		      char* p_out)
{
    for ( ; *p_str != '`'; ++p_str) {
        if (!isspace((int)*p_str)) {
            *p_out++ = *p_str;
        }
    }
    *p_out = '\0';
    return p_str + 1;
}

// return the new pos to be assigned to p_line at the caller
static inline char*
alnumParseSSA(char* p_lineIt)
{
    char           stripped[256];
    char*          ret = alnumStripStr(p_lineIt + 1, stripped);
	const uint32_t val = strMapHash(stripped);
	if (!strMapFind(&gv, stripped, val)) {
		if (InFnScope) {
			// Append '_' FnCnt times to each string of local variable
			char* it = &stripped[strlen(stripped)];
			for (uint32_t i = 0u; i < FnCnt; ++i) {
				*it++ = '_';
			}
			*it = '\0';
		}
		else {
			strMapPush(&gv, stripped, val);
		}
	}
	// Write to Out the hexidecimal hash value with leading "%_" and '\0' terminator
	OutIt[0] = '%', OutIt[1] = '_', OutIt[10] = '\0';
    alnumHexStr(strMapHash(stripped), &OutIt[2]);
	// Record the SSA name
	if (DebugInfo) {
		if (!strMapFind(&dbgNames, stripped, val)) {
			strMapPush(&dbgNames, stripped, val);
			sprintf(Names.it, "OpName %s \"%s\"\n", OutIt, stripped);
			Names.it += strlen(stripped) + 21;
		}
	}
    OutIt += 10;

	return ret;
}

static inline void
alnumCheckFnScope()
{
	if (strncmp("OpFunctionEnd", Str, 14) == 0) {
		InFnScope = 0;
    }
	else if (strncmp("OpFunction", Str, 11) == 0) {
		++FnCnt;
        InFnScope = 1;
    }
}

static inline void
alnumParsePragma()
{
	if (strncmp("annotation", Str, 11) == 0) {
		if (DebugInfo) {
			// Rememer the location to insert variable names as debug info
			NameInsert = OutIt;
		}
	}
	else if (strncmp("end_annotation", Str, 15) == 0) {
		PostAnnotation = 1;
	}
}

static inline int
alnumCheckPragma(char** const p_pLineIt)
{
	if (strncmp("#pragma", Str, 7) == 0) {
		for (; isspace(**p_pLineIt); ++(*p_pLineIt));
		StrIt = Str; // reset
		while (**p_pLineIt != '\n') {
			*StrIt++ = *((*p_pLineIt)++);
		}
		*StrIt = '\0'; // wrap up
		alnumParsePragma();
		StrIt = Str; // reset
		return 1;
	}
	return 0;
}

static inline int
alnumIsType()
{
	return (*(StrIt - 2) == '_' && *(StrIt - 1) == 't')
		|| (*(StrIt - 3) == '_' && *(StrIt - 2) == 'f' && *(StrIt - 1) == 't')
		|| strcmp("void", Str) == 0
		|| strcmp("bool", Str) == 0;
}

static inline void
alnumParseLine(char*          p_line,
			   const uint32_t p_lineCnt)
{
    while (1) {
		if (*p_line == ';') {
			for ( ; *p_line != '\n'; ++p_line);
			break;
		}
		else if (*p_line == '`') {
			// Parse and advance p_line to the char after ending backtick
      		p_line = alnumParseSSA(p_line);
			StrIt = Str; // reset recording string
		}
        else if (isspace(*p_line)) {
            *StrIt = '\0'; // wrap up for later string operations
			if (alnumCheckPragma(&p_line)) {
				break;
			}
			alnumCheckFnScope();
            if (alnumIsType()) {
                *OutIt++ = '%';
            }
			
			// Append Str to Out
			const uint32_t n = StrIt - Str;
            memcpy(OutIt, Str, n * sizeof(char));
            OutIt += n;
            StrIt = Str; // reset recording string
            if (*p_line == '\n') {
                break;
            }
            *OutIt++ = *p_line++;
        }
        else {
            *StrIt++ = *p_line++;
        }
    }
    *OutIt++ = '\n';
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
    *++OutIt = '\0';
    return Out;
}

int
main(int p_numArgs, char** p_args)
{
    const char* out = spvAlnum(p_args[1]);
    if (out != nullptr) {
		if (DebugInfo) {
			char tmp = *NameInsert;
			*NameInsert = '\0';
			printf("%s%s", out, Names.str);
			*NameInsert = tmp;
        	printf("%s", NameInsert);
		}
		else {
			printf("%s", out);
		}
    }
    return 0;
}

