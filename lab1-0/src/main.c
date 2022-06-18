#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_INPUT_LENGHT 18
#define MAX_SAMPLE_LENGHT 256
#define _CRT_SECURE_NO_WARNINGS 
void make_sample(unsigned char s[], int* sample);
int search(unsigned char s[], int sample[]);
int input_len;

int main(void) {
	unsigned char s[MAX_INPUT_LENGHT];
	int int_char;
	input_len = 0;
	int i = 0;
	while ((int_char = getchar()) != '\n') {
		s[i] = (unsigned char)int_char;
		++i;
		++input_len;
	}
	int sample[MAX_SAMPLE_LENGHT];
	for (i = 0; i < MAX_SAMPLE_LENGHT; i++) {
		sample[i] = input_len;
	}

	make_sample(s, sample);
	search(s, sample);
	return EXIT_SUCCESS;
}

void make_sample(unsigned char s[], int* sample) {
	for (int i = input_len - 2; i >= 0; i--) {
		if (sample[s[i]] == input_len) {
			sample[s[i]] = input_len - i - 1;
		}
	}

}
int search(unsigned char s[], int sample[]) {
	int i;
	int j;
	unsigned char last_char;
	unsigned char text[MAX_INPUT_LENGHT] = { '0' };
	int int_char;
	for (i = 0;i < input_len;i++) {
		if ((int_char = getchar()) != EOF) {
			text[i] = (unsigned char)int_char;
		}
		else {
			return 0;
		}
	}
	i = 0;
	j = input_len - 1;
	last_char = text[j];//запоминаем крайний символ 
	while (1) {
		printf("%d ", j + i + 1);// пишем индекс символа, который сравниваем 
		if (text[j] != s[j] || j == 0) {//если символы не совпали или уже нашли полное совпадение 
			i += sample[last_char];//делаем шаг, который указан в таблице соответствий 
			for (int q = 0; q < input_len - sample[last_char];q++) {
				text[q] = text[q + sample[last_char]];
			}
			for (int q = input_len - sample[last_char]; q < input_len;q++) {
				if ((int_char = getchar()) != EOF) {
					text[q] = (unsigned char)int_char;
				}
				else {
					return 0;
				}
			}
			j = input_len - 1;
			last_char = text[j];
		}
		else {//если совпало то проверяем предыдущий символ 
			j--;
		}
	}
	return 1;
}

