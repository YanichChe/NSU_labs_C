#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_INPUT_LENGHT 18
#define _CRT_SECURE_NO_WARNINGS

int input_len;
int count_hsum_sample(unsigned char sample[]);
int rabin_karp(int hsum_sample, unsigned char sample[]);

int main(void) {
	unsigned char sample[MAX_INPUT_LENGHT];
	unsigned char c;
	input_len = 0;
	int i = 0;
	while ((c = getchar()) != '\n') {
		sample[i] = c;
		++i;
		++input_len;
	}
	int hsum_sample = count_hsum_sample(sample);
	rabin_karp(hsum_sample, sample);
	return EXIT_SUCCESS;
}

int count_hsum_sample(unsigned char sample[]) {
	int sum = 0;
	for (int i = 0; i < input_len; i++) {
		sum += ((sample[i]) % 3) * pow(3, i);
	}
	return sum;
}

int rabin_karp(int hsum_sample, unsigned char sample[]) {
	int factor = pow(3, input_len - 1);
	int hsum0 = 0;
	int int_char;
	unsigned char text[MAX_INPUT_LENGHT];
	for (int i = 0; i < input_len; i++) {
		int_char = getchar();
		if (int_char == EOF) {
			printf("%d ", hsum_sample);
			return 0;
		}
		else {
			text[i] = (unsigned char)int_char;
		}
	}
	hsum0 = count_hsum_sample(text);
	printf("%d ", hsum_sample);
	int ind1 = 0;
	int ind = 0;
	if (hsum0 == hsum_sample) {
		for (int i = 0; i < input_len; i++) {
			printf("%d ", ind1 + 1);
			if (sample[i] != text[i]) {
				break;
			}
			++ind1;
		}
	}
	while ((int_char = getchar()) != EOF) {
		int hsum1 = 0;
		ind += 1;
		for (int i = 0; i < input_len - 1;i++) {
			text[i] = text[i + 1];
		}
		text[input_len - 1] = (unsigned char)int_char;
		hsum1 = (hsum0 / 3) + (((unsigned char)int_char) % 3) * factor;
		if (hsum1 == hsum_sample) {
			ind1 = ind;
			for (int i = 0; i < input_len; i++) {
				printf("%d ", ind1 + 1);
				if (sample[i] != text[i]) {
					break;
				}
				++ind1;
			}
		}
		hsum0 = hsum1;
	}
	return 1;
}
