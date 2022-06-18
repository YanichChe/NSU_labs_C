#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_INPUT_LENGHT 18
#define MAX_SAMPLE_LENGHT 256
#define _CRT_SECURE_NO_WARNINGS 

void make_sample(int input_len, unsigned char data[], int* sample);
void search(int input_len, unsigned char data[], int sample[]);
void input_data(int* input_len, unsigned char* data);
bool fill_text(int from, int to, unsigned char* text);

int main(void) 
{
	int input_len = 0;
	
	unsigned char data[MAX_INPUT_LENGHT];
	input_data(&input_len, data);

	int sample[MAX_SAMPLE_LENGHT];

	for (int i = 0; i < MAX_SAMPLE_LENGHT; i++) {
		sample[i] = input_len;
	}

	make_sample(input_len, data, sample);
	search(input_len, data, sample);

	return EXIT_SUCCESS;
}

void input_data(int* input_len, unsigned char* data)
{
	int int_char;
	int i = 0;

	while ((int_char = getchar()) != '\n') {
		data[i] = (unsigned char)int_char;
		++i;
		++(*input_len);
	}
}

void make_sample(int input_len, unsigned char data[], int* sample) 
{
	for (int i = input_len - 2; i >= 0; i--) {
		if (sample[data[i]] == input_len) {
			sample[data[i]] = input_len - i - 1;
		}
	}
}

bool fill_text(int from, int to, unsigned char* text)
{
	int int_char;

	for (int i = from; i < to; i++) {
		if ((int_char = getchar()) != EOF) {
			text[i] = (unsigned char)int_char;
		}
		else {
			return false;
		}
	}

	return true;
}

void search(int input_len, unsigned char data[], int sample[]) 
{
	unsigned char text[MAX_INPUT_LENGHT] = { '0' };
	int int_char;

	if (fill_text(0, input_len, text) == false){
		return;
	}

	int i = 0;
	int j = input_len - 1;
	unsigned char last_char = text[j];

	while (1) {
		printf("%d ", j + i + 1);

		if (text[j] != data[j] || j == 0) {
			i += sample[last_char];

			for (int k = 0; k < input_len - sample[last_char]; k++) {
				text[k] = text[k + sample[last_char]];
			}

			if (fill_text(input_len - sample[last_char], input_len, text) == false){
				return;
			}

			j = input_len - 1;
			last_char = text[j];
		}

		else {
			j--;
		}
	}
	return;
}

