#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR sizeof(char)*1000

FILE* file_contents(char* filename);
int dash_f(FILE* file);
int dash_r(FILE* file);
void get_columns(FILE* file, char* filename, char** columns_list);
char* get_column_value(char* field, char* str, char** columns_list);
void initialize_array(char** columns_list, int num_columns);
float dash_minmaxmean(FILE* file, char* field, char* filename, int type, int h);


int main(int argc, char* argv[]) {

  FILE* input_file = file_contents(argv[argc-1]);


  if (argc > 2){
    int h = 0;
    for (int i = 1; i < argc-1; i++) { //loop through all arguments and check for '-h' and set h to 1 if there is a '-h'
      if (argv[i][0] == '-' && argv[i][1] == 'h') {
        h = 1;
      }
    }
    for (int i = 1; i < argc-1; i++){ // loop through all the arguments
      if (argv[i][0] == '-' && argv[i][1] == 'r'){ // check for '-r'
        if (h == 1){
          printf("%d\n", dash_r(input_file)-1); // subtract 1 to account for header line if '-h' was present
        }else{
            printf("%d\n", dash_r(input_file));
          }
        }
        input_file = file_contents(argv[argc-1]);
      if (argv[i][0] == '-' && argv[i][1] == 'f'){
        printf("%d\n", dash_f(input_file));
        input_file = file_contents(argv[argc-1]);
      }
      if (argv[i][0] == '-' && argv[i][1] == 'm' && argv[i][2] == 'i' && argv[i][3] == 'n'){
        char* field = argv[i+1];
        float min = dash_minmaxmean(input_file, field, argv[argc-1],1, h);
        printf("%f\n", min);
        i += 1;
      }
      if (argv[i][0] == '-' && argv[i][1] == 'm' && argv[i][2] == 'a' && argv[i][3] == 'x'){
        char* field = argv[i+1];
        float max = dash_minmaxmean(input_file, field, argv[argc-1],2, h);
        printf("%f\n", max);
        input_file = file_contents(argv[argc-1]);
        i += 1;
      }
      if (argv[i][0] == '-' && argv[i][1] == 'm' && argv[i][2] == 'e' && argv[i][3] == 'a' && argv[i][4] == 'n'){
        char* field = argv[i+1];
        float mean = dash_minmaxmean(input_file, field, argv[argc-1],3, h);
        printf("%f\n", mean);
        input_file = file_contents(argv[argc-1]);
        i += 1;
      }
    }
  }else{
    return EXIT_FAILURE;
  }

  fclose(input_file);

  return EXIT_SUCCESS;
}

FILE* file_contents(char* filename) { //this function opens the file and returns the file pointer
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("Could not open file\n");
    return NULL;
  }
  return file;
}

int dash_f(FILE* file){ //this function counts the number of columns in the file
  int count = 0;
  char c = fgetc(file);
  while(c != '\n'){
    if (c == ','){
      count++;
    }
    c = fgetc(file);
  }
  fclose(file);
  return count+1;
}

int dash_r(FILE* file){
  int count = 0;
  char c = fgetc(file);
  while(c != EOF){
    if (c == '\n'){
      count++;
    }
    c = fgetc(file);
  }
  fclose(file);
  return count;
}

void get_columns(FILE* file, char* filename, char** columns_list){ //this function gets the column names and puts them in an array
  int num_columns = dash_f(file);
  file = fopen(filename,"r");

  char* str = (char*)malloc(sizeof(char)*1000);
  fgets(str, 100, file);
  char* token = strtok(str, ",");

  //char** columns_list = (char**)malloc(sizeof(char*)*num_columns);

  for(int i = 0; i < num_columns; i++){
    strcpy(columns_list[i], token);
    token = strtok(NULL, ",");
  }
  free(str);
  fclose(file);
}


char* get_column_value(char* field, char* str, char** columns_list){ //this function gets the value at a certain column given a row 
  int index = 0;
  int j = 0;
  while (str[j] != '\n'){ //iterate through the row character by character
    if (strcmp(columns_list[index], field) == 0){ //if the inputted field matches the column name at the 'index' index
      int k = j;
      char* value = (char*)malloc(sizeof(char)*50);
      while(str[k] != ','){ //iterate through chars until a comma is found and construct the string stored at that column in the row
        value[k-j] = str[k];
        k++;
      }

      str[j] = '\n';
      str[j+1] = '\n';

      return value;

    }else{ //edge case to handle if the column name is the last column (Value) and the field wont match since Value is actually Value\n
      int m = 1;
      for (int o = 0; o < strlen(field); o++){
        if (field[o] != columns_list[index][o]){
          m = 0;
        }
      }
      if (m == 1){
        int k = j;
        char* value = (char*)malloc(sizeof(char)*50);
        while(str[k] != '\n'){
          value[k-j] = str[k];
          k++;
        }

        str[j] = '\n';
        str[j+1] = '\n';
        return value;
      }
    }
    if (str[j] == '\"'){ //check if there is an open quote and iterate through chars to the next quote and disregard the string in between
      j++;
      while (str[j] != '\"'){
        j++;
      }
    }
    if (str[j] == ','){ //if a comma is found move to the next column
      index++;
    }
    j++;
  }
  return NULL;
}

void initialize_array(char** columns_list, int num_columns){
  for (int i = 0; i < num_columns; i++){ //allocate memory for each column name (string) in the array of strings
    columns_list[i] = (char*)malloc(sizeof(char)*100);
  }
}


float dash_minmaxmean(FILE* file, char* field, char* filename, int type, int h){ //return min or max or mean of a column
  int num_columns = dash_f(file);
  file = fopen(filename,"r");

  char** columns_list = (char**)malloc(sizeof(char*)*num_columns);
  initialize_array(columns_list, num_columns);

  get_columns(file, filename, columns_list); //get the column names and put them in the array

  file = fopen(filename,"r"); //have to keep opening and closing the file since the other functions mess up the file by analyzing it

  char* str = (char*)malloc(sizeof(char)*10000);
  fgets(str, MAXCHAR, file); 

  char* field_invar = (char*)malloc(sizeof(char)*50);
  strcpy(field_invar, field); //copy the field into field_invar which is an invariant version of the field 

  //fgets(str, MAXCHAR, file);
  if (h == 0){ //if no -h detected, then convert the numeric field into the corresponding column name. The numeric field can be used to index the columns_list to get the column name at that index
    int pos = (int)atoi(field);
    field_invar = realloc(field_invar, strlen(columns_list[pos])+1); //reallocate field_invar to size of column name at index
    strcpy(field_invar, columns_list[pos]); //copy the column name at index into field_invar
    //printf("%s passed \n", field);  
  }

  //char* token = strtok(str, ",");
  float min = (float)(1<<12); //just a massive value 
  float max = -1;
  float sum = 0;
  float count = 0;
  while(fgets(str, MAXCHAR, file) != NULL){ //iterate through the rows until none left
    char* value = get_column_value(field_invar, str, columns_list); //get the value at the field (column) in the current row
    float num = atoi(value); //convert the string to a float
    if (num < min){
      min = num;
    }
    if (num > max){
      max = num;
    }
    sum = sum + num;
    count++;
    fgets(str, MAXCHAR, file); //get next row
  }
  float mean = sum/count;

  float return_val;
  if (type == 1){
    return_val = min;
  }
  if (type == 2){
    return_val = max;
  }
  if (type == 3){
    return_val = mean;
  }
  
  free(str);
  for (int i = 0; i < num_columns; i++){
    free(columns_list[i]);
  }
  free(columns_list);
  free(field_invar);
  fclose(file);
  return return_val;
}






