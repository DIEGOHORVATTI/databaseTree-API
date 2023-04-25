#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <json-c/json.h>
#include "./testTime_2.c"

#define MAX_LEN 256
#define MAX_TEST_LENGTH 1000
#define pathFileJson "./public/data_2.json"

void createDatabaseElementString(double value, char* element, json_object* obj) {
  char output[MAX_LEN];
  sprintf(output, "%.2f", value);
  json_object_object_add(obj, element, json_object_new_string(output));
}

void createDatabaseElementArray(double* value, int size, char* element, json_object* obj) {
  json_object* arr = json_object_new_array();
  for (int i = 0; i < size; i++) {
    char output[MAX_LEN];
    sprintf(output, "%.2f", value[i]);
    json_object_array_add(arr, json_object_new_string(output));
  }
  json_object_object_add(obj, element, arr);
}

int main() {
  double results[MAX_TEST_LENGTH];
  for (int i = 0; i < MAX_TEST_LENGTH; i++) {
    results[i] = get_query_time();
  }

  double sum = 0;
  for (int i = 0; i < MAX_TEST_LENGTH; i++) {
    sum += results[i];
  }

  double average = sum / MAX_TEST_LENGTH;

  json_object* json;
  FILE* fp;
  fp = fopen(pathFileJson, "r+");
  if (fp == NULL) {
    json = json_object_new_object();
  }
  else {
    char buffer[MAX_LEN];
    size_t len = fread(buffer, 1, MAX_LEN, fp);
    fclose(fp);

    if (len == 0) {
      json = json_object_new_object();
    }
    else {
      json = json_tokener_parse(buffer);
      if (json == NULL) {
        printf("Não foi possível parsear o arquivo JSON.\n");
        exit(1);
      }
    }
  }

  createDatabaseElementString(average, "resultFinal", json);
  createDatabaseElementArray(results, MAX_TEST_LENGTH, "resultFinalArray", json);

  fp = fopen(pathFileJson, "w");
  if (fp == NULL) {
    printf("Não foi possível abrir o arquivo JSON.\n");
    exit(1);
  }

  fprintf(fp, "%s", json_object_to_json_string(json));
  printf("json atualizado: %.2fms\n", average);


  fclose(fp);
  json_object_put(json);

  exit(1);
  return 0;
}