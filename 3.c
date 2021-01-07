#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
static pthread_mutex_t pt_lock = PTHREAD_MUTEX_INITIALIZER;
struct data
{
    int pos;
    char filename[40];
    int size;
    int pes;
};

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
                        foo(fileName);
                        sleep(2);
                        wait();
                        exit(0);
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
    {
        printf("File Not Found!\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    int res = ftell(fp);
    fclose(fp);
    printf("to megethos tou arxeiou einai bytes %d", res);
    thread(res, array);
    return res;
}

void *spit(void *arxeia)
{

    struct data *dedomena = arxeia;
    int seg_size = dedomena->pes;
    int i;
    int count;
    int file = 0;
    char buffer[seg_size];
    int pos = dedomena->pos;
    file = open(dedomena->filename, O_RDONLY);
    lseek(file, pos, SEEK_CUR);
    read(file, buffer, seg_size);
    printf("%s\n", buffer);
    for (int i = 0; i < seg_size; i++)
    {
        if (buffer[i] == ' ')
        {
            count++;
        }
    }
    printf(" to counter einai  %d\n", count);
    close(file);
}

void thread(int res, char *array)
{
    struct data dedomena[4];
    strcpy(dedomena->filename, array);
    dedomena->size = res;
    sleep(1);
    long t;
    pthread_t *thread_handles;
    int statikanimata = 4;
    int thread_count = res / statikanimata; //12.5
    if (thread_count < 1)
    {
        thread_count = 1;
    }
    dedomena->pos = 0;
    dedomena->pes = thread_count;

    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for (t = 0; t < 4; t++)
    {
        sleep(1);
        pthread_create(&thread_handles[t], NULL, spit, &dedomena);
        pthread_mutex_lock(&pt_lock);
        dedomena->pos = thread_count + dedomena->pos;
        pthread_mutex_unlock(&pt_lock);
        printf("ta dedomena pou auksanontai einai edw pera  %d \n", dedomena->pos);
    }

    for (t = 0; t < 4; t++)
        pthread_join(thread_handles[t], NULL);

    printf("done\n");
    free(thread_handles);
}
