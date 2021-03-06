// CountWordsFromFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
//#include <string>
#include <list>

using namespace std;

const int MAX_SIZE = 10000;
const int MAX_NODES = 5000;


HANDLE open_for_read() {
	return CreateFile(TEXT("file.txt"), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

int read_file(char* text) {

	HANDLE file = open_for_read();

	if (INVALID_HANDLE_VALUE == file) {
		return -1;
	}

	LARGE_INTEGER size;
	GetFileSizeEx(file, &size);
	__int64 size_n = size.QuadPart;
	CloseHandle(file);

	file = open_for_read();
	unsigned long success_bytes;
	//char text[MAX_SIZE] = "";
	ReadFile(file, text, size_n, &success_bytes, NULL);

	if (size_n != success_bytes) {
		CloseHandle(file);
		cout << "Expected " << success_bytes << " bytes in file, but actually was "
			<< size.LowPart << "." << endl;
		getchar();
		return -1;
	}

	CloseHandle(file);
	return success_bytes;
}

void printword(char *start, char *end) {
	while (*start != ' ' && start <= end) {
		cout << *start;
		start++;
	}
}

//return pointer to first character after word, if start != ' '
char * readword(char *start, char *end) {
	while (*start != ' ' && start <= end)
		start++;
	return start;
}

//return first letter pointer in text, if start == ' '
char * skipspaces(char *start, char *end) {
	while (*start == ' ' && start <= end)
		start++;
	return start;
}

// compares two words in array
bool comparewith(char *first, char *second, char *end) {
	while (first <= end && second <= end) {
		if (*first != *second)
			return false;
		if (*first == ' ')
			return true;
		first++;
		second++;
	}

	return true;
}

char *removeword(char *start, char *end) {
	while (*start != ' ' && start <= end) {
		*start = ' ';
		start++;
	}
	return start;
}

int countwordsfrom(char *word, char *from, char *end) {
	int count = 0;
	
	while (from <= end) {
		if (comparewith(word, from, end))
			count++;
		from = skipspaces(readword(from, end), end);
	}

	return count;
}

void removeinputs(char *word, char *from, char *end) {
	while (from <= end) {
		if (comparewith(word, from, end))
			removeword(from, end);
		from = skipspaces(readword(from, end), end);
	}
}

int countandremove(char *word, char *end) {
	char *from = skipspaces(readword(word, end), end);

	int count = countwordsfrom(word, from, end);
	removeinputs(word, from, end);
	removeword(word, end);

	return count + 1;
}

void printcounts(char *text, char *end) {
	char *from = text;

	while (from <= end) {
		from = skipspaces(from, end);
		printword(from, end);
		cout << " : " << countandremove(from, end) << endl;
	}
}

int main(int argc, char* argv[])
{

	char text[MAX_SIZE] = "";
	int length = read_file(text);
	printcounts(&text[0], &text[length - 1]);

	getchar();

    return 0;
}