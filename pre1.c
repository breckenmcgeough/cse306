#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

FILE* file_contents(char* filename);
int dash_f(FILE* file);
int dash_r(FILE* file);

int main(int argc, char* argv[]) {

  FILE* input_file = file_contents(argv[argc-1]);


  if (argc > 2){
    int h = 0;
    for (int i = 1; i < argc-1; i++){

      //"-r" code
      if (argv[i][0] == '-' && argv[i][1] == 'r'){
        if (h == 1){
          printf("%d\n", dash_r(input_file)-1);
        }else{
          for (int j = i+1; j < argc-1; j++){
            if (argv[j][0] == '-' && argv[j][1] == 'h'){
              h = 1;
            }
          }
          if (h == 1){
            printf("%d\n", dash_r(input_file)-1);
          }else{
            printf("%d\n", dash_r(input_file));
          }
        }
        input_file = file_contents(argv[argc-1]);
      }

      //"-f" code
      if (argv[i][0] == '-' && argv[i][1] == 'f'){
        printf("%d\n", dash_f(input_file));
        input_file = file_contents(argv[argc-1]);
      }

      //"-h" code
      if (argv[i][0] == '-' && argv[i][1] == 'h'){
        h = 1;
      }

      //"-max" code
      if (argv[i] == "-max"){
        if (h == 1){
          dash_maxField_h(argv[argc-1],argv[i+1]);
        }
        else {
          if (argc == i + 1 || !isdigit(argv[i+1])){
            dash_maxField(argv[argc-1],0);
          }
          else{
            dash_maxField(argv[argc-1],(atoi(argv[i+1])));
          }
        }
      }
    }
  }else{
    return EXIT_FAILURE;
  }

  fclose(input_file);

  return EXIT_SUCCESS;
}

FILE* file_contents(char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("Could not open file\n");
    return NULL;
  }
  return file;
}

int dash_f(FILE* file){
  int count = 0;
  char c;
  while((c = fgetc(file)) != '\n'){
    if (c == ','){
      count++;
    }
  }
  fclose(file);
  return count+1;
}

int dash_r(FILE* file){
  int count = 0;
  char c;
  while((c = fgetc(file)) != EOF){
    if (c == '\n'){
      count++;
    }
  }
  fclose(file);
  return count;
}

//This function will stop at the nearest '\n' character
void skipLine(FILE* file){
  char c;
  while(c = fgetc(file) != '\n'){
    c = fgetc(file);
  }
}

bool getNumber(FILE* file,int *result){
  char chance[40];
  char c;
  int i = 0;
  while((c = fgetc(file)) != ',') {
    if (i == 40){
      return false;
    }
    chance[i] = c;
  }

  

}

int dash_maxField(FILE* file, int position){
  int maximumValue;
  bool firstRun = true;
  int commaCount = 0;
  int amountOfCommas = dash_f(file);
  char c;
  
  //Checks if not out of bounds
  if (amountOfCommas < position) {
    printf("maxField requested for position %d, csv file only contains %d.",position,amountOfCommas);
    exit(EXIT_FAILURE);
  }


  //Skips the Header LInes
  skipLine(file);

  //Look Through until EOF
  while((c = fgetc(file)) != EOF) { 
    //If we are in the correct position scan the nearest string
    if(commaCount == position) {
      char temp[100];
      fscanf(file,"%s,",temp);

      //If string is a digit
      if(isdigit(temp)) { 
        int value = atoi(temp);

        //If first run just set maximumValue as value  
        if(firstRun){
          firstRun = false;
          maximumValue = value;
        }
        //If value is bigger than maximumValue, replace
        if(value > maximumValue){
          maximumValue = value;
        }
      }
      //Skipline and set commaCount back to 0
      skipLine(file);
      commaCount = 0;
    }
    else{
      commaCount += 1;
    }
  }

  printf("The Maximum Value is: %d", maximumValue);
  return maximumValue;
}

int dash_maxField_h(FILE* file, char* field){
  int maximumValue;
  int commaCount = 0;
}










