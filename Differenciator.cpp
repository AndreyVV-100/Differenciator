#include "Tree.h"
#include "Differenciator.h"
#include "InOut.h"

#pragma warning (disable : 26451)
/*
    Арифметическое переполнение: использование оператора "*" на байтовом значении 4 и
    приведение результата к байтовому значению 8. Приведите значение к более широкому
    типу перед вызовом оператора "*", чтобы избежать переполнения (io.2).
*/

int main ()
{
    Tree function = {};
    if (ReadFunction (&function, "EqFiles/eq2.txt"))
        return 0;
    Tree differential = {};

    DiffFunction (&function, &differential);
    CreateTex (&differential);
    return 0;
}

void DiffFunction (Tree* func, Tree* diff)
{
    assert (func);
    assert (diff);

    TreeDestructor  (diff);
    TreeConstructor (diff);

    diff->head = DiffBranch (func->head);
    return;
}

element* CopyBranch (element* el)
{
    assert (el);

    if (el->type == NIL)
        return nullptr;

    if (el->type == NUM)
        return CR_N (el->num);

    if (el->type == VAR)
        return CR_V (el->symb);

    element* el_ret = CR_O (el->symb);
    assert (el_ret);

    if (el->left)
        el_ret->left  = CopyBranch (el->left);
    if (el->right)
        el_ret->right = CopyBranch (el->right);

    return el_ret;
}

element* DiffBranch (element* el)
{
    assert (el);

    if (el->type == NIL)
        return nullptr;

    if (el->type == NUM)
        return CR_N (0);

    if (el->type == VAR)
        return CR_N (1);

    // d - derivative
    // c - copy
    // r - return
    
    switch (el->symb)
    {
        case ADD:
        case SUB:
            return Un (el->symb, dL, dR);
        case MUL:
            return Un (ADD, Un (MUL, dL, cR), Un (MUL, cL, dR));
        case DIV:
        {
            element* numerator   = Un (SUB, Un (MUL, dL, cR), Un (MUL, cL, dR));
            element* denominator = Un (POW, cR, CR_N (2));
            return Un (DIV, numerator, denominator);
        }
        case POW:
        {
            element* power = nullptr;
            element* coeff = nullptr;

            if (el->right->type == NUM)
            {
                power = Un (POW, cL, CR_N (el->right->num - 1));
                coeff = Un (MUL, cR, dL);
            }

            else if (el->left->type == NUM)
            {
                power = Un (POW, cL, cR);
                coeff = Un (MUL, Un (LN, nullptr, cL), dR);
            }

            else
            {
                power = Un (POW, cL, cR);

                element ln_left   = { OPER, nullptr,   el->left, NAN, LN };
                element hard_diff = { OPER, el->right, &ln_left, NAN, MUL };
                coeff = DiffBranch (&hard_diff);
            }

            return Un (MUL, coeff, power);
        }
        case SIN:
            return Un (MUL, dR, Un (COS, nullptr, cR));
        case COS:
            return Un (MUL, CR_N (-1), Un (MUL, dR, Un (SIN, nullptr, cR)));
        case SH:
            return Un (MUL, dR, Un (CH, nullptr, cR));
        case CH:
            return Un (MUL, dR, Un (SH, nullptr, cR));
        case LN:
            return Un (DIV, dR, cR);
    }
    return nullptr;
}

element* UnionBranches (char oper, element* el_left, element* el_right)
{
    element* union_element = CR_O (oper);
    union_element->left    = el_left;
    union_element->right   = el_right;
    return union_element;
}