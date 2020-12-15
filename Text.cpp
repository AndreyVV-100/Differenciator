#include "Text.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void ConstructorText (Text* text, const char* file_in)
{
	assert (text);
	
	text->size_text = ReadTxt (&(text->buffer), file_in);
	text->num_lines = CountSymbols (text->buffer + 1, '\n') + 1;

	text->lines = (Line*) calloc (text->num_lines, sizeof (*(text->lines)));
	text->n_empty_lines = DoLines (text->buffer, text->lines, text->num_lines);

	return;
}

void DestructorText (Text* text)
{
	assert (text);
	assert (text->buffer);
	assert (text->lines);

	free (text->buffer);
	free (text->lines);

	return;
}

int CountSize (FILE* file)
{
	fseek(file, 0, SEEK_END);
	size_t num_symbols = ftell(file);
	fseek(file, 0, SEEK_SET);

	return num_symbols;
}

size_t ReadTxt (char** text, const char* file_name)
{
	assert (text);
	assert (file_name);

	FILE* file = fopen (file_name, "r");
	if (file == nullptr)
	{
		printf ("[Input error] Unable to open file \"%s\"\n", file_name);
		return 0;
	}

	size_t num_symbols = CountSize (file);

	*text = (char*) calloc (num_symbols + 2, sizeof (**text));

	if (*text == nullptr)
	{
		printf ("[Error] Unable to allocate memory\n");
		return 0;
	}

	fread (*text, sizeof (**text), num_symbols, file);
	fclose (file);

	(*text)[num_symbols] = '\0';

	return num_symbols;
}

int CountSymbols (char* text, const char str)
{
	assert (text);
	int counter = 0;
	if (*text == str) 
		counter++;
	while (text = strchr (text + 1, str))
		counter++;
	return counter;
}

int DoLines(char* text, Line* lines, int num_lines)
{
	assert(text);
	assert(lines);
	assert(num_lines > 0);

	char* first = nullptr,
		* second = text - 1;

	int position = 0;

	for (int i_line = 0; i_line < num_lines; i_line++)
	{
		size_t count_empty = 0;

		do
		{
			position++;
			count_empty++;
			first = second + 1;
			second = strchr (first, '\n');
		} while ((*first == '\r' || second - first < 1) && second);

		num_lines -= --count_empty;

		if (second)
		{
			lines[i_line].point    = first;
			lines[i_line].lenght   = second - first + 1;
			lines[i_line].position = position;

			*second = '\0';
		}
		else
		{
			second = strchr(first + 1, '\0');
			if (second && second - first)
			{
				lines[i_line].point    = first;
				lines[i_line].lenght   = second - first - 1;
				lines[i_line].position = position;

				*second = '\0';
			}
		}
	}

	assert(num_lines > 0);
	return num_lines;
}

void  SkipSpaces (char** eq)
{
	assert (eq);
	assert (*eq);

	while (**eq == ' ' || **eq == '\t' || **eq == '\r')
		*eq += 1;

	return;
}
