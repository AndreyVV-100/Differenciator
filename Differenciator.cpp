#include "Libraries/Tree/Tree.h"
#include "Differenciator.h"

int main ()
{
    Tree function = {};
    if (ReadFunction (&function, "EqFiles/eq1.txt"))
        return 0;

    CreateGraph (&function);
    CreateTex   (&function);

    return 0;
}

bool ReadFunction (Tree* func, const char* file_name)
{
    assert (file_name);

    FILE* eq = fopen (file_name, "r"); // equation
    if (!eq)
    {
        printf ("I'm sorry, I can't open this file.");
        return 1;
    }

    size_t num_symbols = CountSize (eq);
    char* equation = (char*) calloc (num_symbols + 1, sizeof (*equation));
    if (!equation)
    {
        printf ("Memory error.");
        return 1;
    }

    fread (equation, sizeof (*equation), num_symbols, eq);
    fclose (eq);

    char* equation_copy = equation;
    SkipSpaces (&equation_copy);
    func->head = ReadElement (&equation_copy);
    if (func->head == nullptr)
    {
        printf ("The equation was poorly written.");
        free (equation);
        return 1;
    }

    free (equation);
    return 0;
}

size_t CountSize (FILE* file)
{
    fseek (file, 0, SEEK_END);
    size_t num_symbols = ftell (file);
    fseek (file, 0, SEEK_SET);

    return num_symbols;
}

element* ReadElement (char** eq)
{
    assert (eq);
    assert (*eq);

    if (**eq != '(')
    {
        printf ("No open bracket:\n" "%s\n", *eq);
        return nullptr;
    }
    *eq += 1;
    SkipSpaces (eq);

    element* el_ret = CR_O(NO_OP);

    if (**eq == '(')
    {
        el_ret->left = ReadElement (eq);
        if (el_ret->left == nullptr)
        {
            ElementDestructor (el_ret);
            return nullptr;
        }
    }
    else
    {
        el_ret->left = ReadVar (eq);
        if (el_ret->left == nullptr)
            el_ret->left = ReadNum (eq);
    }

    ReadOper (eq, el_ret);
    if (el_ret->symb == NO_OP)
    {
        printf ("Bad operator.\n" "%s\n", *eq);
        ElementDestructor (el_ret);
        return nullptr;
    }

    SkipSpaces (eq);

    if (**eq == '(')
        el_ret->right = ReadElement (eq);
    else
    {
        el_ret->right = ReadVar (eq);
        if (el_ret->right == nullptr)
            el_ret->right = ReadNum (eq);
    }

    SkipSpaces (eq);

    if (el_ret->right == nullptr || **eq != ')')
    {
        printf ("Bad end.\n" "%s\n", *eq);
        ElementDestructor (el_ret);
        return nullptr;
    }

    *eq += 1;
    return el_ret;
}

void  SkipSpaces (char** eq)
{
    assert (eq);
    assert (*eq);

    while (**eq == ' ' || **eq == '\t' || **eq == '\n' || **eq == '\r')
        *eq += 1;

    return;
}

element* ReadVar (char** eq)
{
    assert (eq);
    assert (*eq);

    char var = '\0';
    size_t shift = 0;
    sscanf (*eq, " %c%n", &var, &shift);

    if (!var)
        return nullptr;

    // ToDo: List of vars, check all list
    if (var != 'x')
        return nullptr;

    *eq += shift;
    return CR_V (var);
}

element* ReadNum (char** eq)
{
    assert (eq);
    assert (*eq);

    double num = NAN;
    size_t shift = 0;
    sscanf (*eq, " %lf%n", &num, &shift);

    *eq += shift;
    return CR_N (num);
}

void    ReadOper (char** eq, element* elem)
{
    assert (elem);
    assert (eq);
    assert (*eq);

    const size_t oper_size  = 16;
    char    oper[oper_size] = "";
    size_t shift = 0;

    sscanf (*eq, " %s%n", oper, &shift);
    *eq += shift;

    switch (*oper)
    {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case POW:
            elem->symb = *oper;
            return;
        case 's':
            if (strcmp ("sin", oper) == 0)
                elem->symb = SIN;
            else if (strcmp ("sh", oper) == 0)
                elem->symb = SH;
            return;
        case 'c':
            if (strcmp ("cos", oper) == 0)
                elem->symb = COS;
            else if (strcmp ("ch", oper) == 0)
                elem->symb = CH;
            return;
        default:
            return;
    }
}
