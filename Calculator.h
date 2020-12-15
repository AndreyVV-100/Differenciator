#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "Tree.h"

#pragma warning (disable : 4996)
#pragma warning (disable : 26451)

#define ass assert (eq);    \
            assert (*eq);   \
            assert (vars)

#define Require(eq, symb) if (Require_F (eq, symb)) return nullptr;

#define SkSp SkipSpaces (eq)

bool Require_F (char** eq, char symb);

void GetG (const char* file_path, Tree* tree, double* vars);

bool GetVarDefine (char** eq, double* vars);

element* GetE (char** eq, double* vars);

element* GetT (char** eq, double* vars);

element* GetDegree (char** eq, double* vars);

element* GetUnary (char** eq, double* vars);

element* GetP (char** eq, double* vars);

element* GetVar (char** eq, double* vars);

element* GetN (char** eq, double* vars);
