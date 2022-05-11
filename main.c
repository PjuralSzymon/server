#include "common.h"
#include <strings.h>


struct arg_host_struct {
    int port;
    int pipe_in;
    int pipe_out;
    char * data;
};

struct thread_mess{
    int port;
    int mode;
};

void* host_service(void * args);
void send_dummy_data(char* addr, int port);

int main(int argc, char **argv)
{

    // creating temporary data for testing:
    int ports[] = {60001,60011,60021};
    char * page_name[] = {"examplePage.html","examplePage2.html","examplePage3.html"};
    pthread_t t_1;
    pthread_t t_2;
    pthread_t t_3;
    pthread_t threads[]= {t_1,t_2,t_3};

    // creating pipes for communication:
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

    for(int i =0;i<3;i++)
    {
        struct arg_host_struct *args = malloc(sizeof(struct arg_host_struct));
        args->port = ports[i];
        args->pipe_in = pipe_terminate[0];
        args->pipe_out = pipe_info[1];
        args->data = read_file(page_name[i]);
        pthread_create(&threads[i],NULL,host_service,(void*)args);
    }

    //close(pipe_info[1]);
    //close(pipe_terminate[0]);
    int looping = 1;
    while(looping)
    {
        //printf("\e[1;1H\e[2J");
        printf("\n");
        printf("write C to terminate, U to update");
        char ch;
        scanf("%c", &ch);
        printf("input: %c \n", ch);
        if(ch=='C')
        {
            printf("C button on input server termiantes\n");
            looping = 0;
        }
        else if (ch=='U')
        {
            char buffer[8192];
            int n;
            
            if ((n = read ( pipe_info[0], buffer, 8192 ) ) >= 0) 
            {
                buffer[n] = 0;
                printf("read %d bytes from the pipe: %s\n", n, buffer);
            }  
            else
                perror("read");
        }
        else if (ch=='S')
        {
            printf("SEND dummy data\n");
            send_dummy_data("0.0.0.0",ports[0]);
        }
        
    }

    char * T = "TERMINATE";
    for(int i =0;i<3;i++)
    {
        write(pipe_terminate[1], &T, sizeof(T));
        send_dummy_data("0.0.0.0",ports[i]);
        pthread_join(threads[i], NULL);
    }
}



void * host_service(void * args)
{
    //int port, char * data
    struct arg_host_struct *host_args = (struct arg_host_struct *)args;
    int port = host_args->port;
    int pipe_in = host_args->pipe_in;
    int pipe_out = host_args->pipe_out;
    char * data = host_args->data;

    printf("host_service STARTED ON PORT: %d\n",port);


    char * page = connect_two_txt("HTTP/1.0 200 OK \r\n\r\n",data);
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    uint8_t buff[MAXLINE+1];
    uint8_t recvline[MAXLINE+1];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("SOCKET error on port %d \n",port);
        return NULL;
    }
        //err_n_die("socker error.");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
    {
        printf("BIND error on port %d \n",port);
        return NULL;
    }
    if ((listen(listenfd, 100)) < 0)
    {
        printf("LISTEN error on port %d \n",port);
        return NULL;
    }
    char buffer[5];
    int looping = read(pipe_in,buffer,5);
    while(looping<=0)
    {
        //printf("LOOP on port: %d \n",port);
        char * info2 = "LOOP STARTED";
        write(pipe_out,info2,sizeof(info2));

        //printf("MESS sended on port: %d \n",port);
        struct sockaddr_in addr;
        socklen_t addr_len;

        //printf("waiting for a connection on port %d\n",port);
        //fflush(stdout);

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
        
        if(n<0)
            err_n_die("read error");


        write(connfd, page, strlen((char *)page));
        close(connfd);

        char info[] = "[connection on port]";
        struct thread_mess *mess = malloc(sizeof(struct thread_mess));
        mess->port = port;
        mess->mode = 0;
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