#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long ull_t;

typedef struct {
  ull_t* data;
  unsigned int size;
  unsigned int capacity;
}Vector;

void vector_append(Vector* vec, ull_t data){
  if(vec->size == vec->capacity){
    if(vec->capacity == 0){
      vec->data = malloc(16 * sizeof(ull_t));
      if(vec->data == NULL) {
          printf("data Memory allocation failed!\n");
      }
      vec->capacity = 16;
    }else{
      unsigned int new_capacity = vec->capacity*2;
      ull_t* new_data = malloc(new_capacity * sizeof(ull_t));
      if(new_data == NULL) {
          printf("new_data Memory allocation failed!\n");
      }
      memcpy(new_data, vec->data, vec->capacity * sizeof(ull_t));
      free(vec->data);
      vec->data = new_data;
      vec->capacity = new_capacity;
    }
  }
  vec->data[vec->size] = data;
  vec->size = vec->size + 1;
};

void vector_free(Vector* vec){
  free(vec->data);
  vec->data = NULL;
  vec->size = 0;
  vec->capacity = 0;
}

ull_t int_pow(int base, int exp) {
    ull_t result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

ull_t make_cephalopod_number_three(ull_t one,ull_t two,ull_t three,int order){
  return (( one / int_pow(10,order) ) % 10)*100 + 
         (( two / int_pow(10,order) ) % 10)*10 + 
         (( three / int_pow(10,order) ) % 10)*1;
  
  int digit_one = (( one / int_pow(10,order) ) % 10);
  int digit_two = (( two / int_pow(10,order) ) % 10);
  int digit_three = (( three / int_pow(10,order) ) % 10);
}
ull_t make_cephalopod_number(ull_t one,ull_t two,ull_t three,ull_t four,int order){
  return (( one / int_pow(10,order) ) % 10)*1000 + 
         (( two / int_pow(10,order) ) % 10)*100 + 
         (( three/ int_pow(10,order) ) % 10)*10 + 
         (( four / int_pow(10,order) ) % 10)*1 ;
}

int main(int argc, char **argv) {
  if(argc < 3){
    printf("please specify a filename and equation size.\n");
    return -1;
  }
  char* filename = argv[1];
  int equation_size = atoi(argv[2]);
  
  int data = 0;
  int line_number = 0;
  ull_t number = 0;
  Vector numbers_first = {0};
  Vector numbers_second = {0};
  Vector numbers_third = {0};
  Vector numbers_fourth = {0};
  Vector operators = {0};

  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  do{
    data = fgetc(fptr); //read one character from file
    if(data >= 48 && data <= 57){ //ASCII characters 48 to 57 are numerals 0 to 9.
      number = (number*10) + (data - 48);
    }else if(data == ' ' || data == '\n'){
      if(number != 0){
        if(line_number < equation_size){
          switch(line_number){
            case 0: vector_append(&numbers_first,number); break; 
            case 1: vector_append(&numbers_second,number); break; 
            case 2: vector_append(&numbers_third,number); break;
            case 3: vector_append(&numbers_fourth,number); break;
            case 4: printf("parse error: number on wrong line!\n"); break; 
            default: printf("parse error: unknown line number!\n"); break;
          }
        }
        number = 0;
      }
      if(data == '\n'){
        line_number += 1;
      }
    }else if(data=='+'){
      if(line_number == equation_size){
        vector_append(&operators,0);
      }else{
        printf("parse error: + operator on wrong line!\n");
      }
    }else if(data=='*'){
      if(line_number == equation_size){
        vector_append(&operators,1);
      }else{
        printf("parse error: * operator on wrong line!\n");
      }
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  if( numbers_first.size != numbers_second.size ||
      numbers_first.size != numbers_third.size ||
      (equation_size == 4 && numbers_first.size != numbers_fourth.size) ||
      numbers_first.size != operators.size ){
      printf("parse error: vector size mismatch! %i %i %i %i %i\n",
              numbers_first.size,numbers_second.size,numbers_third.size,numbers_fourth.size,operators.size);
      return -1;
  }
  
  //part 1 solution
  ull_t grand_total = 0;
  for(int i = 0; i < numbers_first.size; i++){
    ull_t value = 0;
    if(operators.data[i] == 0){ //addition
      if(equation_size == 4){
        value = numbers_first.data[i] + numbers_second.data[i] + numbers_third.data[i] + numbers_fourth.data[i];
        printf("%llu + %llu + %llu + %llu = %llu\n",
                numbers_first.data[i],numbers_second.data[i],numbers_third.data[i],numbers_fourth.data[i],value);
      }else if(equation_size == 3){
        value = numbers_first.data[i] + numbers_second.data[i] + numbers_third.data[i];
        printf("%llu + %llu + %llu = %llu\n",
              numbers_first.data[i],numbers_second.data[i],numbers_third.data[i],value);
      }
    }else if(operators.data[i] == 1){ //multiplication
      if(equation_size == 4){
        value = numbers_first.data[i] * numbers_second.data[i] * numbers_third.data[i] * numbers_fourth.data[i];
        printf("%llu * %llu * %llu * %llu = %llu\n",
              numbers_first.data[i],numbers_second.data[i],numbers_third.data[i],numbers_fourth.data[i],value);
      }else if(equation_size == 3){
        value = numbers_first.data[i] * numbers_second.data[i] * numbers_third.data[i];
        printf("%llu * %llu * %llu = %llu\n",
              numbers_first.data[i],numbers_second.data[i],numbers_third.data[i],value);
      }
    }else{
      printf("parse error: unknown operator!\n");
    }
    grand_total += value;
  }
  printf("grand total: %llu\n", grand_total);
  
  printf("cleanup\n");
  vector_free(&numbers_first);
  vector_free(&numbers_second);
  vector_free(&numbers_third);
  vector_free(&numbers_fourth);
  vector_free(&operators);
  
  return 0;
}
