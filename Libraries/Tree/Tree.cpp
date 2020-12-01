#include "Tree.h"

// Types Names
const char* NUM_NAME  = "Number";
const char* OPER_NAME = "Operator";
const char* VAR_NAME  = "Variable";
const char* NIL_NAME  = "Not determined";
const char* ERR_NAME  = "Error!";

void TreeConstructor (Tree* tree)
{
    assert (tree);
    tree->head = nullptr;
    return;
}

void TreeDestructor (Tree* tree)
{
    assert (tree);

    if (tree->head != nullptr)
        ElementDestructor (tree->head);

    tree->head = nullptr;

    return;
}

element* ElementConstructor (Types type, double num, char symb)
{
    element* el_crt = (element*)calloc (1, sizeof (*el_crt));
    if (!el_crt)
        return nullptr;

    *el_crt = { type, nullptr, nullptr, num, symb };
    return el_crt;
}

void ElementDestructor (element* el)
{
    assert (el);

    element* left = el->left;
    element* right = el->right;

    el->left  = nullptr;
    el->right = nullptr;
    el->num   = NAN;
    el->symb  = '\0';
    el->type  = NIL;
    free (el);

    if (left != nullptr)
        ElementDestructor (left);

    if (right != nullptr)
        ElementDestructor (right);

    return;
}

element* InsertHead   (Tree* tree, Types type, double num, char symb)
{
    assert (tree);
    assert (tree->head == nullptr);

    create_el;
    tree->head = el_crt;

    return el_crt;
}

element* InsertLeft  (element* el, Types type, double num, char symb)
{
    assert (el->left);

    create_el;
    el->left = el_crt;

    return el_crt;
}

element* InsertRight (element* el, Types type, double num, char symb)
{
    assert (el->right);

    create_el;
    el->right = el_crt;

    return el_crt;
}

void CreateDump (Tree* tree)
{
    assert (tree);

    FILE* graph = fopen ("Graph/out.dot", "w");
    assert (graph);
    fprintf (graph, "digraph G{\n" "rankdir = HR;\n node[shape=box];\n");

    size_t passed_elems = 0;

    if (tree->head)
    {
        fprintf (graph, "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\";\n",
                 tree->head, TypeCheck (tree->head->type), tree->head->symb, tree->head->num);
        ElementDump (graph, tree->head);
    }
    else
        fprintf (graph, "\"No elements\";\n");

    fprintf (graph, "}");
    fclose (graph);

    system ("dot -Tpng Graph\\out.dot -o Graph\\gr.png");
    system ("start Graph\\gr.png");

    return;
}

int ElementDump (FILE* graph, element* el)
{
    assert (graph);
    assert (el);

    if (el->left)
    {
        fprintf (graph, "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\" -> "
                        "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\";\n",
                       el,       TypeCheck (el->type),       el->symb,       el->num, 
                 el->left, TypeCheck (el->left->type), el->left->symb, el->left->num);

        if (ElementDump (graph, el->left))
            return 1;
    }

    if (el->right)
    {
        fprintf (graph, "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\" -> "
                        "\"Point: %p\\n %s\\n Symb: %c\\n Num: %lf\";\n",
                        el,        TypeCheck (el->type),        el->symb,        el->num, 
                 el->right, TypeCheck (el->right->type), el->right->symb, el->right->num);

        if (ElementDump (graph, el->right))
            return 1;
    }

    return 0;
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