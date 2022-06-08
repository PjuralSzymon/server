#include "common.h"

void err_n_die(const char *fmt, ...)
{
    int errno_save;
    va_list     ap;
    errno_save = errno;

    va_start(ap, fmt);
    vfprintf(stdout,fmt,ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (errno_save != 0)
    {
        fprintf(stdout, "errno = %d : %s\n", errno_save,strerror(errno_save));
        fprintf(stdout,"\n");
        fflush(stdout);
    }

    va_end(ap);
    exit(1);
}

char *bin2hex(const unsigned char *input, size_t len)
{
    char *result;
    char *hexits = "0123456789ABCDEF";

    if(input == NULL || len <= 0)
        return NULL;

    int resultlength = (len*3) + 1;

    result = malloc(resultlength);
    bzero(result, resultlength);

    for(int i = 0; i<len; i++)
    {
        result[i*3] = hexits[input[i] >> 4];
        result[(i*3)+1] = hexits[input[i] & 0x0F];
        result[(i*3)+2] = ' ';
    }

    return result;
}

char * read_file(char * filename)
{
    printf("filename [%s] \n",filename);
    FILE* ptr;
    char str[50];
    ptr = fopen(filename, "rb");

    char * buffer = 0;
    long length;
    length = ftell(ptr);
    fseek(ptr, 0, SEEK_END);
    length = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer)
    {
        fread(buffer,1, length, ptr);
    }
    else
    {
        err_n_die("reading page file");
    }
    fclose(ptr);

    return buffer;

}

char *connect_two_txt(char * orginal, char *new)
{
    char* result;
    result = malloc(strlen(orginal)+1+strlen(new)); /* make space for the new string (should check the return value ...) */
    strcpy(result, orginal); /* copy name into the new var */
    strcat(result, new); /* add the extension */
    return result;
}



struct config_data* read_config_file(char * path)
{
    int number_of_lines = count_lines_in_file(path);
    if(number_of_lines==0)
    {
        printf("no such a file");
        exit(1);  
    }

    FILE * cf = fopen(path,"r");
    if(cf==NULL)
    {
        printf("no such a file");
        exit(1);
    }
    char line[100];

    struct config_data* config_data_set = (struct config_data*)malloc(number_of_lines * sizeof(struct config_data));
    int i = 0;
    int id = 0;
    while (fgets(line, sizeof(line),cf))
    {
        // print the line
        char * line_split = strtok(line," ");
        char * data;

        id = id + 3;
        char *dict = malloc(3*sizeof(char));
        dict[0] = '0' + id;
        dict[1] = '0' + id+1;
        dict[2] = '0' + id+2;
        config_data_set[i].dict = dict;
        config_data_set[i].number_of_websites = number_of_lines;

        int j = 0;
        while(line_split !=NULL)
        {
            data = malloc(strlen(line_split)); 
            strcpy(data, line_split);
            removeChar(data,'\n');
            if(j==0)
            {
                config_data_set[i].name = data;
            }
            else if(j==1)
            {
                config_data_set[i].port = atoi(data);
            }
            else if(j==2)
            {
                config_data_set[i].path = data;
            }
            else if(j==3)
            {
                config_data_set[i].security = data;
            }
            line_split = strtok(NULL," ");
            j = j + 1;
        }
        i = i + 1;
    }
    /*
    for(int i=0;i<number_of_lines;i++)
    {
        printf("config_data_set[%d].name: [%s]\n",i,config_data_set[i].name);
        printf("config_data_set[%d].port: [%d]\n",i,config_data_set[i].port);
        printf("config_data_set[%d].path: [%s]\n\n",i,config_data_set[i].path);
        printf("config_data_set[%d].dict: [%s]\n\n",i,config_data_set[i].dict);
    }
    */
    return config_data_set;
}

int count_lines_in_file(char * path)
{
    FILE * file = fopen(path,"r");
    if(file==NULL)
    {
        return 0;
    }
    char line[100];
    int i = 0;
    while (fgets(line, sizeof(line),file))
    {
        i = i+1;
    }
    fclose(file);
    return i;
}

void removeChar(char * str, char charToRemmove){
    int i, j;
    int len = strlen(str);
    for(i=0; i<len; i++)
    {
        if(str[i] == charToRemmove)
        {
            for(j=i; j<len; j++)
            {
                str[j] = str[j+1];
            }
            len--;
            i--;
        }
    }
    
}
