struct element;
struct Tree;
enum Types;
#include <stdio.h>

// defines: CReate Operator, Variable, Number
#define CR_O(oper) ElementConstructor (OPER, NAN, oper)
#define CR_V(var)  ElementConstructor (VAR , NAN, var)
#define CR_N(num)  ElementConstructor (NUM , num, NO_OP)

bool ReadFunction (Tree* func, const char* file_name);

size_t CountSize (FILE* file);

element* ReadElement (char** eq);

void  SkipSpaces (char** eq);

element* ReadVar (char** eq);

element* ReadNum (char** eq);

void    ReadOper (char** eq, element* elem);

void CreateGraph (Tree* tree);

void ElementGraph (FILE* graph, element* el);

const char* TypeCheck (Types type);

void CreateTex (Tree* tree);

void ElementTex (FILE* tex, element* el, bool need_brackets);
