#include "Libraries/Tree/Tree.h"
#include "Differenciator.h"

int main ()
{

}

ReadErr ReadFunction (Tree* func, const char* file_name)
{
    assert (file_name);

    FILE* eq = fopen (file_name, "r");
    if (!eq)
    {
        printf ("I'm sorry, I can't open this file.");
        return OPEN;
    }

    func->head = 
}