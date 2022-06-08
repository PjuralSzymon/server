#include "common.h"

void show_GUI(struct website_status server_status[], int websites, char* pass)
{
    printf("+====================================================+\n");
    printf("| server is running                                  |\n");
    printf("| currently running: %d websites                      \n",websites);
    for(int i = 0;i<websites;i++)
    {
    printf("|-website: %s                                         \n",server_status[i].name);
    printf("| |-security: %d                                        \n",server_status[i].security);
    printf("| |-port: %d                                        \n",server_status[i].port);
    printf("| |-connections: %d                                   \n",server_status[i].number_of_connections);
    printf("| |-errors: %d                                        \n",server_status[i].number_of_errors);
    }
    printf("|                                                    |\n");
    printf("|/ Options:                                          |\n");
    printf("| [U - Update] [R - Reload all websites] [C - finish]|\n");
    printf("+====================================================+\n");
}