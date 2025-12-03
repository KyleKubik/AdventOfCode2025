#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int max(int a, int b) {
  return (a > b) ? a : b;
}

int min(int a, int b) {
  return (a < b) ? a : b;
}

unsigned long long int_pow(int base, int exp) {
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

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
  
  bool first = true;
  unsigned long long min_val = 0;
  unsigned long long max_val = 0;
  unsigned long long total = 0;
  unsigned long long data = 0;
  
  do{
    bool calculate = false;
    data = fgetc(fptr); //read one character from file
    if(data >= 48 && data <= 57){ //ASCII characters 48 to 57 are numerals 0 to 9.
      if(first){
        min_val = min_val*10 + (data - 48); //build our number from the provided characters
      }else{
        max_val = max_val*10 + (data - 48);
      }
    }else if(data == '-'){
      first = false;
    }else if(data == ',' || data == '\n'){
      calculate = true;
      first = true;
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
   
    if(calculate){
      unsigned long long min_order = (floor(log10(min_val))+1)/2;
      unsigned long long max_order = (floor(log10(max_val))+1)/2;
      printf("values: %llu - %llu\n",min_val,max_val);
      printf("orders: %llu - %llu\n",min_order,max_order);
      
      for(long long i = min_order; i <= max_order; i++){
        printf("   order: %llu\n", i );
        unsigned long long  pattern = int_pow(10,i)+1 ;
        unsigned long long  min_possible = int_pow(10,i-1);
        unsigned long long  max_possible = int_pow(10,i)-1;
        for(unsigned long long j = min_possible; j <= max_possible; j++){
          if(min_val <= j*pattern && j*pattern <= max_val){
            printf("bad value: %llu\n", j*pattern );
            total += j*pattern;
          }
        }
      }
      
      min_val = 0;
      max_val = 0;
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  //print our final output
  printf("total %llu\n", total);
  
  return 0;
}
