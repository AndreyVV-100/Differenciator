#include "InOut.h"
#include "Tree.h"
#include "Text.h"
#include <time.h>

#pragma warning (disable : 26451)
/*
    Арифметическое переполнение: использование оператора "*" на байтовом значении 4 и
    приведение результата к байтовому значению 8. Приведите значение к более широкому
    типу перед вызовом оператора "*", чтобы избежать переполнения (io.2).
*/

// Types Names
const char* NUM_NAME  = "Number";
const char* OPER_NAME = "Operator";
const char* VAR_NAME  = "Variable";
const char* NIL_NAME  = "Not determined";
const char* ERR_NAME  = "Error!";

void CreateGraph (Tree* tree)
{
    assert (tree);

    FILE* graph = fopen ("AllDumps/out.dot", "w");
    assert (graph);
    fprintf (graph, "digraph G{\n" "rankdir = HR;\n node[shape=box];\n");

    if (tree->head)
    {
        fprintf (graph, "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\";\n",
                 tree->head, TypeCheck (tree->head->type), tree->head->symb, tree->head->num);
        ElementGraph (graph, tree->head);
    }
    else
        fprintf (graph, "\"No elements\";\n");

    fprintf (graph, "}");
    fclose (graph);

    system ("dot -Tpng AllDumps\\out.dot -o AllDumps\\gr.png");
    system ("start AllDumps\\gr.png");

    return;
}

void ElementGraph (FILE* graph, element* el)
{
    assert (graph);
    assert (el);

    if (el->left)
    {
        fprintf (graph, "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\" -> "
                        "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\";\n",
                       el,       TypeCheck (el->type),       el->symb,       el->num, 
                 el->left, TypeCheck (el->left->type), el->left->symb, el->left->num);

        ElementGraph (graph, el->left);
    }

    if (el->right)
    {
        fprintf (graph, "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\" -> "
                        "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\";\n",
                        el,        TypeCheck (el->type),        el->symb,        el->num, 
                 el->right, TypeCheck (el->right->type), el->right->symb, el->right->num);

        ElementGraph (graph, el->right);
    }

    return;
}

const char* TypeCheck (Types type)
{
    switch (type)
    {
        case NUM:  return  NUM_NAME;
        case OPER: return OPER_NAME;
        case VAR:  return  VAR_NAME;
        case NIL:  return  NIL_NAME;
        default:   return  ERR_NAME;
    }
}

void CreateTex (element* el_was, element* el_became, TexingModes tex_mode)
{
    // ToDo: functions

    static FILE* tex = fopen ("AllDumps/Tree.tex", "w");
    if (!tex)
        return;

    if (tex_mode == LAST_ITERATION)
    {
        PrintFrase (tex, "frases/last.txt");
        CreateTex (el_was, el_became, DIFFERENTIAL);

        bool end_ok = PrintFrase (tex, "frases/ending.txt");

        fclose (tex);
        tex = nullptr;

        if (end_ok)
            system ("calltex>nul Tree AllDumps");

        return;
    }
    
    if (!el_was || !el_became)
        return;

    static bool first_iteration = 1;
    static Text frases = {};
    static int  shizi = 0;
    static Text count = {};

    if (first_iteration)
    {
        first_iteration = 0;
        srand (time (nullptr));

        if (!PrintFrase (tex, "frases/intro.txt"))
            return;

        ConstructorText (&count, "frases/count.txt");
        ConstructorText (&frases,  "frases/frases.txt");
    }

    if (shizi < 10)
        PrintChange (tex, &frases, &count, &shizi, el_was, el_became, tex_mode);
    else
        shizi--;

    if (shizi == 10 || shizi == 11)
        shizi = 0;

    return;
}

void ElementTex (FILE* tex, element* el, bool need_brackets)
{
    assert (tex);
    if (!el || el->type == NIL)
        return;

    if (el->type == NUM)
    {
        fprintf (tex, "%.0lf", el->num);
        return;
    }

    if (el->type == VAR)
    {
        fprintf (tex, "%c", el->symb);
        return;
    }

    switch (el->symb)
    {
        #define TexL(need) ElementTex (tex, el->left,  need)
        #define TexR(need) ElementTex (tex, el->right, need)

        #define priority(elem, bracket) if (elem->type == OPER)     \
                                            fprintf (tex, bracket)

        #define unar_oper(oper) fprintf (tex, "\\" oper " ");   \
                                priority (el->right, "(");      \
                                TexR (0);                       \
                                priority (el->right, ")")

        case ADD:
        case SUB:
            if (need_brackets)
                fprintf (tex, "(");
            TexL (0);
            fprintf (tex, " %c ", el->symb);
            TexR (0);
            if (need_brackets)
                fprintf (tex, ")");
            break;
        case MUL:
            TexL (1);
            fprintf (tex, " \\cdot ");
            TexR (1);
            break;
        case DIV:
            fprintf (tex, " \\frac{");
            TexL (0);
            fprintf (tex, "}{");
            TexR (0);
            fprintf (tex, "}");
            break;
        case POW:
            priority (el->left, "(");
            TexL (0);
            priority (el->left, ")");

            fprintf (tex, "^{");
            TexR (0);
            fprintf (tex, "}");
            break;
        case SIN:
            unar_oper ("sin");
            break;
        case COS:
            unar_oper ("cos");
            break;
        case TG:
            unar_oper ("tg");
            break;
        case CTG:
            unar_oper ("ctg");
            break;
        case SH :
            unar_oper ("sh");
            break;
        case CH :
            unar_oper ("ch");
            break;
        case TH:
            unar_oper ("th");
            break;
        case CTH:
            unar_oper ("cth");
            break;
        case ARCSIN:
            unar_oper ("arcsin");
            break;
        case ARCCOS:
            unar_oper ("arccos");
            break;
        case ARCTG:
            unar_oper ("arctg");
            break;
        case ARCCTG:
            unar_oper ("arcctg");
            break;
        case ARCSH:
            unar_oper ("arcsh");
            break;
        case ARCCH:
            unar_oper ("arcch");
            break;
        case ARCTH:
            unar_oper ("arcth");
            break;
        case ARCCTH:
            unar_oper ("arccth");
            break;
        case LN:
            unar_oper ("ln");
            break;

        #undef TexL
        #undef TexR
        #undef priority
        #undef unar_oper
    }

    return;
}

void PrintChange (FILE* tex, Text* frases, Text* count, int* shizi,
                  element* el_was, element* el_became, TexingModes tex_mode)
{
    assert (shizi);
    assert (el_was);
    assert (el_became);

    // space
    #define sp "\\\\\n"

    int rand_number = rand () % frases->n_empty_lines;

    if (*shizi == 0)
        fprintf (tex, sp "%s" sp, frases->lines[rand_number].point);

    if ((*shizi > 0 || rand_number == 0) && *shizi < 11)
    {
        fprintf (tex, sp "%s" sp, count->lines[*shizi].point);
        (*shizi)++;
    }

    if (rand_number == 1)
        *shizi = 16;

    if (*shizi < 11)
    {
        char bracket_left[] = "\\Bigg (";
        char bracket_right[] = "\\Bigg)'";

        if (tex_mode == SIMPLIFY)
        {
            bracket_left[0] = '\0';
            bracket_right[0] = '\0';
        }

        fprintf (tex, "\\begin{equation} %s \\displaystyle\n", bracket_left);

        ElementTex (tex, el_was, 0);
        fprintf (tex, "%s = \n", bracket_right);
        ElementTex (tex, el_became, 0);

        fprintf (tex, "\\end{equation}" sp);
    }

    #undef sp
    return;
}

bool PrintFrase (FILE* tex, const char* frase_place)
{
    assert (tex);
    assert (frase_place);

    char* frase = nullptr;
    ReadTxt (&frase, frase_place);

    if (!frase)
    {
        fclose (tex);
        return 0;
    }

    fprintf (tex, "%s", frase);
    free (frase);

    return 1;
}

/*
This is old code:

bool ReadFunction (Tree* func, const char* file_name)
{
    assert (file_name);

    char* equation = nullptr;
    size_t num_symbols = ReadTxt (&equation, file_name);

    if (!equation)
    {
        printf ("Memory error.");
        return 1;
    }

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

        case 'a':
            if (strcmp ("arcsin", oper) == 0)
                elem->symb = ARCSIN;
            else if (strcmp ("arccos", oper) == 0)
                elem->symb = ARCCOS;
            else if (strcmp ("arctg", oper) == 0)
                elem->symb = ARCTG;
            else if (strcmp ("arcctg", oper) == 0)
                elem->symb = ARCCTG;
            if (strcmp ("arcsh", oper) == 0)
                elem->symb = ARCSH;
            else if (strcmp ("arcch", oper) == 0)
                elem->symb = ARCCH;
            else if (strcmp ("arcth", oper) == 0)
                elem->symb = ARCTH;
            else if (strcmp ("arccth", oper) == 0)
                elem->symb = ARCCTH;
            return;

        case 'c':
            if (strcmp ("cos", oper) == 0)
                elem->symb = COS;
            else if (strcmp ("ch", oper) == 0)
                elem->symb = CH;
            else if (strcmp ("ctg", oper) == 0)
                elem->symb = CTG;
            else if (strcmp ("cth", oper) == 0)
                elem->symb = CTH;
            return;

        case 'l':
            if (strcmp ("ln", oper) == 0)
                elem->symb = LN;
            return;

        case 's':
            if (strcmp ("sin", oper) == 0)
                elem->symb = SIN;
            else if (strcmp ("sh", oper) == 0)
                elem->symb = SH;
            return;

        case 't':
            if (strcmp ("tg", oper) == 0)
                elem->symb = TG;
            else if (strcmp ("th", oper) == 0)
                elem->symb = TH;
            return;

        default:
            return;
    }
}
*/