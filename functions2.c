/*
SARAH LIANG
this functions file provides functions to be used in conjuntion with main.c

findCol loops through the first line of a CSV file to find the specific column of the grade you're looking for and returns it

getMin loops through a certain column and finds the smallest grade, excluding blank fields

getMax does the same as getMin, but finds the Maximum gradeLine

getAvg gets the average of the grades in a specific column, exculding blank fields

getCount returns the number of students who made a grade equal to or above the provided number on a specific assignment

getGrade returns the weighted average of a specified students
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int findCol(char csvfile[], char column[]){
  FILE *fp = fopen(csvfile, "r");

  char startLine[301];
  char* line = startLine;
  char startToken[31];
  char* token = startToken;
  fgets(line, 301, fp);

  int count = 0;
  token = strsep(&line, ",");
  while (token != NULL){
    if (strcmp(token, column) == 0){
      break;
    }
    token = strsep(&line, ",");
    count++;
  }

  return count;

  fclose(fp);
}

double getMin(char csvfile[], char column[]){

  double minimum;  //fixme dont initialize
  printf("HELLO\n");
  int numGrades = 0;
  FILE *fp = fopen(csvfile, "r");

  int count = findCol(csvfile, column);

  char x[301];
  char* line = x;
  char y[301];
  char* token = y;
  fgets(line, 301, fp);

  while(1){

      fgets(line, 301, fp);
      if (feof(fp)) break;

      for(int i = 0; i <= count; ++i){
        token = strsep(&line, ",");
        if (i == count){
          double num;

          if(strlen(token)>0){
            sscanf(token, "%lf", &num);
            numGrades++;
          }
          else {
            continue;
          }

          if (numGrades == 1 ) minimum = num;
          else if (num < minimum){
            minimum = num;
          }
        }
      }
  }

  return minimum;

  fclose(fp);
}

double getMax(char csvfile[], char column[]){

  double maximum;
  int numGrades = 0;
  FILE *fp = fopen(csvfile, "r");

  int count = findCol(csvfile, column);


  char x[301];
  char* line = x;
  char y[301];
  char* token = y;
  fgets(line, 301, fp);

  while(1){

      fgets(line, 301, fp);
      if (feof(fp)) break;

      for(int i = 0; i <= count; ++i){
        token = strsep(&line, ",");
        if (i == count){
          double num;

          if(strlen(token)>0){
            sscanf(token, "%lf", &num);
            numGrades++;
          }
          else {
            continue;
          }

          if (numGrades == 1 ) maximum = num;
          else if (num > maximum){
            maximum = num;
          }
        }
      }
  }

  return maximum;

  fclose(fp);
}

double getAvg(char csvfile[], char column[]){

  double total = 0;
  int numGrades = 0;
  double average;

  FILE *fp = fopen(csvfile, "r");

  int count = findCol(csvfile, column);

  char x[301];
  char* line = x;
  char y[301];
  char* token = y;
  fgets(line, 301, fp);

  while(1){

      fgets(line, 301, fp);
      if (feof(fp)) break;

      for(int i = 0; i <= count; ++i){
        token = strsep(&line, ",");
        if (i == count){
          double num;

          if(strlen(token)>0){
            sscanf(token, "%lf", &num);
            numGrades++;
            total += num;
          }
          else continue;

        }
      }
  }
  average = total/(double)numGrades;

  return average;

  fclose(fp);
}

int getCount(char csvfile[], char column[], double threshold){
    int total = 0;
    int numGrades = 0;

    FILE *fp = fopen(csvfile, "r");

    int count = findCol(csvfile, column);

    char x[301];
    char* line = x;
    char y[301];
    char* token = y;
    fgets(line, 301, fp);

    while(1){

        fgets(line, 301, fp);
        if (feof(fp)) break;

        for(int i = 0; i <= count; ++i){
          token = strsep(&line, ",");
          if (i == count){
            double num;

            if(strlen(token)>0){
              sscanf(token, "%lf", &num);
              numGrades++; //FIXME
              if (num >= threshold) total++;
            }
            else continue;

          }
        }
    }
    return total;

    fclose(fp);
}


double getGrade(char csvfile[], char first[], char last[]){
  FILE *fp = fopen(csvfile, "r");
  double finalGrade = 0;
  char keyLine[301];
  fgets(keyLine, 301, fp);


  char* gradeLine = NULL;
  int count = 2;
  while(1){
    char personLine[301];
    char *pLine = personLine;
    char *pToken;
    fgets(personLine, 301, fp);
    if (feof(fp)){
      //no match
      finalGrade = -1;
      break;
    }

    pToken = strsep(&pLine, ",");
    if(strcasecmp(pToken, first) == 0){
      pToken = strsep(&pLine, ",");
      if(strcasecmp(pToken, last) == 0){
        gradeLine = pLine;
        break;
      }
    }
    count++;
  }

  if (finalGrade != -1){

    char* personToken = NULL;
    char* key = keyLine;
    char* keyToken = NULL;

    keyToken = strsep(&key, ",");
    keyToken = strsep(&key, ",");

    double minimum = 100;
    for(int i = 0; i < 45; ++i){
      keyToken = strsep(&key, ",");
      personToken = strsep(&gradeLine, ",");

      double grade;
      if (strlen(personToken) > 0){
        sscanf(personToken, "%lf", &grade);
      }
      else grade = 0;


      switch(*keyToken){
        case 'Q': //QUIZ
          grade *= 10;
          if(grade < minimum){
            minimum = grade;
          }
        case 'L': //LAB
        case 'B': //EXERCISES
          finalGrade += grade * .01;
          break;
        case 'P': //PROJECTS
          switch (*(keyToken+1)){
            case '1':
              grade *= 0.02;
              break;
            case '2':
            case '3':
              grade *= 0.04;
              break;
            case '4':
            case '5':
            case '6':
              grade *= 0.05;
              break;
          }
          finalGrade += grade;
          break;
        case 'E': //EXAMS
          switch(*(keyToken + 1)){
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
              grade = (grade/50.0) * 100 * 0.05;
              break;
            case '7':
            case '8':
              grade = (grade/75) * 100 * 0.075;
          }
          finalGrade +=  grade;
          break;
      }
    }
    //DROP LOWEST
    finalGrade -= minimum * 0.01;
  }

  fclose(fp);
  return finalGrade;
}
