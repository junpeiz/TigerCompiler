#include <assert.h>

typedef char * string;
typedef char bool;

#define TRUE (char)1
#define FALSE (char)0
#define MAX_LENGTH 512
#define DEBUG_ERROR assert(0)

string String(char *);

char *UT_ConcatStr(char* strs[], int num);
char *UT_itoa(int t);

char *UT_prefix_itoa(const char *str, int t);
void indent(FILE *out, int d);

typedef struct intList_ *intList;
struct intList_
{
    int value;
    intList next;
};

intList IntList(int i, intList next);
