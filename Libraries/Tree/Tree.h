#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#pragma warning (disable : 4996)

#define create_el element* el_crt = ElementConstructor (type, num, symb)

#define tree_error_printf {                                                                      \
                          printf ("\n""������: ������ ����������. ���������� � ������������.\n");\
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
    char symb       = '\0';
};

struct Tree
{
    element* head = nullptr;
};

void TreeConstructor (Tree* tree);

void TreeDestructor (Tree* tree);

element* ElementConstructor (Types type, double num, char symb);

void ElementDestructor (element* el);

element* InsertHead (Tree* tree, const char* str);

element* InsertLeft (Tree* tree, element* el, const char* str);

element* InsertRight (Tree* tree, element* el, const char* str);

void CreateDump (Tree* tree);

int ElementDump (FILE* graph, element* el);
