#include "Tree.h"

#pragma warning (disable : 26451)
/*
    Арифметическое переполнение: использование оператора "*" на байтовом значении 4 и
    приведение результата к байтовому значению 8. Приведите значение к более широкому
    типу перед вызовом оператора "*", чтобы избежать переполнения (io.2).
*/

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
