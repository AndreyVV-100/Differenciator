struct element;
struct Tree;
struct Text;
enum Types;
#include <stdio.h>

// defines: CReate Operator, Variable, Number
#define CR_O(oper) ElementConstructor (OPER, NAN, oper)
#define CR_V(var)  ElementConstructor (VAR , NAN, var)
#define CR_N(num)  ElementConstructor (NUM , num, NO_OP)

const double REPLACEMENT = 228;

enum TexingModes
{
    DIFFERENTIAL   = 0,
    SIMPLIFY       = 1,
    LAST_ITERATION = 2
};

void CreateGraph (Tree* tree);

void ElementGraph (FILE* graph, element* el);

const char* TypeCheck (Types type);

void CreateTex (element* el_was, element* el_became, TexingModes tex_mode);

void ElementTex (FILE* tex, element* el, bool need_brackets, char* repl_name);

bool PrintFrase (FILE* tex, const char* frase_place);

void PrintChange (FILE* tex, Text* frases, Text* count, int* shizi, 
                  element* el_was, element* el_became, TexingModes tex_mode);

size_t DefineReplacement (element* el, bool* need_repl);

void PrintReplacements (FILE* tex, element* el, char* repl_name);

/*
This is old code:

bool ReadFunction (Tree* func, const char* file_name);

element* ReadElement (char** eq);

element* ReadVar (char** eq);

element* ReadNum (char** eq);

void    ReadOper (char** eq, element* elem);
*/