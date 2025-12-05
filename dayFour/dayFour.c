#include <stdio.h>
#include <stdlib.h>

#define joltage_size 12

int main(int argc, char **argv) {
  if(argc < 2){
    printf("please specify a filename.\n");
    return -1;
  }
  char* filename = argv[1];
  
  char data = 0;
  int cols = 0;
  int rows = 0;

  //first pass to get array size
  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  do{
    data = fgetc(fptr); //read one character from file
    if(data == '.' || data == '@'){
      if(rows==0){
        cols += 1;
      }
    }else if(data == '\n'){
      rows += 1;
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file

  printf("grid size %i x %i\n",rows,cols);
  char* grid = malloc(rows * cols * sizeof(char));
  char* print_grid = malloc(rows * cols * sizeof(char));
  int col = 0;
  int row = 0;

  //second pass to get data
  fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  do{
    data = fgetc(fptr); //read one character from file
    if(data == '.'){
      if(row<rows && col<cols ){
        grid[row*cols + col] = 0;
        col += 1;
      }else{
        printf("out of bounds! %i %i \n",row,col);
        return -1;
      }
    }else if(data == '@'){
      if(row<rows && col<cols ){
        grid[row*cols + col] = 1;
        col += 1;
      }else{
        printf("out of bounds! %i %i \n",row,col);
        return -1;
      }
    }else if(data == '\n'){
      row += 1;
      col = 0;
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  //third pass to count the data
  unsigned long long available = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if(grid[(i)*cols + (j)]){
        int neighbors = 0;
        if(i>0      && j>0      ){neighbors += grid[(i-1)*cols + (j-1)];}
        if(            j>0      ){neighbors += grid[( i )*cols + (j-1)];}
        if(i<rows-1 && j>0      ){neighbors += grid[(i+1)*cols + (j-1)];}
        if(i>0                  ){neighbors += grid[(i-1)*cols + ( j )];}
        if(i<rows-1             ){neighbors += grid[(i+1)*cols + ( j )];}
        if(i>0      && j<cols-1 ){neighbors += grid[(i-1)*cols + (j+1)];}
        if(j<cols-1             ){neighbors += grid[( i )*cols + (j+1)];}
        if(i<rows-1 && j<cols-1 ){neighbors += grid[(i+1)*cols + (j+1)];}
        if(neighbors < 4){
          available++;
          print_grid[(i)*cols + (j)] = 'x';
        }else{
          print_grid[(i)*cols + (j)] = '@';
        }
      }else{
        print_grid[(i)*cols + (j)] = '.';
      }
    }
  }
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%c", print_grid[(i)*cols + (j)]);
    }
    printf("\n");
  }
  
  //print our final output
  printf("available rolls %llu\n", available);
  
  return 0;
}
