#include <stdio.h>
#include <stdlib.h>

FILE* file_contents(char* filename);
int dash_f(FILE* file);
int dash_r(FILE* file);



int main(int argc, char* argv[]) {

  FILE* input_file = file_contents(argv[argc-1]);


  if (argc > 2){
    int h = 0;
    for (int i = 1; i < argc-1; i++){
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
      if (argv[i][0] == '-' && argv[i][1] == 'f'){
        printf("%d\n", dash_f(input_file));
        input_file = file_contents(argv[argc-1]);
      }
      if (argv[i][0] == '-' && argv[i][1] == 'h'){
        h = 1;
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










