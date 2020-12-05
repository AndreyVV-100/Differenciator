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
    if (ReadFunction (&function, "EqFiles/eq4.txt"))
        return 0;
    Tree differential = {};

    DiffFunction (&function, &differential);
    while (Simplifier (&(differential.head)));
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
    // ToDo: denominator, numerator

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

        case TG:
        {
            element* denominator = Un (POW, Un (COS, nullptr, cR), CR_N (2));
            return Un (DIV, dR, denominator);
        }

        case CTG:
        {
            element* denominator = Un (POW, Un (SIN, nullptr, cR), CR_N (2));
            return Un (DIV, Un (MUL, CR_N(-1), dR), denominator);
        }

        case SH:
            return Un (MUL, dR, Un (CH, nullptr, cR));

        case CH:
            return Un (MUL, dR, Un (SH, nullptr, cR));

        case TH:
        {
            element* denominator = Un (POW, Un (CH, nullptr, cR), CR_N (2));
            return Un (DIV, dR, denominator);
        }

        case CTH:
        {
            element* denominator = Un (POW, Un (SH, nullptr, cR), CR_N (2));
            return Un (DIV, Un (MUL, CR_N (-1), dR), denominator);
        }

        case ARCSIN:
        {
            element* denominator = Un (SUB, CR_N (1), Un (POW, cR, CR_N (2)));
            return Un (DIV, dR, Un (POW, denominator, CR_N (0.5)));
        }

        case ARCCOS:
        {
            element* numerator = Un (MUL, CR_N (-1), dL);
            element* denominator = Un (SUB, CR_N (1), Un (POW, cR, CR_N (2)));
            return Un (DIV, numerator, Un (POW, denominator, CR_N (0.5)));
        }

        case ARCTG:
        {
            element* denominator = Un (ADD, CR_N (1), Un (POW, cR, CR_N (2)));
            return Un (DIV, dR, denominator);
        }

        case ARCCTG:
        {
            element* numerator = Un (MUL, CR_N (-1), dL);
            element* denominator = Un (ADD, CR_N (1), Un (POW, cR, CR_N (2)));
            return Un (DIV, numerator, denominator);
        }

        case ARCSH:
        {
            element* denominator = Un (ADD, CR_N (1), Un (POW, cR, CR_N (2)));
            return Un (DIV, dR, Un (POW, denominator, CR_N (0.5)));
        }

        case ARCCH:
        {
            element* denominator = Un (SUB, Un (POW, cR, CR_N (2)), CR_N (1));
            return Un (DIV, dR, Un (POW, denominator, CR_N (0.5)));
        }

        case ARCTH:
        case ARCCTH:
        {
            element* denominator = Un (SUB, CR_N (1), Un (POW, cR, CR_N (2)));
            return Un (DIV, dR, denominator);
        }

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

bool IsEqual (double num1, double num2)
{
    const double precision = 1e-10;
    return fabs (num1 - num2) < precision;
}

bool Simplifier (element** el)
{
    if (!el || !elem || elem->type != OPER)
        return 0;
    
    while (Simplifier (&L));
    while (Simplifier (&R));

    switch (elem->symb)
    {
        case ADD: return SimpleAdd (el);
        case SUB: return SimpleSub (el);
        case MUL: return SimpleMul (el);
        case DIV: return SimpleDiv (el);
        case POW: return SimplePow (el);
    }

    return 0;
}

bool SimpleAdd (element** el)
{
    assert (el);
    assert (elem);
    assert (elem->symb == ADD);

    if (L->type == NUM && R->type == NUM)
        num_branch (L->num + R->num);

    if (L->type == NUM && IsEqual (L->num, 0))
        move_branch (right);

    if (R->type == NUM && IsEqual (R->num, 0))
        move_branch (left);
    
    return 0;
}

bool SimpleSub (element** el)
{
    assert (el);
    assert (elem);
    assert (elem->symb == SUB);

    if (L->type == NUM && R->type == NUM)
        num_branch (L->num - R->num);

    if (R->type == NUM && IsEqual (R->num, 0))
        move_branch (left);

    // ToDo: 0 - f(x) = -1 * f(x)
    return 0;
}

bool SimpleMul (element** el)
{
    assert (el);
    assert (elem);
    assert (elem->symb == MUL);

    if (L->type == NUM && R->type == NUM)
        num_branch (L->num * R->num);

    if (L->type == NUM && IsEqual (L->num, 1))
        move_branch (right);

    if (R->type == NUM && IsEqual (R->num, 1))
        move_branch (left);

    if (L->type == NUM && IsEqual (L->num, 0))
        num_branch (0);

    if (R->type == NUM && IsEqual (R->num, 0))
        num_branch (0);

    return 0;
}

bool SimpleDiv (element** el)
{
    assert (el);
    assert (elem);
    assert (elem->symb == DIV);

    if (L->type == NUM && R->type == NUM)
        num_branch (L->num / R->num);

    if (R->type == NUM && IsEqual (R->num, 1))
        move_branch (left);

    if (L->type == NUM && IsEqual (L->num, 0))
        num_branch (0);

    return 0;
}

bool SimplePow (element** el)
{
    assert (el);
    assert (elem);
    assert (elem->symb == POW);

    if (L->type == NUM && R->type == NUM)
        num_branch (pow(L->num, R->num));

    if (L->type == NUM && IsEqual (L->num, 1))
        num_branch (1);

    if (R->type == NUM && IsEqual (R->num, 1))
        move_branch (left);

    if (L->type == NUM && IsEqual (L->num, 0))
        num_branch (0);

    if (R->type == NUM && IsEqual (R->num, 0))
        num_branch (1);

    return 0;
}
