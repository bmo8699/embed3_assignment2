#include "mylib.h"

int pow(int num, int times)  {
    int res = 1;
    for (int i =0; i < times; i++) {
        res *= num;
    }
    
    return res;
}

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverse(str, i);
    str[i] = '\0';
    return i;
}

void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
  
    // Extract floating part
    float fpart = n - (float)ipart;
  
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
  
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
  
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}


unsigned int len(char *string) {
  unsigned int i = 0;
  while (*string++ != '\0')
      ++i;
  return i;
}

void concat(char *string, char a) {
  unsigned int string_len = len(string);
  string[string_len] = a;
  string[string_len+1] = '\0';
}

int compare(char *string, char *command) {
  for (int i=0; i < len(command); i++) {
    if (string[i] != command[i]) {
      return 0;
    } 
  }

  return 1;
}

void split(char *string, char** holder) {
    char *temp = string, *c = string;
    holder[0] = temp;
    int count = 1;
    while (*c) {
        if (*c == SPACE) {
            *c = '\0'; 
            temp = ++c;
            holder[count] = temp;
            count++;
        } else c++;
    }   
}

void empty_string_array(char** string_array) {
  int count = 0; 
  while (string_array[count] != 0) {
    string_array[count] = 0;
    count++;
  }
}

int atoi(char*str) {
  int res = 0;

  for (int i = 0; str[i] != '\0'; ++i) {
     if (str[i]> '9' || str[i]<'0') {
        return -1;
     }
     res = res*10 + str[i] - '0';
  }

 // return result.
 return res;
}

