#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* fptr;
    fptr = fopen("input.txt", "r");

    if (fptr == NULL) {
        printf("file.txt file failed to open.");
    }else {
        char data;
        int dir = 1;
        int move = 0;
        int location = 50;
        int count = 0;
        do{
            data = fgetc(fptr);
            if(data == 'R'){
              dir = 1;
            }else if(data == 'L'){
              dir = -1;
            }else if(data >= 48 && data <= 57){
              move = move*10 + (data - 48);
            }else if(data == '\n'){
              int prev_location = location;
              location += dir*move;
              int quotient = location / 100;
              int remainder = location % 100;
              if((prev_location < 0 && location > 0) || (prev_location > 0 && location < 0)){
                count += 1;
              }
              if(quotient != 0){
                count += abs(quotient);
              }else if(remainder == 0){
                count += 1;
              }
              location = remainder;
              printf("location: %i count: %i \n",location, count);
              move = 0;
            }else if(data == EOF){
              break;
            }else{
              printf("unhandled char \n");
            }
        }while(data != EOF);
        fclose(fptr);
        printf("count %i\n", count);
    }
    return 0;
}
