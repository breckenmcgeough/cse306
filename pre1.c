#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAXCHAR sizeof(char)*1000

FILE* file_contents(char* filename);
int dash_f(FILE* file);
int dash_r(FILE* file);
void get_columns(FILE* file, char* filename, char** columns_list);
char* get_column_value(char* field, char* str, char** columns_list);
float dash_minmaxmean(FILE* file, char* field, char* filename, int type,int h);
void findField(FILE* file, char* filename, int field, char* result);
void dash_h_determiner(FILE* input_file, char* filename,char** argv,int argc,int i,int h,int type);


int main(int argc, char* argv[]) {

  FILE* input_file = file_contents(argv[argc-1]);

  if (argc > 2){
    int h = 0;

    //Check if there is -h in the list
    for (int i = 1; i < argc-1; i++) { 
      if (argv[i][0] == '-' && argv[i][1] == 'h') {
        h = 1;
      }
    }

    //For each arguement in argv  
    for (int i = 1; i < argc-1; i++){ 
      //Get file content
      input_file = file_contents(argv[argc-1]);

      //If -r occurs and there is -h subtract by one otherwise print result
      if (argv[i][0] == '-' && argv[i][1] == 'r'){ 
        if (h == 1){
          printf("%d\n", dash_r(input_file)-1); 
        }else{
            printf("%d\n", dash_r(input_file));
          }
        }

      //If -f than just call dash_f
      if (argv[i][0] == '-' && argv[i][1] == 'f'){
        printf("%d\n", dash_f(input_file));
        input_file = file_contents(argv[argc-1]);
      }

      //This obtains the minimum value
      if (argv[i][0] == '-' && argv[i][1] == 'm'&& argv[i][2] == 'i'  && argv[i][3] == 'n'){
        dash_h_determiner(input_file,argv[argc-1],argv,argc,i,h,1);
      }

      //This obtains the maximum value
      if (argv[i][0] == '-' && argv[i][1] == 'm'&& argv[i][2] == 'a'  && argv[i][3] == 'x') {
        dash_h_determiner(input_file,argv[argc-1],argv,argc,i,h,2);
      }

      //This obtains the mean value
      if (argv[i][0] == '-' && argv[i][1] == 'm'&& argv[i][2] == 'e'  && argv[i][3] == 'a' && argv[i][4] == 'n'){
        dash_h_determiner(input_file,argv[argc-1],argv,argc,i,h,3);
      }
    }
  }
  else {
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

//Using get_columns and the field number, return the Data type associated with the number (field)
void findField(FILE* file, char* filename, int field, char* result){
  int num_columns = dash_f(file);
  char** tempList = (char**)malloc(sizeof(char*)*num_columns);

  for (int i = 0; i < num_columns; i++){
    tempList[i] = (char*)malloc(sizeof(char)*50);
  }
  get_columns(file_contents(filename),filename,tempList);
  result = (char*)strcpy(result,tempList[field]);
  free(tempList);
}

void dash_h_determiner(FILE* input_file, char* filename,char** argv,int argc,int i,int h,int type){
  char* field = argv[i+1];
        float finalValue;
        /*
          If h is 0:
            If field == '0' or '-' or its the last value than look at first column
            If atoi can return a number look at that column
          If h is 1:
            Just look at the column labeled that
          Otherwise:
            Exit with a failure
        */
        printf("Type: %s\n",argv[i]);
        printf("\tH: %d\n",h);
        printf("\tField: %s\n",field);
        //If h == 0 and the start of the argv starts with 0, -, or its the end of the file than it should be default value
        if (h == 0 && (field[0] == '0' || (field[0] == '-') || (i + 1 == argc - 1)) ) {
          //Find the header given position and get the min  
          char* label = (char*)malloc(sizeof(char) * 50);
          findField(file_contents(filename),argv[argc-1],0,label);
          printf("\tDefaultField\n");
          printf("\tLabel: %s\n",label);
          finalValue = dash_minmaxmean(file_contents(filename), label, argv[argc-1],type,h);
          free(label);
        }

        else if (h == 0 && atoi(argv[i+1]) != 0) {
          int lookUp = atoi(argv[i+1]);
          int datapoints = dash_f(input_file);
          char* label = (char*)malloc(sizeof(char) * 50);
          //If inputted number is larger than amount of datapoints, exit with failure
          if (lookUp >= datapoints){
            printf("Invalid Column, Datapoints: %d, Entered: %d", datapoints - 1,lookUp);
            exit(EXIT_FAILURE);
          }

          
          findField(file_contents(filename),argv[argc-1],lookUp,label);

          //This if loop activates only for last value per line to remove the \n
          if (lookUp == datapoints-1){
            int i = 0;
            int l = -1;
            while (label[i] != '\0'){
              i += 1;
              l += 1;
            }
            label[l] = '\0';
          }
          //Find the header given position and get the min
          printf("\tAtoiField\n");
          printf("\tAtoi: %d\n",lookUp);
          printf("\tLabel: %s\n",label);
          finalValue = dash_minmaxmean(file_contents(filename), label, argv[argc-1],type,h);

          free(label);
        }
        else if (h == 1){
          //If h == 1 than use topic
          if ((field[0] == '0' || (field[0] == '-') || (i + 1 == argc - 1))){
            printf("Missing String Column Identifier\n");
            exit(EXIT_FAILURE);
          }

          printf("\tHField\n");  
          finalValue = dash_minmaxmean(file_contents(filename), field, argv[argc-1],type,h);
          }
        else {
          //If none of the above, exit with error
          printf("Invalid Column Identifier\n");
          exit(EXIT_FAILURE);
        }
      printf("%f\n", finalValue);
}

//Gets the amount of Datapoints in the csv file
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

//Gets the amount of Lines in the csv file
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

/* 
Given the file, filename, and a place to place the result
Put a matrix that contains the strings seperated by ',' inside of columns list
*/
void get_columns(FILE* file, char* filename, char** columns_list){ //this function gets the column names and puts them in an array
  int num_columns = dash_f(file);
  file = fopen(filename,"r");

  char* str = (char*)malloc(sizeof(char)*500);
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
      char* value = (char*)malloc(sizeof(char)*20);
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
        char* value = (char*)malloc(sizeof(char)*30);
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
    columns_list[i] = (char*)malloc(sizeof(char)*50);
  }
}

float dash_minmaxmean(FILE* file, char* field, char* filename, int type, int h){ //return min or max or mean of a column
  int num_columns = dash_f(file);
  file = fopen(filename,"r");

  char** columns_list = (char**)malloc(sizeof(char*)*num_columns);
  initialize_array(columns_list, num_columns);

  get_columns(file, filename, columns_list); //get the column names and put them in the array

  file = fopen(filename,"r"); //have to keep opening and closing the file since the other functions mess up the file by analyzing it

  char* str = (char*)malloc(sizeof(char)*500);
  fgets(str, MAXCHAR, file); 

  char* field_invar = (char*)malloc(sizeof(char)*30);
  strcpy(field_invar, field); //copy the field into field_invar which is an invariant version of the field 

  //fgets(str, MAXCHAR, file);
  // if (h == 0){ //if no -h detected, then convert the numeric field into the corresponding column name. The numeric field can be used to index the columns_list to get the column name at that index
  //   int pos = (int)atoi(field);
  //   field_invar = realloc(field_invar, strlen(columns_list[pos])+1); //reallocate field_invar to size of column name at index
  //   strcpy(field_invar, columns_list[pos]); //copy the column name at index into field_invar
  //   //printf("%s passed \n", field);  
  // }

  //char* token = strtok(str, ",");
  float min = INT_MAX; //just a massive value 
  float max = INT_MIN;
  float sum = 0;
  float count = 0;
  int foundnumber = 0;
  while(fgets(str, MAXCHAR, file) != NULL){ //iterate through the rows until none left
    char* value = get_column_value(field_invar, str, columns_list); //get the value at the field (column) in the current row
    float num = atof(value); //convert the string to a float
    if (num != 0.0) { // atof returns 0.0 if there is no valid conversion to make
      foundnumber = 1; // set flag that a number was found in the column
    }
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

  if (foundnumber == 0){
    printf("No numerical data was found.");
    exit(EXIT_FAILURE);
  }

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