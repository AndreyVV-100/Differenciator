#include "Tree.h"
#include "Differenciator.h"
#include "InOut.h"

int main ()
{
    Tree function = {};
    if (ReadFunction (&function, "EqFiles/eq1.txt"))
        return 0;

    CreateGraph (&function);
    CreateTex   (&function);

    return 0;
}


