#include "Tree.h"

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

    FILE* graph = fopen ("Graph/out1251.dot", "w");
    assert (graph);
    fprintf (graph, "digraph G{\n" "rankdir = HR;\n node[shape=box];\n");

    size_t passed_elems = 0;

    if (tree->head)
    {
        fprintf (graph, "\"Type: %d\\n Symb: %c\\n Num: %d\"",
                 tree->head->type, tree->head->symb, tree->head->num);
        ElementDump (graph, tree->head);
    }
    else
        fprintf (graph, "Нет элементов;\n");

    fprintf (graph, "}");
    fclose (graph);

    system ("win_iconv -f 1251 -t UTF8 \"Graph\\out1251.dot\" > \"Graph\\outUTF.dot\"");
    system ("dot -Tpng Graph\\outUTF.dot -o Graph\\gr.png");
    system ("start Graph\\gr.png");

    return;
}

int ElementDump (FILE* graph, element* el)
{
    assert (graph);
    assert (el);

    if (el->left)
    {
        fprintf (graph, "\"Type: %d\\n Symb: %c\\n Num: %d\" ->"
                        "\"Type: %d\\n Symb: %c\\n Num: %d\" [label = \"нет\"]\n;",
                       el->type,       el->symb,       el->num, 
                 el->left->type, el->left->symb, el->left->num);

        if (ElementDump (graph, el->left))
            return 1;
    }

    if (el->right)
    {
        fprintf (graph, "\"Type: %d\\n Symb: %c\\n Num: %d\" ->"
                        "\"Type: %d\\n Symb: %c\\n Num: %d\" [label = \"нет\"]\n;",
                        el->type,        el->symb,        el->num, 
                 el->right->type, el->right->symb, el->right->num);

        if (ElementDump (graph, el->right))
            return 1;
    }

    return 0;
}
