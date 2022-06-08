#include "common.h"

int security_check(char* recvline, char* password)
{
    char * result = strstr(recvline, password);
    if (result)
    {
        return 1;
    }
    return 0;
}

char* get_random_password(int size)
{
    srand(time(NULL));
    char* new_password = (char *)malloc(size * sizeof(char));
    int max = 122;
    int min = 97;
    for(int i=0;i<size;i++)
    {
        new_password[i] = (int)(rand()%(max - min) + min);
    }
    return new_password;
}

char* add_pass_to_website(char * page,char * pass)
{
    char * new_div_start = "\n<div id=\"Prot\" style=\"visibility: hidden\">";
    char * new_div_end = "</div>";
    char * new_div_1 = connect_two_txt(page,new_div_start);
    char * new_div_2 = connect_two_txt(new_div_1,pass);
    char * new_div_3 = connect_two_txt(new_div_2,new_div_end);
    return new_div_3;
}