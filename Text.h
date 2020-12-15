#pragma once
#pragma warning(disable : 6387)
//"file_out" может быть равен "0":  это не соответствует спецификации функции "fclose".
#pragma warning(disable : 4996)
//'fopen': This function or variable may be unsafe.Consider using fopen_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.

#include <stdio.h>

/*!
	The struct contains a pointer to string and buffer and length
of this string ('including '\0').
*/

struct Line
{
	char* point = nullptr;
	size_t lenght = 0;
	size_t position = 0;
};

//! The struct is full set of data.

struct Text
{
	char* buffer = nullptr;
	Line* lines = nullptr;

	size_t num_lines = 0;
	size_t n_empty_lines = 0;
	size_t size_text = 0;
};

void ConstructorText (Text* text, const char* file_in);

void DestructorText  (Text* text);

/*!
	This function are reading txt file.
	@param [out] text Place, when point to text will be saved.
	@param [in] file_name Name of file, when text will be reading.
*/

size_t ReadTxt (char** text, const char* file_name);

/*!
	This function count size of file.
	@param [in] file Checking file.
*/

int CountSize(FILE* file);

/*!
	Count, how many symbols are in this text
	@param [in] text Text, when symbols need to be countable.
	@param [in] str Symbols, what was countable.
	@return Return number of symbols.
*/

int CountSymbols (char* text, const char str);

/*!
	This function transform text to massive of Lines.
	@param [in] text Text, what will be transformed.
	@param [out] lines The place, when massive of Lines will be saved.
	@param [in] num_lines Number of Lines.
*/

int DoLines (char* text, Line* lines, int num_lines);

void  SkipSpaces (char** eq);