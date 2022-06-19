#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//////////////////////////////////////////

#define MAX_INPUT_LENGHT 18
#define _CRT_SECURE_NO_WARNINGS

//////////////////////////////////////////

bool inputText(unsigned char* text, int hsumSample);
void inputSample(unsigned char* sample);
int inputLen;

int countHsumSample(unsigned char sample[]);
void printComprassions(unsigned char* sample, unsigned char* text, int ind);
void rabinKarp(int hsumSample, unsigned char sample[]);

// MAIN //////////////////////////////////

int main(void) 
{
	unsigned char sample[MAX_INPUT_LENGHT];
	inputSample(sample);
	
	int hsumSample = countHsumSample(sample);
	rabinKarp(hsumSample, sample);
	return EXIT_SUCCESS;
}

// INPUT //////////////////////////////////

void inputSample(unsigned char* sample)
{
	unsigned char c;
	inputLen = 0;

	while ((c = getchar()) != '\n') {
		sample[inputLen++] = c;
	}

}

bool inputText(unsigned char* text, int hsumSample)
{
	for (int i = 0; i < inputLen; i++) {

		int intChar = getchar();
		if (intChar == EOF) {
			printf("%d ", hsumSample);
			return false;
		}
		else {
			text[i] = (unsigned char)intChar;
		}
	}
	return true;
}

// RABIN KARP ////////////////////////////

void printComprassions(unsigned char* sample, unsigned char* text, int ind)
{
	for (int i = 0; i < inputLen; i++) {
		printf("%d ", ind + 1);
		if (sample[i] != text[i]) {
			break;
		}
		++ind;
	}
}

int countHsumSample(unsigned char sample[]) 
{
	int sum = 0;
	for (int i = 0; i < inputLen; i++) {
		sum += ((sample[i]) % 3) * pow(3, i);
	}
	return sum;
}

void rabinKarp(int hsumSample, unsigned char sample[]) 
{
	int factor = pow(3, inputLen - 1);
	int intChar;

	unsigned char text[MAX_INPUT_LENGHT];
	if (inputText(text, hsumSample) == false) return;

	int hsum0 = countHsumSample(text);
	printf("%d ", hsumSample);

	int ind = 0;

	if (hsum0 == hsumSample) {
		printComprassions(sample, text, ind);
	}

	while ((intChar = getchar()) != EOF) {

		int hsum1 = 0;
		ind += 1;

		for (int i = 0; i < inputLen - 1;i++) {
			text[i] = text[i + 1];
		}

		text[inputLen - 1] = (unsigned char)intChar;

		hsum1 = (hsum0 / 3) + (((unsigned char)intChar) % 3) * factor;
		if (hsum1 == hsumSample) {
			ind = ind;
			printComprassions(sample, text, ind);
		}
		
		hsum0 = hsum1;
	}

}
