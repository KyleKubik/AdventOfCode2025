#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct{
  unsigned int x;
  unsigned int y;
  unsigned int z;
  int circuit; 
}Junction;

typedef struct{
  unsigned int j1;
  unsigned int j2;
  double distance;
}Gap;

void swap(Gap* A, unsigned long first, unsigned long second){
  Gap temp = A[first];
  A[first] = A[second];
  A[second] = temp;
}

// Divides array into two partitions
unsigned long partition(Gap* A, unsigned long lo, unsigned long hi){
  double pivot = A[hi].distance; // Choose the last element as the pivot
  // Temporary pivot index
  unsigned long i = lo;
  for (unsigned long j = lo; j < hi; j++){ 
    // If the current element is less than or equal to the pivot
    if (A[j].distance <= pivot){
      // Swap the current element with the element at the temporary pivot index
      swap(A,i,j);
      // Move the temporary pivot index forward
      i = i + 1;
    }
  }
  // Swap the pivot with the last element
  swap(A,i,hi);
  return i; // the pivot index
}
void quicksort(Gap* A, long long lo, long long hi){
  // Ensure indices are in correct order
  if (lo >= hi || lo < 0 ){
    return;
  }

  // Partition array and get the pivot index
  long long p = partition(A, lo, hi);
  
  // Sort the two partitions
  if(p>0){quicksort(A, lo, p - 1);} // Left side of pivot
  quicksort(A, p + 1, hi); // Right side of pivot
}

int main(int argc, char **argv) {
  if(argc < 3){
    printf("please specify a filename and max connections\n");
    return -1;
  }
  char* filename = argv[1];
  unsigned int max_connections = atoi(argv[2]);
  
  int data = 0;
  unsigned int number = 0;
  unsigned int num_junctions = 0;
  Junction junctions[1000] = {0};

  FILE* fptr = fopen(filename, "r");
  if (fptr == NULL) {
    printf("file failed to open.\n");
    return -1;
  }
  do{
    data = fgetc(fptr); //read one character from file
    if(data == '\n'){
      junctions[num_junctions].z = number;
      junctions[num_junctions].circuit = -1;
      num_junctions += 1;
      number = 0;
    }else if(data >= 48 && data <= 57){ 
      number = number*10 + (data - 48); 
    }else if(data == ','){
      if(junctions[num_junctions].x == 0){ 
        junctions[num_junctions].x = number;
      }else if(junctions[num_junctions].y == 0){ 
        junctions[num_junctions].y = number;
      }
      number = 0;
    }else if(data == EOF){
      break; //EOF means we are done
    }else{
      printf("unhandled char %i\n", data);//print unhandled character as int so we can see what it is
    }
  }while(data != EOF);
  fclose(fptr); //dont forget to close the file
  
  /*for(int i = 0; i< num_junctions; i++){
    printf("%i %i %i\n",junctions[i].x,junctions[i].y,junctions[i].z);
  }*/
  
  unsigned long long num_gaps = 0;
  Gap gaps[499500] = {0};
  
  for(int i = 0; i< num_junctions; i++){
    for(int j = i+1; j< num_junctions; j++){
      double x = (double)junctions[j].x - (double)junctions[i].x;
      double y = (double)junctions[j].y - (double)junctions[i].y;
      double z = (double)junctions[j].z - (double)junctions[i].z;
      double x2 = x*x;
      double y2 = y*y;
      double z2 = z*z;
      gaps[num_gaps].j1 = i;
      gaps[num_gaps].j2 = j;
      gaps[num_gaps].distance = sqrt(x2+y2+z2);
      num_gaps += 1;
    }
  }
  
  printf("%llu gaps calculated. sorting...\n",num_gaps);
  quicksort(gaps, 0, num_gaps-1);

  unsigned int num_circuits = 0;
  unsigned int circuit_junctions[1000] = {0};
  unsigned int biggest_circuit = 0;
  for(int i = 0; i < num_gaps && i < max_connections; i++){
    Junction* j1 = &junctions[gaps[i].j1];
    Junction* j2 = &junctions[gaps[i].j2];
    printf("[%i,%i,%i] - ",j1->x,j1->y,j1->z);
    printf("[%i,%i,%i]",j2->x,j2->y,j2->z);
    printf(": %f :",gaps[i].distance);
    if(j1->circuit == -1 && j2->circuit == -1){
      printf("j1 & j2 form new circuit %i\n",num_circuits);
      j1->circuit = num_circuits;
      j2->circuit = num_circuits;
      circuit_junctions[num_circuits] += 2;
      if(circuit_junctions[num_circuits] > biggest_circuit){biggest_circuit = circuit_junctions[num_circuits];}
      num_circuits += 1;
    }else if(j1->circuit == j2->circuit){
      printf("j1 already connected to j2 - skip\n");
    }else if(j1->circuit >= 0 && j2->circuit >= 0){
      int circuit1 = j1->circuit;
      int circuit2 = j2->circuit;
      int size1 = circuit_junctions[j1->circuit];
      int size2 = circuit_junctions[j2->circuit];
      printf("merging %i (size %i) and %i (size %i)\n",circuit1,size1,circuit2,size2);
      if(size1 > size2){
        printf("circuit %i transfered to circuit %i\n",circuit2,circuit1);
        circuit_junctions[j1->circuit] += size2;
        circuit_junctions[j2->circuit] = 0;
        if(circuit_junctions[j1->circuit] > biggest_circuit){biggest_circuit = circuit_junctions[j1->circuit];}
        int num = 0;
        for(int k = 0; k< num_junctions; k++){
          if(junctions[k].circuit == circuit2){
            printf("    moving [%i,%i,%i] to %i\n",junctions[k].x,junctions[k].y,junctions[k].z,circuit1);
            junctions[k].circuit = circuit1;
            num++;
          }
        }
        printf("    %i junctions moved\n",num);
      }else{
        printf("circuit %i transfered to circuit %i\n",circuit1,circuit2);
        circuit_junctions[j2->circuit] += size1;
        circuit_junctions[j1->circuit] = 0;
        if(circuit_junctions[j2->circuit] > biggest_circuit){biggest_circuit = circuit_junctions[j2->circuit];}
        int num = 0;
        for(int k = 0; k< num_junctions; k++){
          if(junctions[k].circuit == circuit1){
            printf("    moving [%i,%i,%i] to %i\n",junctions[k].x,junctions[k].y,junctions[k].z,circuit2);
            junctions[k].circuit = circuit2;
            num++;}
        }
        printf("    %i junctions moved\n",num);
      }
    }else if(j1->circuit >= 0){
      printf("j2 added to circuit %i\n",j1->circuit);
      j2->circuit = j1->circuit;
      circuit_junctions[j1->circuit] += 1;
      if(circuit_junctions[j1->circuit] > biggest_circuit){biggest_circuit = circuit_junctions[j1->circuit];}
    }else if(j2->circuit >= 0){
      printf("j1 added to circuit %i\n",j2->circuit);
      j1->circuit = j2->circuit;
      circuit_junctions[j2->circuit] += 1;
      if(circuit_junctions[j2->circuit] > biggest_circuit){biggest_circuit = circuit_junctions[j2->circuit];}
    }
    
    if(biggest_circuit == num_junctions){
      printf("finally completed the circuit!\n");
      printf("[%i,%i,%i] - ",j1->x,j1->y,j1->z);
      printf("[%i,%i,%i]\n",j2->x,j2->y,j2->z);
      printf("answer: %llu\n",j1->x * j2->x);
      break;
    }
  }
  
  unsigned int total_junctions = 0;
  
  int large1 = 0;
  int large2 = 0;
  int large3 = 0;
  for(int i = 0; i < num_circuits; i++){
    if(circuit_junctions[i] == 0){continue;}
    printf("circuit %i has %i connections\n",i,circuit_junctions[i]);
    total_junctions += circuit_junctions[i];
    if(circuit_junctions[i]>large1){
      large3 = large2;
      large2 = large1;
      large1 = circuit_junctions[i];
    }else if(circuit_junctions[i]>large2){
      large3 = large2;
      large2 = circuit_junctions[i];
    }else if(circuit_junctions[i]>large3){
      large3 = circuit_junctions[i];
    }
  }
  unsigned int unconnected = 0;
  for(int j = 0; j< num_junctions; j++){
    if(junctions[j].circuit == -1){unconnected+=1;}
  }
  total_junctions += unconnected;
  printf("plus there are %i unconnected circuits\n",unconnected);
  
  printf("sanity check there are %i junctions\n",total_junctions);
  
  printf("3 largest circuits are %i %i %i\n",large1, large2, large3);
  printf("answer = %i\n",large1*large2*large3);
  
  return 0;
}
