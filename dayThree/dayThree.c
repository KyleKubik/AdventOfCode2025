#include <stdio.h>
#include <stdlib.h>

#define joltage_size 12

int main(int argc, char **argv) {
  if(argc < 2){
    printf("please specify a filename.\n");
    return -1;
  }
  char* filename = argv[1];
  
  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }

  int joltage_parts[joltage_size] = {0};
  unsigned long long total_joltage = 0;
  
  int data = 0;
  do{
    data = fgetc(fptr); //read one character from file
    if(data >= 48 && data <= 57){ //ASCII characters 48 to 57 are numerals 0 to 9.
      for(int i = 1; i < joltage_size; i++){
        if(joltage_parts[i-1] < joltage_parts[i]){
          joltage_parts[i-1] = joltage_parts[i];
          joltage_parts[i] = 0;
        }
      }
      int val = data - 48;
      if(val > joltage_parts[joltage_size-1]){
        joltage_parts[joltage_size-1] = val;
      }
    }else if(data == '\n'){
      unsigned long long joltage = 0;
      for(int i = 0; i < joltage_size; i++){
        joltage = joltage*10 + joltage_parts[i];
        joltage_parts[i] = 0;
      }
      printf("joltage %llu\n", joltage);
      total_joltage += joltage;
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  //print our final output
  printf("total joltage %llu\n", total_joltage);
  
  return 0;
}
