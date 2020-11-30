#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#pragma warning (disable : 4996)

#define ass_ins_emp assert (tree);                  \
                    assert (tree->head == nullptr);

#define ass_ins_n_emp(pos)  assert (tree);                  \
                            assert (str);                   \
                            assert (el);                    \
                            assert (tree->head != nullptr); \
                            assert (tree->size != 0);       \
                            assert (pos == nullptr);

#define create_el element* el_crt = ElementConstructor ();

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

int ElementDump (FILE* graph, element* el, size_t* passed_elems, size_t size);
