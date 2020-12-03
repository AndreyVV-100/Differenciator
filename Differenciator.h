#define dL DiffBranch (el->left)
#define dR DiffBranch (el->right)
#define cL CopyBranch (el->left)
#define cR CopyBranch (el->right)
#define Un UnionBranches

void DiffFunction (Tree* func, Tree* diff);

element* CopyBranch (element* el);

element* DiffBranch (element* el);

element* UnionBranches (char oper, element* el_left, element* el_right);
