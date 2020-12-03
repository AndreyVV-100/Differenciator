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

void CreateTex (Tree* tree)
{
    assert (tree);

    FILE* tex = fopen ("AllDumps/Tree.tex", "w");
    assert (tex);
    fprintf (tex,   "\\documentclass[12pt]{article}\n"
                    "\\usepackage[russian]{babel}\n\n"
                    "\\begin{document}\n");

    if (tree->head)
    {
        fprintf (tex, "\\begin{equation}\n");
        ElementTex (tex, tree->head);
        fprintf (tex, "\\end{equation}\n");
    }
    else
        fprintf (tex, "No elements\n");

    fprintf (tex, "\\end{document}\n");
    fclose (tex);

    system ("calltex>nul Tree AllDumps");
    return;
}

void ElementTex (FILE* tex, element* el)
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
        #define TexL ElementTex (tex, el->left)
        #define TexR ElementTex (tex, el->right)

        case ADD:
        case SUB:
            fprintf (tex, "(");
            TexL;
            fprintf (tex, " %c ", el->symb);
            TexR;
            fprintf (tex, ")");
            break;
        case MUL:
            TexL;
            fprintf (tex, " \\cdot ", el->symb);
            TexR;
            break;
        case DIV:
            fprintf (tex, " \\frac{");
            TexL;
            fprintf (tex, "}{");
            TexR;
            fprintf (tex, "}");
            break;
        case POW:
            if (el->left->type == OPER)
                fprintf (tex, "(");
            TexL;
            if (el->left->type == OPER)
                fprintf (tex, ")");
            fprintf (tex, "^{");
            TexR;
            fprintf (tex, "}");
            break;
        case SIN:
            fprintf (tex, "\\sin ");
            TexR;
            break;
        case COS:
            fprintf (tex, "\\cos ");
            TexR;
            break;
        case SH :
            fprintf (tex, "\\sh ");
            TexR;
            break;
        case CH :
            fprintf (tex, "\\ch ");
            TexR;
            break;

        #undef TexL
        #undef TexR
    }

    return;
}
