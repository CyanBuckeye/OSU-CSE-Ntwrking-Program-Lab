#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#define MAXLEN 100

char buffer[MAXLEN + 1];
FILE *fp = NULL;

int general_search(char* searchStr, char* input_buffer, int input_len, int start_idx, int* occur_time, char* ret)
{
    while(start_idx + strlen(searchStr) <= input_len) //look for the whole searchStr
    {
        bool found = true;
        for(int temp_idx = 0; temp_idx < strlen(searchStr); temp_idx++)
        {
            if((searchStr[temp_idx] & 0xff) != (input_buffer[start_idx + temp_idx] & 0xff)){
                found = false;
                break;
            }
        }
        if(found){
            *occur_time = *occur_time + 1;
        }
        start_idx++;
    }

    int alloc_len = input_len - start_idx;
    if(ret != NULL)
        free(ret);
    ret = (char*)malloc(sizeof(char) * alloc_len);
    memcpy(ret, input_buffer + start_idx, alloc_len);
    return alloc_len;
}

int main(int argc, char **argv){
    if(argc != 4){
        printf("Error Input: You must give two parameters: the first one specifies the input file; the second one is the search string\n");
        return 0;
    }

    buffer[MAXLEN] = '\0';
    char* filePath = argv[1];
    char* searchStr = argv[2];
    char* output_filePath = argv[3];

    if(searchStr == NULL)
    {
        printf("Error Input: searchStr cannot be NULL\n");
    }

    fp = fopen(filePath, "rb");
    if(fp == NULL){
        printf("Error: Fail to open the given file\n");
        return 0;
    }

    FILE* output_fp = fopen(output_filePath, "wb");
    if(output_fp == NULL){
        printf("Error: Fail to open the output file\n");
        return 0;
    }

    int sz = 0;
    int occur_time = 0;
    char *last_rd = NULL;
    size_t last_cnt = 0;
    while(1){
        size_t readLen = fread(buffer, 1, MAXLEN, fp);
        sz += readLen;
        if(last_rd != NULL)
        {
            int strcat_len = strlen(last_rd) + strlen(buffer) + 1;
            char *temp_buffer = (char*)malloc(sizeof(char) * strcat_len);
            memcpy(temp_buffer, last_rd, last_cnt);
            memcpy(temp_buffer + last_cnt, buffer, readLen);
            last_cnt = general_search(searchStr, temp_buffer, last_cnt + readLen, 0, &occur_time, last_rd);
            free(temp_buffer);
        }
        else{
            last_cnt = general_search(searchStr, buffer, readLen, 0, &occur_time, last_rd);
        }
        if(readLen < MAXLEN){
            break;
        }
    }

    fprintf(output_fp, "size of given file is %d\n", sz);
    fprintf(output_fp, "the queried string occurs %d times\n", occur_time);
    printf("size of given file is %d\n", sz);
    printf("the queried string occurs %d times\n", occur_time);
    return 0;
}
