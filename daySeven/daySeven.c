#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if(argc < 2){
    printf("please specify a filename\n");
    return -1;
  }
  char* filename = argv[1];
  
  int data = 0;

  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  fseek(fptr, 0, SEEK_END);
  long long file_len = ftell(fptr);
  rewind(fptr);
  char* input_data = malloc(sizeof(char) * file_len);
  fread(input_data, sizeof(char), file_len, fptr);
  fclose(fptr); //dont forget to close the file
  
  long long width = 0;
  for(int i=0; i<file_len; i++){
    if(input_data[i] == '\n'){
      width = i + 1;
      break;
    }
  }
  long long height = (file_len / width) - 1;
  
  long long* simulation_data = malloc(sizeof(long long) * file_len);
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width-1; j++){
      switch(input_data[(i)*width+(j)]){
        case 'S':
          simulation_data[(i)*width+(j)] = -1; break;
        case '^':
          simulation_data[(i)*width+(j)] = -2; break;
        case '.':
          simulation_data[(i)*width+(j)] = 0; break;
      }
    }
  }
  
  long long splits = 0;
  long long timelines = 0;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width-1; j++){
      switch(simulation_data[(i)*width+(j)]){
        case -1:
          simulation_data[(i+1)*width+(j)] = 1;
          break;
        case -2:
          if(i>0){
            if(simulation_data[(i-1)*width+(j)] > 0){
              splits += 1;
              simulation_data[(i)*width+(j-1)] = simulation_data[(i)*width+(j-1)] + simulation_data[(i-1)*width+(j)];
              simulation_data[(i)*width+(j+1)] = simulation_data[(i)*width+(j+1)] + simulation_data[(i-1)*width+(j)];
            }
          }
          break;
        default:
          if(i>0){
            if(simulation_data[(i-1)*width+(j)] >= 0){
              simulation_data[(i)*width+(j)] = simulation_data[(i)*width+(j)] + simulation_data[(i-1)*width+(j)];
            }
          }
          break; 
      }
    }
  }
  
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width-1; j++){
      if(simulation_data[i*width + j] > 0 && simulation_data[i*width + j] <= 9){
        printf("%c",simulation_data[i*width + j]+48);
      }else if(simulation_data[i*width + j] > 9 && simulation_data[i*width + j] <= 35){
        printf("%c",simulation_data[i*width + j]+96 - 9);
      }else if(simulation_data[i*width + j] > 35 && simulation_data[i*width + j] <= 61){
        printf("%c",simulation_data[i*width + j]+65 - 35);
      }else if(simulation_data[i*width + j] == -1){
        printf("%c",'S');
      }else if(simulation_data[i*width + j] == -2){
        printf("%c",'^');
      }else if(simulation_data[i*width + j] == 0){
        printf("%c",'.');
      }else{
        printf("%c",'~');
      }
    }
    printf("\n");
  }
  
  for(int j = 0; j < width-1; j++){
    if(simulation_data[(height-1)*width + j] > 0){
      timelines += simulation_data[(height-1)*width + j];
    }
  }
  
  printf("number of splits: %ld\n",splits);
  printf("number of timelines: %ld\n",timelines);
  
  return 0;
}
