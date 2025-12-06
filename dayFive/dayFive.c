#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned long long min;
  unsigned long long max;
}Range;
typedef struct {
  Range** data;
  unsigned int size;
  unsigned int capacity;
}Vector;
void vector_append(Vector* vec, Range* ptr){
  if(vec->size == vec->capacity){
    if(vec->capacity == 0){
      vec->data = malloc(16 * sizeof(Range*));
      if(vec->data == NULL) {
          printf("data Memory allocation failed!\n");
      }
      vec->capacity = 16;
    }else{
      unsigned int new_capacity = vec->capacity*2;
      Range** new_data = malloc(new_capacity * sizeof(Range*));
      if(new_data == NULL) {
          printf("new_data Memory allocation failed!\n");
      }
      memcpy(new_data, vec->data, vec->capacity * sizeof(Range*));
      free(vec->data);
      vec->data = new_data;
      vec->capacity = new_capacity;
    }
  }
  vec->data[vec->size] = ptr;
  vec->size = vec->size + 1;
};

void vector_free(Vector* vec){
  for(int i = 0; i < vec->size; i++){
    free(vec->data[i]);
  }
  free(vec->data);
  vec->data = NULL;
  vec->size = 0;
  vec->capacity = 0;
}

int main(int argc, char **argv) {
  if(argc < 2){
    printf("please specify a filename.\n");
    return -1;
  }
  char* filename = argv[1];
  
  int data = 0;
  char newline_count = 0;
  char check_ingredients = 0;
  unsigned long long number = 0;
  Range* range = NULL;
  Vector ranges = {0};
  
  
  unsigned long long total_fresh = 0;

  //first pass to get array size
  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  do{
    data = fgetc(fptr); //read one character from file
    if(data >= 48 && data <= 57){ //ASCII characters 48 to 57 are numerals 0 to 9.
      number = (number*10) + (data - 48);
      newline_count = 0;
    }else if(data == '-'){
      if(check_ingredients == 0){
        range = malloc(sizeof(Range));
        if (range == NULL) {
            printf("range Memory allocation failed!\n");
            return -1;
        }
        range->min = number;
        number = 0;
        newline_count = 0;
      }else{
        printf("got a - while checking ingredients!\n");
        return -1;
      }
    }else if(data == '\n'){
      if(check_ingredients == 0){
        if(newline_count == 0){
          range->max = number;
          number = 0;
          vector_append(&ranges, range);
          range = NULL;
          newline_count = 1;
        }else{
          number = 0;
          check_ingredients = 1;
          printf("time to check ingredients\n");
        }
      }else{
        //check ingredients
        for(int i = 0; i < ranges.size; i++){
          Range* _range = ranges.data[i];
          if(_range->min <= number && number <= _range->max){
            printf("ingredient %llu is fresh!\n", number);
            total_fresh += 1;
            break;
          }
        }
        number = 0;
      }
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  printf("%llu ingredients are fresh!\n",total_fresh);
  
  Vector new_ranges = {0};
  int compressions = 0;
  do{
    for(int i = 0; i < ranges.size; i++){
      Range* _range = ranges.data[i];
      int overlap = 0;
      for(int j = 0; j < new_ranges.size; j++){
        Range* _new_range = new_ranges.data[j];
        if(_range->min <= _new_range->min && _range->max >= _new_range->min){
          _new_range->min = _range->min;
          overlap = 1;
        }
        if(_range->max >= _new_range->max && _range->min <= _new_range->max){
          _new_range->max = _range->max;
          overlap = 1;
        }
        if(_range->min >= _new_range->min && _range->max <= _new_range->max){
          overlap = 1;
        }
      } 
      if(overlap == 0){
        Range* add_range = malloc(sizeof(Range));
        add_range->min = _range->min;
        add_range->max = _range->max;
        vector_append(&new_ranges, add_range);
      }
    }
    printf("%i ranges compressed to %i...\n",ranges.size, new_ranges.size);
    compressions = ranges.size - new_ranges.size;
    
    if(compressions > 0){
      vector_free(&ranges);
      for(int j = 0; j < new_ranges.size; j++){
        Range* add_range = malloc(sizeof(Range));
        add_range->min = new_ranges.data[j]->min;
        add_range->max = new_ranges.data[j]->max;
        vector_append(&ranges,add_range);
      }
      vector_free(&new_ranges);
    }
    
  }while(compressions > 0);
  
  unsigned long long total_fresh_ids = 0;
  for(int j = 0; j < new_ranges.size; j++){
    Range* new_range = new_ranges.data[j];
    printf("%llu - %llu\n",new_range->min, new_range->max);
    total_fresh_ids += (new_range->max - new_range->min + 1);
  }
  
  printf("%llu IDs are fresh!\n",total_fresh_ids);
  
  printf("cleanup\n");
  vector_free(&ranges);
  vector_free(&new_ranges);
  
  return 0;
}
