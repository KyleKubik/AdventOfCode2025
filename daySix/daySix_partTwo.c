#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long ull_t;

typedef struct {
  char* data;
  unsigned int size;
  unsigned int capacity;
}Vector;

void vector_append(Vector* vec, char data){
  if(vec->size == vec->capacity){
    if(vec->capacity == 0){
      vec->data = malloc(16 * sizeof(char));
      if(vec->data == NULL) {
          printf("data Memory allocation failed!\n");
      }
      vec->capacity = 16;
    }else{
      unsigned int new_capacity = vec->capacity*2;
      char* new_data = malloc(new_capacity * sizeof(char));
      if(new_data == NULL) {
          printf("new_data Memory allocation failed!\n");
      }
      memcpy(new_data, vec->data, vec->capacity * sizeof(char));
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

int main(int argc, char **argv) {
  if(argc < 3){
    printf("please specify a filename and equation size.\n");
    return -1;
  }
  char* filename = argv[1];
  int equation_size = atoi(argv[2]);
  
  int data = 0;
  int line_number = 0;
  Vector characters_first = {0};
  Vector characters_second = {0};
  Vector characters_third = {0};
  Vector characters_fourth = {0};
  Vector characters_operators = {0};

  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  do{
    data = fgetc(fptr); //read one character from file
    if(data == '\n'){
      line_number += 1;
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      if(line_number == 0){
        vector_append(&characters_first,data);
      }else if(line_number == 1){
        vector_append(&characters_second,data);
      }else if(line_number == 2){
        vector_append(&characters_third,data);
      }else if(line_number == 3){
        if(equation_size == 4){
          vector_append(&characters_fourth,data);
        }else{
          vector_append(&characters_operators,data);
        }
      }else if(line_number == 4){
        vector_append(&characters_operators,data);
      }
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  if( characters_first.size != characters_second.size ||
      characters_first.size != characters_third.size ||
      (equation_size == 4 && characters_first.size != characters_fourth.size) ||
      characters_first.size != characters_operators.size ){
      printf("parse error: vector size mismatch! %i %i %i %i %i\n",
              characters_first.size,characters_second.size,characters_third.size,characters_fourth.size,characters_operators.size);
      return -1;
  }
  
  ull_t grand_total = 0;
  ull_t numbers[4] = {0};
  int steps = 0;
  for( int i = characters_first.size-1; i >=0; i-- ){
    if(equation_size == 4){
      char number_string[] = {characters_first.data[i],characters_second.data[i],characters_third.data[i],characters_fourth.data[i],0};
      numbers[steps] = atoi(number_string);
    }else{
      char number_string[] = {characters_first.data[i],characters_second.data[i],characters_third.data[i],0};
      numbers[steps] = atoi(number_string);
    }
    if(numbers[steps] == 0){printf("error - string became 0!\n");}
    steps++;
    
    if(characters_operators.data[i] == '+'){
      ull_t value = numbers[0];
      printf("%llu ",numbers[0]);
      for(int j = 1; j < steps; j++){
        value = value + numbers[j];
        printf("+ %llu ",numbers[j]);
      }
      printf("= %llu\n",value);
      grand_total += value;
      steps = 0;
      i--;
    }else if(characters_operators.data[i] == '*'){
      ull_t value = numbers[0];
      printf("%llu ",numbers[0]);
      for(int j = 1; j < steps; j++){
        value = value * numbers[j];
        printf("* %llu ",numbers[j]);
      }
      printf("= %llu\n",value);
      grand_total += value;
      steps = 0;
      i--;
    }
  }
  
  printf("grand total: %llu\n", grand_total);
  
  printf("cleanup\n");
  vector_free(&characters_first);
  vector_free(&characters_second);
  vector_free(&characters_third);
  vector_free(&characters_fourth);
  vector_free(&characters_operators);
  
  return 0;
}
