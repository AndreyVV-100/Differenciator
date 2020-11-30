// defines: CReate Operator, Variable, Number

#define CR_O(oper) ElementConstructor (OPER, NAN, oper)
#define CR_V(var)  ElementConstructor (VAR , NAN, var)
#define CR_N(num)  ElementConstructor (NUM , num, '\0')

enum ReadErr
{
    OK   = 0,
    OPEN = 1,
    INP  = 2
};