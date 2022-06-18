#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 100
#define _CRT_SECURE_NO_WARNINGS 

int isCorrect(char num[], int sys1, int sys2);
int corresp(char sys);
char correspLetters(int s);
long double toDec(char intPart[], char fractPart[], int sys, int lenIntPart, int lenFractPart);
void fromDecToNewSystemIntPart(long long int intPart, int sys, char* resIntPart);
void fromDecToNewSystemFractPart(long double fractPart, int sys, char* resFractPart);

int main(void) {

	int sys1, sys2;
	char num[SIZE];
	int isOkey = 1;

	if (scanf("%d %d\n%13s", &sys1, &sys2, num) != 3) {
		printf("bad input");
		isOkey = 0;
	}

	if (isCorrect(num, sys1, sys2) == 0) {
		printf("bad input");
		isOkey = 0;
	}

	if (isOkey == 1) {

		int i = 0;
		int countIntPart = 0;
		int countFractPart = 0;
		char intPart[SIZE] = { 'z' };
		char fractPart[SIZE] = { 'z' };

		while (num[i] != '\0' && num[i] != '.') {
			intPart[i] = num[i];
			++countIntPart;
			++i;
		}
		if (num[i] == '.') {
			int j = 0;
			++i;
			while (num[i] != '\0') {
				fractPart[j] = num[i];
				++i;
				++j;
				++countFractPart;
			}
		}

		char resIntPart[SIZE] = { '0' };
		char resFractPart[SIZE] = { '0' };
		long double newNum = toDec(intPart, fractPart, sys1, countIntPart - 1, countFractPart - 1);
		fromDecToNewSystemIntPart((long long int)newNum, sys2, resIntPart);
		long double decfractpart = newNum - (long long int)newNum;
		fromDecToNewSystemFractPart(decfractpart, sys2, resFractPart);
		printf("%s.%s\n", resIntPart, resFractPart);
	
	}
	return EXIT_SUCCESS;
}
/*перевод из десятичной в новую систему счисления для целой части числа*/
void fromDecToNewSystemFractPart(long double fractPart, int sys, char* resFractPart) {

	int i = 0;
	double long n = 1;
	while (i < 12 && n != 0) {
		n = fractPart * sys;
		if (n != 0) {
			resFractPart[i] = correspLetters(((int)n));
		}
		fractPart = n - (int)n;
		++i;
	}
}
/*перевод из десятичной в новую систему счисления для дробной части числа*/
void fromDecToNewSystemIntPart(long long int intPart, int sys, char* resIntPart) {

	char buffer[SIZE] = { 'z' };
	int i = 0;

	while (intPart >= sys) {
		buffer[i] = correspLetters((intPart % sys));
		intPart /= sys;
		++i;
	}
	buffer[i] = correspLetters(intPart);

	int j = 0;
	for (; i >= 0; --i) {
		if (buffer[i] != 'z') {
			resIntPart[j] = buffer[i];
			++j;
		}
	}
}
/*проверка на верность введенных значений*/
int isCorrect(char num[], int sys1, int sys2) {
	if (sys1 > 16 || sys1 < 2 || sys2>16 || sys2 < 2) {
		return 0;
	}
	int isNumbersAfterPoint = 0;
	int isOnePoint = 0;
	int currentNumber;
	int i = 0;
	while (num[i] != '\0') {
		if (num[i] != '.') {
			if (isOnePoint == 1) {
				isNumbersAfterPoint = 1;
			}
			currentNumber = corresp(num[i]);
			if (currentNumber >= sys1) {
				return 0;
			}
		}
		else {
			if (i == 0) {
				return 0;
			}
			isOnePoint += 1;
		}
		i += 1;
	}
	if (isOnePoint > 1 || (isOnePoint == 1 && isNumbersAfterPoint == 0)){
		return 0;
	}
	return 1;
}
/*соответсвия числам*/
int corresp(char sys) {
	if (sys >= '0' && sys <= '9') {
		return sys - '0';
	}
	else if (sys >= 'a' && sys <= 'f') {
		return sys - 'a' + 10;
	}
	else if (sys >= 'A' && sys <= 'F') {
		return sys - 'A' + 10;
	}
	else {
		return 16;
	}
}
/*соответствия буквам*/
char correspLetters(int s) {
	char letters[] = "0123456789abcdef";
	return letters[s];
}
/*перевод в десятичную систему счисления*/
long double toDec(char intPart[], char fractPart[], int sys, int lenIntPart, int lenFractPart) {
	int currentNumber;
	long double res = 0;
	int i = 0;
	for (; i <= lenIntPart; ++i) {
		if (intPart[i] == 'z')
			break;
		else {
			currentNumber = corresp(intPart[i]);
			res += currentNumber * pow(sys, (lenIntPart - i));
		}
	}
	i = 0;
	for (; i <= lenFractPart; ++i) {
		if (fractPart[i] == 'z')
			break;
		else {
			currentNumber = corresp(fractPart[i]);
			res += currentNumber * pow(sys, -(i + 1));
		}
	}
	return res;
}
