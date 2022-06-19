#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//////////////////////////////////////////

#define MAX_INPUT_LENGHT 18
#define MAX_SAMPLE_LENGHT 256
#define _CRT_SECURE_NO_WARNINGS 

//////////////////////////////////////////

void inputData(int* inputLen, unsigned char* data);
bool fillText(int from, int to, unsigned char* text);

int createSample(int inputLen);
void fillSample(int inputLen, unsigned char data[], int* sample);
void boyerMoore(int inputLen, unsigned char data[], int sample[]);

// MAIN //////////////////////////////////

int main(void) 
{
	int inputLen = 0;
	
	unsigned char data[MAX_INPUT_LENGHT];
	inputData(&inputLen, data);

	int sample = createSample(inputLen);

	fillSample(inputLen, data, sample);
	boyerMoore(inputLen, data, sample);

	return EXIT_SUCCESS;
}

// INPUT /////////////////////////////////

void inputData(int* inputLen, unsigned char* data)
{
	int intChar;
	int i = 0;

	while ((intChar = getchar()) != '\n') {
		data[i] = (unsigned char)intChar;
		++i;
		++(*inputLen);
	}
}

int createSample(int inputLen)
{
	int sample[MAX_SAMPLE_LENGHT];

	for (int i = 0; i < MAX_SAMPLE_LENGHT; i++) {
		sample[i] = inputLen;
	}

	return sample;
}

// FILLING ///////////////////////////////

void fillSample(int inputLen, unsigned char data[], int* sample) 
{
	for (int i = inputLen - 2; i >= 0; i--) {
		if (sample[data[i]] == inputLen) {
			sample[data[i]] = inputLen - i - 1;
		}
	}
}

bool fillText(int from, int to, unsigned char* text)
{
	int intChar;

	for (int i = from; i < to; i++) {
		if ((intChar = getchar()) != EOF) {
			text[i] = (unsigned char)intChar;
		}
		else {
			return false;
		}
	}

	return true;
}

// Boyer Moore ///////////////////////////

void boyerMoore(int inputLen, unsigned char data[], int sample[]) 
{
	unsigned char text[MAX_INPUT_LENGHT] = { '0' };
	int intChar;

	if (fillText(0, inputLen, text) == false) return;

	int currentPosition = 0;
	int comparisonPosition = inputLen - 1;
	unsigned char lastChar = text[comparisonPosition];

	while (1) {
		printf("%d ", comparisonPosition + currentPosition + 1);

		if (text[comparisonPosition] != data[comparisonPosition] || comparisonPosition == 0) {
			currentPosition += sample[lastChar];

			for (int i = 0; i < inputLen - sample[lastChar]; i++) {
				text[i] = text[i + sample[lastChar]];
			}

			if (fillText(inputLen - sample[lastChar], inputLen, text) == false) return;
			
			comparisonPosition = inputLen - 1;
			lastChar = text[comparisonPosition];
		}

		else comparisonPosition--;
	}
	return;
}

