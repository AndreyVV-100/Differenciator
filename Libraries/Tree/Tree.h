#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#pragma warning (disable : 4996)

#define create_el element* el_crt = ElementConstructor (type, num, symb)

#define tree_error_printf {                                                                      \
                          printf ("\n""Ошибка: дерево повреждено. Обратитесь к разработчику.\n");\
                          break;                                                                 \
                          }

enum Types
{
    NUM  = 0,
    OPER = 1,
    VAR  = 2,
    NIL  = 3
};

struct element
{
    Types type      = NIL;
    element* left   = nullptr;
    element* right  = nullptr;
    double num      = NAN;
    char symb       = '~';
};

struct Tree
{
    element* head = nullptr;
};

enum Operators
{
    NO_OP = '~',
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    POW = '^',
    SIN = '1',
    COS = '2',
    SH = '3',
    CH = '4'
};

void TreeConstructor (Tree* tree);

void TreeDestructor (Tree* tree);

element* ElementConstructor (Types type, double num, char symb);

void ElementDestructor (element* el);

element* InsertHead   (Tree* tree, Types type, double num, char symb);

element* InsertLeft  (element* el, Types type, double num, char symb);

element* InsertRight (element* el, Types type, double num, char symb);

void CreateGraph (Tree* tree);

void ElementGraph (FILE* graph, element* el);

const char* TypeCheck (Types type);

void CreateTex (Tree* tree);

void ElementTex (FILE* tex, element* el);
