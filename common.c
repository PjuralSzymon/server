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