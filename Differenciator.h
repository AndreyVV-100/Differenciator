#define dL DiffBranch (el->left)
#define dR DiffBranch (el->right)
#define cL CopyBranch (el->left)
#define cR CopyBranch (el->right)
#define Un UnionBranches

#define elem (*el)
#define L (elem->left)
#define R (elem->right)

#define move_branch(what)  { CreateTex (elem, elem->what, SIMPLIFY); \
                             element* el_del = elem;                 \
                             elem = elem->what;                      \
                             el_del->what = nullptr;                 \
                             ElementDestructor (el_del);             \
                             return 1;  }

#define num_branch(num) { element* new_elem = CR_N (num);       \
                          CreateTex (elem, new_elem, SIMPLIFY); \
                          ElementDestructor (elem);             \
                          elem = new_elem;                      \
                          return 1; }

void DiffFunction (Tree* func, Tree* diff);

element* CopyBranch (element* el);

element* DiffBranch (element* el);

element* UnionBranches (char oper, element* el_left, element* el_right);

bool Simplifier (element** el);

bool SimpleAdd (element** el);

bool SimpleSub (element** el);

bool SimpleMul (element** el);

bool SimpleDiv (element** el);

bool SimplePow (element** el);

void GetG (const char* file_path, Tree* tree);
