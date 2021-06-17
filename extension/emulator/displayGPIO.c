
#include "displayGPIO.h"

void displayGPIO(uint32_t address) {
    
//GPIO 0-9 have indexes 2 5 7 21 
//GPIO 10-19 have indexes 11 19 23 8 10 12
//GPIO 20-29 have indexes 13 15 16 18 22

  switch(address) {
      case (0x20200000):
        //GPIO 0-9
        printf("\n1  3  " ANSI_COLOR_GREEN "5  7"  ANSI_COLOR_RESET "  9  11 13 15 17 19" ANSI_COLOR_GREEN " 21" ANSI_COLOR_RESET " 23 25\n");
        printf("o  o  " ANSI_COLOR_GREEN "o  o"  ANSI_COLOR_RESET "  o  o  o  o  o  o" ANSI_COLOR_GREEN "  o"  ANSI_COLOR_RESET "  o  o\n");
        printf(ANSI_COLOR_GREEN "o" ANSI_COLOR_RESET  "  o  o  o  o  o  o  o  o  o  o  o  o\n");
        printf(ANSI_COLOR_GREEN "2" ANSI_COLOR_RESET  "  4  6  8  10 12 14 16 18 20 22 24 26\n\n");
        break;
      case (0x20200004):
        //GPIO 10-19
        printf("\n1  3  5  7  9" ANSI_COLOR_GREEN "  11" ANSI_COLOR_RESET " 13 15 17" ANSI_COLOR_GREEN " 19" ANSI_COLOR_RESET " 21" ANSI_COLOR_GREEN " 23" ANSI_COLOR_RESET " 25\n");
        printf("o  o  o  o  o" ANSI_COLOR_GREEN "  o" ANSI_COLOR_RESET "  o  o  o" ANSI_COLOR_GREEN "  o" ANSI_COLOR_RESET "  o" ANSI_COLOR_GREEN "  o"  ANSI_COLOR_RESET "  o\n");
        printf("o  o  o" ANSI_COLOR_GREEN "  o  o  o" ANSI_COLOR_RESET "  o  o  o  o  o  o  o\n");
        printf("2  4  6" ANSI_COLOR_GREEN "  8  10 12" ANSI_COLOR_RESET " 14 16 18 20 22 24 26\n\n");
        break;
      case (0x20200008):
        printf("\n1  3  5  7  9  11" ANSI_COLOR_GREEN " 13 15" ANSI_COLOR_RESET " 17 19 21 23 25\n");
        printf("o  o  o  o  o  o" ANSI_COLOR_GREEN "  o  o" ANSI_COLOR_RESET  "  o  o  o  o  o\n");
        printf("o  o  o  o  o  o  o" ANSI_COLOR_GREEN "  o  o" ANSI_COLOR_RESET "  o" ANSI_COLOR_GREEN "  o" ANSI_COLOR_RESET "  o  o\n");
        printf("2  4  6  8  10 12 14" ANSI_COLOR_GREEN " 16 18" ANSI_COLOR_RESET " 20" ANSI_COLOR_GREEN " 22" ANSI_COLOR_RESET " 24 26\n\n");
        break;
      default:
        //address does not access anything
        break;  
  }
}