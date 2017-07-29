extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_IRError(int, string,...);
void EM_impossible(string,...);
void EM_init(string filename);
void EM_LexError(int, char*);
void EM_MemoryError(char* keywords);

void EM_LexWarning(int, char*);
