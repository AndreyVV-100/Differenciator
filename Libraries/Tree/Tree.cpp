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

element* InsertHead (Tree* tree, )
{
    ass_ins_emp;

    cal;

    tree->head = el_crt;

    return el_crt;
}

element* InsertLeft (Tree* tree, element* el, const char* str)
{
    ass_ins_n_emp (el->left);

    cal;

    el_crt->prev = el;
    el->left = el_crt;

    return el_crt;
}

element* InsertRight (Tree* tree, element* el, const char* str)
{
    ass_ins_n_emp (el->right);

    cal;

    el_crt->prev = el;
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
        fprintf (graph, "\"%p:\\n `%s`\";\n", tree->head, tree->head->str);
        ElementDump (graph, tree->head, &passed_elems, tree->size);
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

int ElementDump (FILE* graph, element* el, size_t* passed_elems, size_t size)
{
    assert (graph);
    assert (el);
    assert (passed_elems);

    (*passed_elems)++;
    if (*passed_elems > size)
        return 1;

    if (el->left)
    {
        fprintf (graph, "\"%p:\\n `%s`\" -> \"%p:\\n `%s`\" [label = \"нет\"]\n;",
            el, el->str, el->left, el->left->str);

        if (ElementDump (graph, el->left, passed_elems, size))
            return 1;
    }

    if (el->left)
    {
        fprintf (graph, "\"%p:\\n `%s`\" -> \"%p:\\n `%s`\" [label = \"да\"]\n;",
            el, el->str, el->right, el->right->str);

        if (ElementDump (graph, el->right, passed_elems, size))
            return 1;
    }

    return 0;
}
