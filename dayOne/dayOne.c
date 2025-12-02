#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if(argc < 3){
    printf("please specify a filename and password method. (try 0x434C49434B for part 2)\n");
    return -1;
  }
  char* filename = argv[1];
  long password_method = strtol(argv[2], NULL, 16);
  
  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  
  int location = 50;  //dial location
  int dir = 1;        //movement direction
  int move = 0;       //movement amount
  int count = 0;      //password count
  char data;
  do{
    data = fgetc(fptr); //read one character from file
    if(data == 'R'){
      dir = 1; //Right means positive movement
    }else if(data == 'L'){
      dir = -1; //Left means negative movements
    }else if(data >= 48 && data <= 57){ //ASCII characters 48 to 57 are numerals 0 to 9.
      move = move*10 + (data - 48); //build our number of moves from the provided characters
    }else if(data == '\n'){
      int prev_location = location;
      location += dir*move;
      int quotient = location / 100;
      int remainder = location % 100;
      if(password_method == 0x434C49434B){ //Special method for part 2
        if((prev_location < 0 && location > 0) || (prev_location > 0 && location < 0)){
          count += 1; //increment for zero crossings
        }
        if(quotient != 0){
          count += abs(quotient); // increment for every 100 additional clicks
        }else if(remainder == 0){
          count += 1; //increment if we land on 0
        }
      }else{ //Regular method for part 1
        if(remainder == 0){
          count += 1; //increment if we land on 0
        }
      }
      location = remainder; //set the location back to the remainder
      move = 0; //reset our move counter (to build a new number)
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  //print our final output
  printf("count %i\n", count);
      
  return 0;
}
