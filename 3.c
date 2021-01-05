#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void foo(char *array);
int main(void)
{
    {
        DIR *p;
        struct dirent *pp;
        p = opendir(".");
        char fileName[255];

#define NUM_THREADS 4;

        if (p != NULL)
        {

            while ((pp = readdir(p)) != NULL)
            {

                int length = strlen(pp->d_name);
                if (strncmp(pp->d_name + length - 4, ".txt", 4) == 0)
                {

                    int pid = fork();
                    if (pid == 0)
                    {
                        strncpy(fileName, pp->d_name, 250);
                        fileName[254] = '\0';

                        //char *array= argv[1];
                        foo(fileName);
                        sleep(2);
                        wait();
                        //printf("to pid einai  %d \n", pid);
                        //exit(0);
                    }
                    else if (pid < 0)
                    {
                        printf("error with the pid ");
                    }
                    else
                    {
                        //edo einai to parent process
                        wait();
                       
                    }
                }
            }
        }
        (void)closedir(p);
    }
}

void foo(char *array)
{
    printf("eimai mesa stin fuction");
    FILE *fp = fopen(array, "r");
    if (fp == NULL)
    { // checking whether the file exists or not {
        printf("File Not Found!\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    int res = ftell(fp); //counting the size of the file
    fclose(fp);
    printf("to megethos tou arxeiou einai bytes  %s %d", array, res); //closing the file
    thread(res, array);
    return res;
}

void *hello(void *rank)
{
     char*iptr = (char*)rank;
    printf("apotelesma einai afto %s",*iptr);
   
   printf("geia sou   %s",*iptr);
  (*iptr++);
  return NULL;
}

void thread(int res, char *array)
{
    sleep(1);
    printf("to res  einai %d", res);
    long t;
    pthread_t *thread_handles;
    int statikanimata = 2;

    int thread_count = res / statikanimata;
    if (thread_count < 1)
    {
        thread_count = 1;
    }

    thread_handles = malloc(thread_count * sizeof(pthread_t));
    printf("thread_count einai %d kai to array einai %s ", thread_count,array);
    

    for (t = 0; t < thread_count; t++)
        pthread_create(&thread_handles[t], NULL,hello,&array);

    printf("Hello from the main thread\n");

    for (t = 0; t < thread_count; t++)
        pthread_join(thread_handles[t], NULL);

    printf("done\n");

    free(thread_handles);
}
