// defines: CReate Operator, Variable, Number

#define CR_O(oper) ElementConstructor (OPER, NAN, oper)
#define CR_V(var)  ElementConstructor (VAR , NAN, var)
#define CR_N(num)  ElementConstructor (NUM , num, '~')

enum Operators
{
    NO_OP   = '~',
    ADD     = '+',
    SUB     = '-',
    MUL     = '*',
    DIV     = '/',
    POW     = '^',
    SIN     = '1',
    COS     = '2',
    SH      = '3',
    CH      = '4'
};

bool ReadFunction (Tree* func, const char* file_name);

size_t CountSize (FILE* file);

element* ReadElement (char** eq);

void  SkipSpaces (char** eq);

element* ReadVar (char** eq);

element* ReadNum (char** eq);

void    ReadOper (char** eq, element* elem);
