#include "common.h"
#include "gui.h"
#include <strings.h>


struct arg_host_struct {
    int port;
    int pipe_in;
    int pipe_out;
    char * data;
    char * dictionary;
};

void* host_service(void * args);
void send_dummy_data(char* addr, int port);



int main(int argc, char **argv)
{
    struct config_data* cf_data_set = read_config_file("/home/szymon/SERVER/config.txt");
    // creating temporary data for testing:
    int websites = cf_data_set[0].number_of_websites;
    struct website_status* server_status = (struct website_status*)malloc(websites * sizeof(struct website_status));
    pthread_t * threads = malloc(websites * sizeof(pthread_t));
    int pipe_info[2];
    int pipe_terminate[2];
    // indx 0 read  1 write
    if(pipe(pipe_info) == -1 || pipe(pipe_terminate) == -1)
    {
        printf("An Error occured while creating a pipe");
        return 1;
    }

    fcntl(pipe_info[0], F_SETFL, O_NONBLOCK); 
    fcntl(pipe_info[1], F_SETFL, O_NONBLOCK); 
    fcntl(pipe_terminate[0], F_SETFL, O_NONBLOCK); 
    fcntl(pipe_terminate[1], F_SETFL, O_NONBLOCK); 

    for(int i =0;i<websites;i++)
    {
        printf("cf_data_set[i].path: %s\n",cf_data_set[i].path);
        struct arg_host_struct *args = malloc(sizeof(struct arg_host_struct));
        args->port = cf_data_set[i].port;
        args->pipe_in = pipe_terminate[0];
        args->pipe_out = pipe_info[1];
        args->data = read_file(cf_data_set[i].path);
        args->dictionary = cf_data_set[i].dict;
        server_status[i].name = cf_data_set[i].name;
        server_status[i].port = cf_data_set[i].port;
        server_status[i].number_of_connections = 0;
        server_status[i].number_of_errors = 0;

        pthread_t new_thread;
        threads[i] = new_thread;
        pthread_create(&threads[i],NULL,host_service,(void*)args);
    }

    int looping = 1;
    while(looping)
    {
        printf("\e[1;1H\e[2J");
        show_GUI(server_status,websites);
        char ch;
        scanf("%c", &ch);
        if(ch=='C')
        {
            looping = 0;
        }
        else if (ch=='U')
        {
            int buf_size = 8192;
            char buffer[buf_size];
            int n;
            for(int i =0;i<buf_size;i++)buffer[i]=' ';
            
            if ((n = read ( pipe_info[0], buffer, buf_size ) ) >= 0) 
            {
                buffer[n] = 0;
            }  
            else
                perror("read");
            for(int i=0;i<buf_size;i++)
            {
                for(int j=0;j<websites;j++)
                {
                    if(cf_data_set[j].dict[0]==buffer[i])
                    {
                        server_status[j].number_of_connections = server_status[j].number_of_connections + 1;
                    }
                    if(cf_data_set[j].dict[1]==buffer[i])
                    {
                        server_status[j].number_of_errors = server_status[j].number_of_errors + 1;
                    }
                }
            }
        }
        else if (ch=='R')
        {
            for(int i =0;i<3;i++)
            {
                send_dummy_data("0.0.0.0",server_status[i].port);
            }
        }
        
    }

    char * T = "TERMINATE";
    for(int i =0;i<3;i++)
    {
        write(pipe_terminate[1], &T, sizeof(T));
        send_dummy_data("0.0.0.0",server_status[i].port);
        pthread_join(threads[i], NULL);
    }
}

void * host_service(void * args)
{
    struct arg_host_struct *host_args = (struct arg_host_struct *)args;
    int port = host_args->port;
    int pipe_in = host_args->pipe_in;
    int pipe_out = host_args->pipe_out;
    char * data = host_args->data;
    char * dict = host_args->dictionary;

    char * page = connect_two_txt("HTTP/1.0 200 OK \r\n\r\n",data);
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    uint8_t buff[MAXLINE+1];
    uint8_t recvline[MAXLINE+1];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // send flag about the error
        char mess[1];
        mess[0] = dict[1];
        write(pipe_out,mess,sizeof(mess));
        printf("SOCKET error on port %d \n",port);
        return NULL;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
    {
        // send flag about the error
        char mess[1];
        mess[0] = dict[1];
        write(pipe_out,mess,sizeof(mess));
        printf("BIND error on port %d \n",port);
        return NULL;
    }
    if ((listen(listenfd, 100)) < 0)
    {
        // send flag about the error
        char mess[1];
        mess[0] = dict[1];
        write(pipe_out,mess,sizeof(mess));
        printf("LISTEN error on port %d \n",port);
        return NULL;
    }
    char buffer[5];
    int looping = read(pipe_in,buffer,5);
    while(looping<=0)
    {
        struct sockaddr_in addr;
        socklen_t addr_len;

        connfd = accept(listenfd, (SA *) NULL, NULL);

        memset(recvline, 0, MAXLINE);

        while( (n = read(connfd, recvline, MAXLINE-1)) > 0)
        {
            if(recvline[n-1]=='\n')
            {
                break;
            }
            memset(recvline,0,MAXLINE);
        }
        printf("recvline: %s \n",recvline);
        
        if(n<0)
        {
            // send flag about the error
            char mess[1];
            mess[0] = dict[1];
            write(pipe_out,mess,sizeof(mess));
            err_n_die("read error");
        }


        write(connfd, page, strlen((char *)page));
        close(connfd);

        // send flag about the connection
        char mess[1];
        mess[0] = dict[0];
        write(pipe_out,mess,sizeof(mess));

        looping = read(pipe_in,buffer,5);
    }
    printf("CONNECTION ENDED ON PORT: %d \n",port);
    close(listenfd);
    return NULL;
}



void send_dummy_data(char* addr, int port)
{
    int sockfd,n;
    int sendbytes=50;
    struct sockaddr_in servaddr;
    char sendline[sendbytes];

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("Error while creating a dummy socket");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if(inet_pton(AF_INET,addr,&servaddr.sin_addr) <= 0)
    {
        printf("Error for inet_pton");
    }

    if(connect(sockfd,(SA*) &servaddr,sizeof(servaddr)) < 0 )
    {
        printf("COnnection failed");
    }

    sprintf(sendline,"GET / HTTP/1,1\r\n\r\n");

    if(write(sockfd, sendline, sendbytes) != sendbytes)
    {
        printf("Sending data failed");
    }
    close(sockfd);
}