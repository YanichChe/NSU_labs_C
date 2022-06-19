#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define SIZE 100
#define _CRT_SECURE_NO_WARNINGS

//////////////////////////////////////////

bool isCorrect(char num[], int sysFrom, int sysTo);
bool checkSystem(int sysFrom, int sysTo);

int getIntLen(char *num);
int getNumber(char symbol);
char getLetter(int number);

long double toDec(int system, char *num);
int fromDecIntPart(long int number, int system, int *index, char *result);
char *getResult(long double number, int system);

// MAIN //////////////////////////////////

int main(void)
{
	int sysFrom, sysTo;
	char num[SIZE];

	if (scanf("%d %d\n%13s", &sysFrom, &sysTo, num) != 3 || (isCorrect(num, sysFrom, sysTo) == false))
	{
		printf("bad input");
		return EXIT_SUCCESS;
	}

	long double resToDec = toDec(sysFrom, num);

	char *result = getResult(resToDec, sysTo);
	printf("%s", result);

	return EXIT_SUCCESS;
}

// GET LENGHTS ///////////////////////////

int getIntLen(char *num)
{
	int len = 0;
	for (int i = 0; i < strlen(num); i++)
	{
		if (num[i] != '.' && num[i] != '\0')
		{
			len++;
		}
		else
			return len;
	}

	return len;
}

// CHECK ERRORS //////////////////////////

bool checkSystem(int sysFrom, int sysTo)
{
	if (sysFrom > 16 || sysFrom < 2 || sysTo > 16 || sysTo < 2)
	{
		return false;
	}

	return true;
}

bool isCorrect(char num[], int sysFrom, int sysTo)
{
	if (checkSystem(sysFrom, sysTo) == false)
		return false;

	bool isNumbersAfterPoint = false;
	int countPoints = 0;
	int currentNumber;
	int i = 0;

	while (num[i] != '\0'){

		if (num[i] != '.'){

			if (countPoints == 1)
				isNumbersAfterPoint = true;

			currentNumber = getNumber(num[i]);

			if (currentNumber >= sysFrom)
				return false;
		}

		else{
			if (i == 0)
				return false;
			countPoints += 1;
		}

		i += 1;
	}

	if (countPoints > 1 || (countPoints == 1 && isNumbersAfterPoint == false))
		return false;

	return true;
}

// LETTERS AND NUMBERS MATCHING //////////

int getNumber(char symbol)
{
	if (symbol >= '0' && symbol <= '9')
		return symbol - '0';

	else if (symbol >= 'a' && symbol <= 'f')
		return symbol - 'a' + 10;

	else if (symbol >= 'A' && symbol <= 'F')
		return symbol - 'A' + 10;

	else
		return 16;
}

char getLetter(int number)
{
	char letters[] = "0123456789abcdef";
	return letters[number];
}

// FROM DEC ////////////////////////////////

int fromDecIntPart(long int number, int system, int *index, char *result)
{
	if (number != 0){
		fromDecIntPart(number / system, system, *index, result);
		result[(*index)++] = getLetter(number % system);
	}
}

char *getResult(long double number, int system)
{
	int index = 0;
	char result[SIZE];

	long int intPart = number;
	long double doublePart = number - intPart;

	fromDecIntPart(intPart, system, &index, result);

	if (doublePart > 0){
		
		result[index++] = '.';

		while (doublePart > 0){
			doublePart *= system;
			result[index++] = getLetter((int)doublePart);
			doublePart -= (int)doublePart;
		}
		
	}

	result[index] = '\0';
	return result;
}

// TO DEC ////////////////////////////////

long double toDec(int system, char *num)
{
	int currentNumber;
	long double result = 0;

	int lenIntPart = getIntLen(num);
	int lenFractPart = strlen(num) - lenIntPart - 1;

	int i;
	for (i = 0; i < lenIntPart; ++i){

		if (num[i] == '\0')
			break;
		else{
			currentNumber = getNumber(num[i]);
			result += currentNumber * pow(system, (lenIntPart - i - 1));
		}

	}

	for (int i = 1; i <= lenFractPart; ++i){

		if (num[lenIntPart + i] == '\0')
			break;

		else{
			currentNumber = getNumber(num[lenIntPart + i]);
			result += currentNumber * pow(system, (-1) * i);
		}

	}
	return result;
}