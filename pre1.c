#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR sizeof(char)*1000

FILE* file_contents(char* filename);
int dash_f(FILE* file);
int dash_r(FILE* file);
void get_columns(FILE* file, char* filename, char** columns_list);
char* get_column_value(char* field, char* str, char** columns_list);
float dash_minmaxmean(FILE* file, char* field, char* filename, int type);
void findField(FILE* file, char* filename, int field, char* result);
void dash_h_determiner(FILE* input_file, char* filename,char** argv,int argc,int i,int h,int type);


int main(int argc, char* argv[]) {

  FILE* input_file = file_contents(argv[argc-1]);


  if (argc > 2){
    int h = 0;

    //Check if there is -h in the list
    for (int i = 1; i < argc-1; i++) { 
      if (argv[i] == "-h") {
        h = 1;
      }
    }

    //For each arguement in argv  
    for (int i = 1; i < argc-1; i++){ 
      //Get file content
      input_file = file_contents(argv[argc-1]);

      //If -r occurs and there is -h subtract by one otherwise print result
      if (argv[i] == "-r"){ 
        if (h == 1){
          printf("%d\n", dash_r(input_file)-1); 
        }else{
            printf("%d\n", dash_r(input_file));
          }
        }

      //If -f than just call dash_f
      if (argv[i] == "-f"){
        printf("%d\n", dash_f(input_file));
        input_file = file_contents(argv[argc-1]);
      }

      //This obtains the minimum value
      if (argv[i] == "-min"){
        dash_h_determiner(input_file,argv[argc-1],argv,argc,i,h,1);
      }

      //This obtains the maximum value
      if (argv[i] == "-max") {
        dash_h_determiner(input_file,argv[argc-1],argv,argc,i,h,2);
      }

      //This obtains the mean value
      if (argv[i] == "-mean"){
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
  char **tempList;
  get_columns(file,filename,tempList);
  result = strcpy(result,((char*)tempList[field]));
}

void dash_h_determiner(FILE* input_file, char* filename,char** argv,int argc,int i,int h,int type){
  char* field = argv[i+1];
        float max;
        /*
          If h is 0:
            If field == '0' or '-' or its the last value than look at first column
            If atoi can return a number look at that column
          If h is 1:
            Just look at the column labeled that
          Otherwise:
            Exit with a failure
        */
        if (field[0] == '0' || field[0] == '-' || (i + 1 >= argc) && h == 0) {
          //Find the header given position and get the min  
          char* label;
          findField(input_file,argv[argc-1],0,label); 
          max = dash_minmaxmean(input_file, label, argv[argc-1],type);
        }

        else if (atoi(argv[i+1]) != 0 && h == 0) {
          int lookUp = atoi(argv[i+1]);
          int datapoints = dash_f(input_file);
          char* label;

          //If inputted number is larger than amount of datapoints, exit with failure
          if (lookUp >= datapoints){
            printf("Invalid Column, Datapoints: %d, Entered: %d", datapoints,lookUp);
            exit(EXIT_FAILURE);
          }

          //Find the header given position and get the min  
          findField(input_file,argv[argc-1],lookUp,label); 
          max = dash_minmaxmean(input_file, label, argv[argc-1],type);
        }
        else if (h == 1){
          //If h == 1 than use topic
          max = dash_minmaxmean(input_file, field, argv[argc-1],type);
          }
        else {
          //If none of the above, exit with error
          printf("Invalid Column Identifier");
          exit(EXIT_FAILURE);
        }
      printf("%f\n", max);
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
void get_columns(FILE* file, char* filename, char** columns_list){
  int num_columns = dash_f(file);
  file = fopen(filename,"r");

  //Copy the list
  char* str = (char*)malloc(sizeof(char)*1000);
  fgets(str, 100, file);
  char* token = strtok(str, ",");


  for(int i = 0; i < num_columns; i++){
    strcpy(columns_list[i], token);
    token = strtok(NULL, ",");
  }
  free(str);
  fclose(file);
}


char* get_column_value(char* field, char* str, char** columns_list){
  int index = 0;
  int j = 0;
  while (str[j] != '\n'){
    if (strcmp(columns_list[index], field) == 0){
      int k = j;
      char* value = (char*)malloc(sizeof(char)*50);
      while(str[k] != ','){
        value[k-j] = str[k];
        k++;
      }

      str[j] = '\n';
      str[j+1] = '\n';

      return value;

    }else{
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
    if (str[j] == '\"'){
      j++;
      while (str[j] != '\"'){
        j++;
      }
    }
    if (str[j] == ','){
      index++;
    }
    j++;
  }
  return NULL;
}

float dash_minmaxmean(FILE* file, char* field, char* filename, int type){
  int num_columns = dash_f(file);
  file = fopen(filename,"r");

  char** columns_list = (char**)malloc(sizeof(char*)*num_columns);
  for (int i = 0; i < num_columns; i++){
    columns_list[i] = (char*)malloc(sizeof(char)*100);
  }

  get_columns(file, filename, columns_list);

  file = fopen(filename,"r");

  char* str = (char*)malloc(sizeof(char)*10000);
  fgets(str, MAXCHAR, file); 
  //fgets(str, MAXCHAR, file);

  //char* token = strtok(str, ",");
  float min = (float)(1<<12);
  float max = -1;
  float sum = 0;
  float count = 0;
  while(fgets(str, MAXCHAR, file) != NULL){
    char* value = get_column_value(field, str, columns_list);
    float num = atoi(value);
    if (num < min){
      min = num;
    }
    if (num > max){
      max = num;
    }
    sum = sum + num;
    count++;
    fgets(str, MAXCHAR, file);
  }
  float mean = sum/count;


  if (type == 1){
    return min;
  }
  if (type == 2){
    return max;
  }
  if (type == 3){
    return mean;
  }
  
  free(str);
  free(columns_list);
  fclose(file);
  return 0;
}








