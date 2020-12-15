#include "Calculator.h"
#include "Text.h"
// ToDo: CR_O include

bool Require_F (char** eq, char symb)
{
	assert (eq);
	assert (*eq);

	if (**eq == symb)
	{
		*eq += 1;
		return 0;
	}

	printf ("Require \'%c\' (%d) error: %s\n", symb, symb, *eq);
	return 1;
}

void GetG (const char* file_path, Tree* tree)
{
	char* equation = nullptr;
	ReadTxt (&equation, file_path);
	if (!equation)
		return;
	
	char* eq = equation;
	double vars['z' - 'a' + 1] = {};

	for (size_t i_var = 0; i_var <= 'z' - 'a'; i_var++)
		vars[i_var] = NAN;

	while (strchr (eq, '='))
		if (GetVarDefine (&eq, vars))
		{
			printf ("Bad var define.");
			free (equation);
			return;
		}

	TreeDestructor  (tree);
	TreeConstructor (tree);

	tree->head = GetE (&eq, vars);
	Require_F (&eq, '\0');

	free (equation);
	return;
}

bool GetVarDefine (char** eq, double* vars)
{
	ass;
	
	if (!islower (**eq))
	{
		printf ("Variable error: %s", *eq);
		return 1;
	}

	SkSp;
	size_t position = **eq - 'a';
	*eq += 1;

	SkSp;
	Require_F (eq, '=');
	SkSp;

	int shift = 0;
	sscanf (*eq, "%lf%n", vars+position, &shift);
	*eq += shift;
	if (isnan (vars[position]))
		return 1;

	SkSp;
	Require_F (eq, '\n');
	return 0;
}

element* GetE (char** eq, double* vars)
{
	ass;

	element* result = GetT (eq, vars);
	SkSp;

	while ((**eq == '+' || **eq == '-') && result)
	{
		bool operat = (**eq == '+') ? 1:0;
		*eq += 1;

		element* new_result = ElementConstructor (OPER, NAN, operat ? ADD : SUB);
		
		new_result->left = result;
		new_result->right = GetT (eq, vars);

		if (new_result->right == nullptr)
		{
			ElementDestructor (new_result);
			new_result = nullptr;
		}

		result = new_result;
		SkSp;
	}

	return result;
}

element* GetT (char** eq, double* vars)
{
	ass;

	element* result = GetDegree (eq, vars);
	SkSp;

	while ((**eq == '*' || **eq == '/') && result)
	{
		bool operat = (**eq == '*') ? 1 : 0;
		*eq += 1;

		element* new_result = ElementConstructor (OPER, NAN, operat ? MUL : DIV);

		new_result->left = result;
		new_result->right = GetDegree (eq, vars);

		if (new_result->right == nullptr)
		{
			ElementDestructor (new_result);
			new_result = nullptr;
		}

		result = new_result;
		SkSp;
	}

	return result;
}

element* GetDegree (char** eq, double* vars)
{
	ass;

	element* result = GetUnary (eq, vars);
	SkSp;

	if (**eq == '^' && result)
	{
		*eq += 1;

		element* new_result = ElementConstructor (OPER, NAN, POW);

		new_result->left  = result;
		new_result->right = GetDegree (eq, vars);
		
		if (new_result->right == nullptr)
		{
			ElementDestructor (new_result);
			new_result = nullptr;
		}

		result = new_result;
	}

	return result;
}

element* GetUnary (char** eq, double* vars)
{
	ass;
	SkSp;

	if (islower (**eq))
	{
		switch (**eq)
		{
			#define check_and_get(cmd, cmd_big, len) if (strncmp (#cmd, *eq, len) == 0)				\
													 {												\
														*eq += len;									\
														element* result =							\
														ElementConstructor (OPER, NAN, cmd_big);	\
														result->right = GetP (eq, vars);			\
														if (result->right == nullptr)				\
														{											\
															ElementDestructor (result);				\
															result = nullptr;						\
														}											\
														return result;								\
													 }
			#include "FunctionsList.h"
			#undef check_and_get
		}
	}

	return GetP (eq, vars);
}

element* GetP (char** eq, double* vars)
{
	ass;
	SkSp;

	element* result = nullptr;
	if (**eq == '(')
	{
		*eq += 1;
		result = GetE (eq, vars);
		Require (eq, ')');
	}
	else if (islower (**eq))
		result = GetVar (eq, vars);
	else
		result = GetN (eq, vars);

	return result;
}

element* GetVar (char** eq, double* vars)
{
	ass;
	assert (islower (**eq));
	
	if (isnan (vars[**eq - 'a']))
	{
		printf ("No variable: \'%c\'\n", **eq);
		return nullptr;
	}

	*eq += 1;
	return ElementConstructor (VAR, NAN, *(*eq - 1));
}

element* GetN (char** eq, double* vars)
{
	assert (eq);
	assert (*eq);

	double result = 0;
	int OK = 0;

	while (isdigit (**eq))
	{
		OK = 1;
		result = result * 10 + (**eq) - '0';
		*eq += 1;
	}

	if (OK && **eq == '.')
	{
		// OK - shift
		OK = 0;
		*eq += 1;
	
		while (isdigit (**eq))
		{
			OK++;
			result = result * 10 + (**eq) - '0';
			*eq += 1;
		}

		result *= pow (0.1, OK);
	}

	if (!OK)
	{
		printf ("Number error: %s", *eq);
		return nullptr;
	}

	return ElementConstructor (NUM, result, NO_OP);
}
