#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#define EOL 10
#define SPACE 32
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

void display_help(int command) {
  if (command == HELP) {
    uart_puts("\nFor more information on a specific command, type help <command-name>");    
    uart_puts("\nsetcolor   Set text color, and/or background color of the console to one of the following color: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE");
    uart_puts("\ncls        Clear screen");
    uart_puts("\nbrdrev     Show board revision");
    uart_puts("\nscrsize    Set screen size");
    uart_puts("\ndraw       Draw something colorful");
    uart_puts("\nclkrate    Get clock rate");
    uart_puts("\nifconfig   Get MAC address");
    uart_puts("\nfirmrev    Get firm revision");
    uart_puts("\nuartclk    Get uart clock state");
  } else if (command == SETCOLOR) {
    uart_puts("\nsetcolor -t <text color> -b <background color>");
    uart_puts("\nSet text color, and/or background color of the console to one of the following color: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
  } else if (command == CLS) {
    uart_puts("\ncls");
    uart_puts("\nClear screen\n");
  } else if (command == BRDREV) {
    uart_puts("\nbrdrev");
    uart_puts("\nShow board revision\n");
  } else if (command == SCRSIZE) {
    uart_puts("\nscrsize -p <physical width> <physical height> -v <virtual width> <virtual height>");
    uart_puts("\nSet screen size");
    uart_puts("\nMust have options to set physical screen size (-p) or virtual screen size (-v), or both (by default)\n");
  } else if (command == DRAW) {
    uart_puts("draw");
    uart_puts("Draw blocks of colorful squares");
  } else if (command == CLKRATE){
    uart_puts("clkrate");
    uart_puts("Get the board clock rate");
  } else if (command == IFCONFIG) {
    uart_puts("mac");
    uart_puts("Get MAC address");
  } else if (command == FIRMREV) {
    uart_puts("firmrev");
    uart_puts("Get firm revision");
  } else if (command == UARTCLK) {
    uart_puts("uartclk");
    uart_puts("Get uart clock state, either ON or OFF");
  } else{
    uart_puts("\nCommand not found");
    uart_puts("\nhelp <command-name> to displays help information on that command");
  }
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

void display_banner() {
  uart_puts("######## ######## ######## ########  #######  ##         #######    #####\n");
  uart_puts("##       ##       ##          ##    ##     ## ##    ##  ##     ##  ##   ##\n");
  uart_puts("##       ##       ##          ##           ## ##    ##  ##     ## ##     ##\n");
  uart_puts("######   ######   ######      ##     #######  ##    ##   ######## ##     ##\n");
  uart_puts("##       ##       ##          ##    ##        #########        ## ##     ##\n");
  uart_puts("##       ##       ##          ##    ##              ##  ##     ##  ##   ##\n");
  uart_puts("######## ######## ########    ##    #######         ##   #######    #####\n \n");

  uart_puts("########     ###    ########  ########           #######   ###### \n");
  uart_puts("##     ##   ## ##   ##     ## ##                ##     ## ##    ##\n");
  uart_puts("##     ##  ##   ##  ##     ## ##                ##     ## ## \n");
  uart_puts("########  ##     ## ########  ######            ##     ##  ######\n");
  uart_puts("##     ## ######### ##   ##   ##                ##     ##       ## \n");
  uart_puts("##     ## ##     ## ##    ##  ##                ##     ## ##    ##\n");
  uart_puts("########  ##     ## ##     ## ########           #######   ######\n");
}

void set_text_color(char* color) {
  if (compare(color, "black")) {
    uart_puts("\x1b[30m");
  } else if (compare(color, "red")) {
    uart_puts("\x1b[34m");
  } else if (compare(color, "green")) {
    uart_puts("\x1b[32m");
  } else if (compare(color, "yellow")) {
    uart_puts("\x1b[33m");
  } else if (compare(color, "blue")) {
    uart_puts("\x1b[34m");
  } else if (compare(color, "purple")) {
    uart_puts("\x1b[35m");
  } else if (compare(color, "cyan")) {
    uart_puts("\x1b[36m");
  } else if (compare(color, "white")) {
    uart_puts("\x1b[37m");
  } else {
    uart_puts("\nInvalid color input!");
    return;
  }
  uart_puts("\nText color set to ");
  uart_puts(color);
  uart_puts("\n");
}

void set_background_color(char* color) {
  if (compare(color, "black")) {
    uart_puts("\033[40m");
  } else if (compare(color, "red")) {
    uart_puts("\033[41m");
  } else if (compare(color, "green")) {
    uart_puts("\033[42m");
  } else if (compare(color, "yellow")) {
    uart_puts("\033[43m");
  } else if (compare(color, "blue")) {
    uart_puts("\033[44m");
  } else if (compare(color, "purple")) {
    uart_puts("\033[45m");
  } else if (compare(color, "cyan")) {
    uart_puts("\033[46m");
  } else if (compare(color, "white")) {
    uart_puts("\033[47m");
  } else {
    uart_puts("\nInvalid color input!");
    return;
  }
  uart_puts("\nBackground color set to ");
  uart_puts(color);
  uart_puts("\n");
}


void handle_help(char**splitted_string) {
  if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
      display_help(HELP);
  } else {
    if (splitted_string[2] == 0) {
      if (compare(splitted_string[1],"setcolor")) {
        display_help(SETCOLOR);
      } else if (compare(splitted_string[1],"cls"))  {
        display_help(CLS);
      } else if (compare(splitted_string[1],"brdrev")) {
        display_help(BRDREV);
      } else if (compare(splitted_string[1],"scrsize")) {
        display_help(SCRSIZE);
      } else {
        display_help(INVALID);  
      }
    } else {
      display_help(INVALID);
    }
  }
}



void handle_setcolor(char**splitted_string) {
  if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    display_help(INVALID);
  } else {
    if (compare(splitted_string[1],"-t")) {
      if (splitted_string[2] == 0 || *splitted_string[2] == '\0') {
        display_help(INVALID);
      } else {
        if (splitted_string[3] == 0 || *splitted_string[3] == '\0') {
          set_text_color(splitted_string[2]);
        } else if (compare(splitted_string[3],"-b")) {
          if (splitted_string[4] == 0 || *splitted_string[4] == '\0') {
            display_help(INVALID);
          } else {
            if (splitted_string[5] == 0 || *splitted_string[5] == '\0') {
              set_text_color(splitted_string[2]);
              set_background_color(splitted_string[4]);
            } 
          }
        } else {
          display_help(INVALID);
        }
      }
    } else if (compare(splitted_string[1],"-b")) {
      if (splitted_string[2] == 0 || *splitted_string[2] == '\0') {
        display_help(INVALID);
      } else {
        if (splitted_string[3] == 0 || *splitted_string[3] == '\0') {
          set_background_color(splitted_string[2]);
        } else if (compare(splitted_string[3],"-t")) {
          if (splitted_string[4] == 0 || *splitted_string[4] == '\0') {
            display_help(INVALID);
          } else {
            if (splitted_string[5] == 0 || *splitted_string[5] == '\0') {
              set_text_color(splitted_string[4]);
              set_background_color(splitted_string[2]);
            } 
          }
        } else {
          display_help(INVALID);
        }
      }
    } else {
      display_help(INVALID);
    }
  } 
}

void handle_cls(char**splitted_string) {
  if (splitted_string[1] == 0 || *splitted_string[1] == '\0') { 
    uart_puts("\e[1;1H\e[2J");
  } else {
    display_help(INVALID);
  }
} 

void handle_brdrev(char**splitted_string) {
  if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    mbox[0] = 7*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mbox[2] = 0x00010002; // TAG Identifier: Get clock rate
    mbox[3] = 4; // Value buffer size in bytes (max of request and response lengths)
    mbox[4] = 0; // REQUEST CODE = 0
    mbox[5] = 0; // clear output buffer (response data are mbox[5])
    mbox[6] = MBOX_TAG_LAST;  

    if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
      uart_puts("\nResponse Code for whole message: ");
      uart_hex(mbox[1]);
      uart_puts("\n");
      uart_puts("Response Code in Message TAG: ");
      uart_hex(mbox[4]);
      uart_puts("\n");
      uart_puts("DATA: board revision = ");
      uart_dec(mbox[5]);
      uart_puts("\n");
    } else {
      uart_puts("Unable to query!\n");
    }
  } else {
    display_help(INVALID);
  }
}

int atoi(char*str) {
  int res = 0;

  for (int i = 0; str[i] != '\0'; ++i) {
     if (str[i]> '9' || str[i]<'0') {
        uart_puts("Invalid number input");
        return -1;
     }
     res = res*10 + str[i] - '0';
  }

 // return result.
 return res;
}

void handle_scrsize(char** splitted_string) {
  if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    display_help(INVALID);
  } else {
    if (compare(splitted_string[1],"-p")) {
      if ((splitted_string[2] == 0 || *splitted_string[2] == '\0') && (splitted_string[3] == 0 || *splitted_string[3] == '\0')) {
        display_help(INVALID);
      } else {
        if (splitted_string[4] == 0 || *splitted_string[4] == '\0') {
          if (atoi(splitted_string[2]) > 0 && atoi(splitted_string[3]) > 0) {
            set_physical_screen(atoi(splitted_string[2]),atoi(splitted_string[3]));
          } 
        } else if (compare(splitted_string[4],"-v")) {
          if ((splitted_string[5] == 0 || *splitted_string[5] == '\0') && (splitted_string[6] == 0 || *splitted_string[6] == '\0')) {
            display_help(INVALID);
          } else {
            if (splitted_string[7] == 0 || *splitted_string[7] == '\0') {
              if (atoi(splitted_string[2]) >= 0 && atoi(splitted_string[3]) >= 0 && atoi(splitted_string[5]) >= 0 && atoi(splitted_string[6]) >= 0) {
                framebf_init(atoi(splitted_string[5]),atoi(splitted_string[6]));
              }
            } 
          }
        } else {
          display_help(INVALID);
        }
      }
    } else if (compare(splitted_string[1],"-v")) {
      if ((splitted_string[2] == 0 || *splitted_string[2] == '\0') && (splitted_string[3] == 0 || *splitted_string[3] == '\0')) {
        display_help(INVALID);
      } else {
        if (splitted_string[4] == 0 || *splitted_string[4] == '\0') {
          if (atoi(splitted_string[2]) >= 0 && atoi(splitted_string[3]) >= 0) {
            set_virtual_screen(atoi(splitted_string[2]),atoi(splitted_string[3]));
          }
        } else if (compare(splitted_string[4],"-p")) {
          if ((splitted_string[5] == 0 || *splitted_string[5] == '\0') && (splitted_string[6] == 0 || *splitted_string[6] == '\0')) {
            display_help(INVALID);
          } else {
            if (splitted_string[7] == 0 || *splitted_string[7] == '\0') {
              if (atoi(splitted_string[2]) >= 0 && atoi(splitted_string[3]) >= 0 && atoi(splitted_string[5]) >= 0 && atoi(splitted_string[6]) >= 0) {
                framebf_init(atoi(splitted_string[5]),atoi(splitted_string[6]));
              }
            } 
          }
        } else {
          display_help(INVALID);
        }
      }
    } else {
      display_help(INVALID);
    }
  } 
}

void handle_clkrate(char**splitted_string) {
   if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    mbox[0] = 8*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mbox[2] = 0x00030002; // TAG Identifier: Get clock rate
    mbox[3] = 8; // Value buffer size in bytes (max of request and response lengths)
    mbox[4] = 0; // REQUEST CODE = 0
    mbox[5] = 3; // clock id: ARM system clock
    mbox[6] = 0; // clear output buffer (response data are mbox[5] & mbox[6])
    mbox[7] = MBOX_TAG_LAST;  

    if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
      uart_puts("\nResponse Code for whole message: ");
      uart_hex(mbox[1]);
      uart_puts("\n");
      uart_puts("Response Code in Message TAG: ");
      uart_hex(mbox[4]);
      uart_puts("\n");
      uart_puts("DATA: ARM clock rate = ");
      uart_dec(mbox[6]);
      uart_puts("\n");
    } else {
      uart_puts("Unable to query!\n");
    }
  }
}

void handle_draw(char** splitted_string) {
  if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    uart_puts("\nDrawing...");
    drawRectARGB32(100,100,400,400,0x00AA0000,1); //RED
    drawRectARGB32(150,150,400,400,0x0000BB00,1); //GREEN
    drawRectARGB32(200,200,400,400,0x000000CC,1); //BLUE
    drawRectARGB32(250,250,400,400,0x00FFFF00,1); //YELLOW
    drawPixelARGB32(300, 300, 0x00FF0000); 
  } else {
    display_help(INVALID);
  }
}

void handle_ifconfig(char** splitted_string) {
   if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    mbox[0] = 7*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mbox[2] = 0x00010003; // TAG Identifier: Get clock rate
    mbox[3] = 6; // Value buffer size in bytes (max of request and response lengths)
    mbox[4] = 0; // REQUEST CODE = 0
    mbox[5] = 0; // clear output buffer (response data are mbox[5])
    mbox[6] = MBOX_TAG_LAST;  

    if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
      uart_puts("\nResponse Code for whole message: ");
      uart_hex(mbox[1]);
      uart_puts("\n");
      uart_puts("Response Code in Message TAG: ");
      uart_hex(mbox[4]);
      uart_puts("\n");
      uart_puts("DATA: MAC address = ");
      uart_hex(mbox[5]);
      uart_puts("\n");
    } else {
      uart_puts("Unable to query!\n");
    }
  } else {
    display_help(INVALID);
  }
}

void handle_firmrev(char**splitted_string) {
   if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    mbox[0] = 7*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mbox[2] = 0x00000001; // TAG Identifier: Get clock rate
    mbox[3] = 4; // Value buffer size in bytes (max of request and response lengths)
    mbox[4] = 0; // REQUEST CODE = 0
    mbox[5] = 0; // clear output buffer (response data are mbox[5])
    mbox[6] = MBOX_TAG_LAST;  

    if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
      uart_puts("\nResponse Code for whole message: ");
      uart_hex(mbox[1]);
      uart_puts("\n");
      uart_puts("Response Code in Message TAG: ");
      uart_hex(mbox[4]);
      uart_puts("\n");
      uart_puts("DATA: Firmware revision = ");
      uart_dec(mbox[5]);
      uart_puts("\n");
    } else {
      uart_puts("Unable to query!\n");
    }
  } else {
    display_help(INVALID);
  }
}

void handle_uartclk(char** splitted_string) {
   if (splitted_string[1] == 0 || *splitted_string[1] == '\0') {
    mbox[0] = 8*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mbox[2] = 0x00030001; // TAG Identifier: Get clock rate
    mbox[3] = 8; // Value buffer size in bytes (max of request and response lengths)
    mbox[4] = 0; // REQUEST CODE = 0
    mbox[5] = 2; //UART Clock
    mbox[6] = 0; // clear output buffer (response data are mbox[5] & mbox[6])
    mbox[7] = MBOX_TAG_LAST;  

    if (mbox_call(ADDR(mbox), MBOX_CH_PROP)) {
      uart_puts("\nResponse Code for whole message: ");
      uart_hex(mbox[1]);
      uart_puts("\n");
      uart_puts("Response Code in Message TAG: ");
      uart_hex(mbox[4]);
      uart_puts("\n");
      uart_puts("UART Clock is ");
      if (mbox[6] % 2 == 0) {
        uart_puts("OFF"); //Bit 0: 0=off, 1=on
      } else {
        uart_puts("ON");
      }
      uart_puts("\n");
    } else {
      uart_puts("Unable to query!\n");
    }
  } else {
    display_help(INVALID);
  }
}


void main() {
  // set up serial console
  uart_init();

  char string[50] = "";
  int space_flag = 1;
  char *splitted_string[10];

  framebf_init(1024,768);
  display_banner();
  uart_puts("\n~/");
  // echo everything back
  while (1) {
    //read each char
    char c = uart_getc(); 
    if (c > 33 && c < 126) {
      concat(string, c);
      space_flag = 0;
    } else if (c == EOL) {
      split(string, splitted_string);
      if (compare(splitted_string[0],"help")) {
        handle_help(splitted_string);
      } else if (compare(splitted_string[0], "setcolor")) {
        handle_setcolor(splitted_string);
      } else if (compare(splitted_string[0],"cls")) {
        handle_cls(splitted_string);
      } else if (compare(splitted_string[0],"brdrev")) {
        handle_brdrev(splitted_string);
      } else if (compare(splitted_string[0],"scrsize")) {
        handle_scrsize(splitted_string);
      } else if (compare(splitted_string[0],"draw")) {
        handle_draw(splitted_string);
      } else if (compare(splitted_string[0],"clkrate")) {
        handle_clkrate(splitted_string);
      } else if (compare(splitted_string[0],"ifconfig")) {
        handle_ifconfig(splitted_string);
      } else if (compare(splitted_string[0],"firmrev")) {
        handle_firmrev(splitted_string);
      } else if (compare(splitted_string[0],"uartclk")) {
        handle_uartclk(splitted_string);
      } else {
        display_help(INVALID);
      }
      space_flag = 1;
      empty_string_array(splitted_string); 
      string[0] = '\0';
      uart_puts("\n~/");
    } else if (c == SPACE) {
      if (space_flag == 0) {
        concat(string, c);
        space_flag = 1;
      }
    }
    if (c != EOL) {
      uart_sendc(c);
    }
  }
}
