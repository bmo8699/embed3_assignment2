#include "printf.h"
#include "uart.h"
#include "mylib.h"

#define MAX_PRINT_SIZE 256


void printf(char *string,...) {

	va_list ap;
	va_start(ap, string);

	char buffer[MAX_PRINT_SIZE];
	int buffer_index = 0;

	char temp_buffer[MAX_PRINT_SIZE];

	while(1) {
		if (*string == 0) {
			buffer[buffer_index] = '\0';
			break;
		}

		if (*string == '%') {
			string++;

			if (*string == 'd') {
				string++;
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;

				do {
					temp_buffer[temp_index] = (x % 10) + '0';
					temp_index--;
					x /= 10;
				} while(x != 0);

				for(int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
					buffer[buffer_index] = temp_buffer[i];
					buffer_index++;
				}
			} else if (*string == 'x') {
				//WRITE YOUR CODE HERE FOR HEXA FORMAT
				//Then continue with other formats
				string++;
				int x = va_arg(ap, int);
				unsigned int hex;
				int c;
				int temp_index = MAX_PRINT_SIZE - 1;

				for (c = 28; c >= 0; c = c - 4) {
					// Get highest 4-bit nibble
					hex = (x >> c) & 0xF;
					// 0-9 => '0'-'9', 10-15 => 'A'-'F'
					hex += (hex > 9) ? (- 10 + 'A') : '0';
					temp_buffer[temp_index] = hex;
					temp_index--;
				}

				
				buffer[buffer_index++] = '0';
				buffer[buffer_index++] = 'x';
				for(int i = MAX_PRINT_SIZE - 1; i > temp_index; i--) {
					buffer[buffer_index++] = temp_buffer[i];

				}
			} else if (*string == 'c') {
				string++;
				int x = va_arg(ap, int);
				buffer[buffer_index++] = x;
			} else if (*string == 's') {
				string++;
				char *x = "";
				x = va_arg(ap, char*);
				int temp_index = MAX_PRINT_SIZE - 1;

				do {
					temp_buffer[temp_index] = *x;
					temp_index--;
					x++;
				} while(*x != '\0');

				for(int i = MAX_PRINT_SIZE - 1; i > temp_index; i--) {
					buffer[buffer_index++] = temp_buffer[i];
				}
			} else if (*string == '%') {
				string++;
				buffer[buffer_index++] = '%';
			} else if (*string == 'f') {
				string++;	
				float x = (float) va_arg(ap, double);
				char res[20];
				int temp_index = MAX_PRINT_SIZE - 1;
				ftoa(x, res, 4);
				int i = 0;

				do {
					temp_buffer[temp_index] = res[i];
					temp_index--;
					i++;
				} while(res[i] != '\0');

				uart_puts(temp_buffer);

				for(int i = MAX_PRINT_SIZE - 1; i > temp_index; i--) {
					buffer[buffer_index++] = temp_buffer[i];
				}
			} else if (*string == '.') {
				string++;
				if (*string >= 48 && *string <= 57) {
					int precision = *string - 48;
					string++;
					if (*string == 'f') {
						string++;
						float x = (float) va_arg(ap, double);
						char res[20];
						int temp_index = MAX_PRINT_SIZE - 1;
						ftoa(x, res, precision);
						int i = 0;

						do {
							temp_buffer[temp_index] = res[i];
							temp_index--;
							i++;
						} while(res[i] != '\0');

						uart_puts(temp_buffer);

						for(int i = MAX_PRINT_SIZE - 1; i > temp_index; i--) {
							buffer[buffer_index++] = temp_buffer[i];
						}
						temp_buffer[0] = '\0';
					}
				} 
			} else if (*string >= 48 && *string <= 57) {
				int width = *string - 48;
				string++;
				if (*string == 'f') {
					string++;
					float x = (float) va_arg(ap, double);
					char res[20];
					int temp_index = MAX_PRINT_SIZE - 1;
					ftoa(x, res, 4);
					int i = 0;

					do {
						temp_buffer[temp_index] = res[i];
						temp_index--;
						i++;
					} while(res[i] != '\0');

					for (int i = len(res); i < width; i++) {
						uart_sendc(width);
						buffer[buffer_index++] = ' ';
					}

					for(int i = MAX_PRINT_SIZE - 1; i > temp_index; i--) {
						buffer[buffer_index++] = temp_buffer[i];
					}
				} else if (*string == '.') {
					string++;
					if (*string >= 48 && *string <= 57) {
						int precision = *string - 48;
						string++;
						if (*string == 'f') {
							string++;
							float x = (float) va_arg(ap, double);
							char res[20];
							int temp_index = MAX_PRINT_SIZE - 1;
							ftoa(x, res, precision);
							int i = 0;

							do {
								temp_buffer[temp_index] = res[i];
								temp_index--;
								i++;
							} while(res[i] != '\0');

							for (int i = len(res); i < width; i++) {
								uart_sendc(width);
								buffer[buffer_index++] = ' ';
							}

							for(int i = MAX_PRINT_SIZE - 1; i > temp_index; i--) {
								buffer[buffer_index++] = temp_buffer[i];
							}
						}
					} 
				}
			}
		}
		else {
			buffer[buffer_index] = *string;
			buffer_index++;
			string++;
		}

		if (buffer_index == MAX_PRINT_SIZE - 1) {
			break;
		}
	}

	va_end(ap);


	//Print out formated string
	uart_puts(buffer);
}
