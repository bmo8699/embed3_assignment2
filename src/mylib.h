unsigned int len(char *string);
void concat(char *string, char a);
int pow(int num, int times);
void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);
unsigned int len(char *string);
void concat(char *string, char a);
int compare(char *string, char *command);
void split(char *string, char** holder);
void empty_string_array(char** string_array);
int atoi(char*str);

#define EOL 10
#define SPACE 32
#define BACKSPACE 8
#define HELP 0
#define SETCOLOR 1
#define CLS 2
#define BRDREV 3
#define SCRSIZE 5
#define DRAW 6
#define CLKRATE 7
#define IFCONFIG 8
#define FIRMREV 9
#define UARTCLK 10
#define INVALID -1