#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>

static pthread_mutex_t pt_lock = PTHREAD_MUTEX_INITIALIZER;
struct data
{
    int pos;
    char filename[40];
    int size;
    int pes;
    int counter;
    int pid;
};

void signalhandler(int signum)
{
    printf("lathos signal!!! %d\n", signum);
}

int main(void)
{
    {
        struct data dedomena;

        DIR *p;
        struct dirent *pp;
        char fakelos[20];
        printf("Kalispera dwse fakelo i ama thes to trexonta pata .");
        scanf("%s", fakelos);
        p = opendir(fakelos);

        char fileName[255];
        signal(SIGINT, signalhandler);
        signal(SIGTERM, signalhandler);

#define NUM_THREADS 4;

        if (p != NULL)
        {
            while ((pp = readdir(p)) != NULL)
            {
                int length = strlen(pp->d_name);
                if (strncmp(pp->d_name + length - 4, ".txt", 4) == 0) //perno mono ta txt files
                {
                    int pid = fork();
                    if (pid == 0)
                    {
                        printf("Process started: PID=%d, PPID=%d \n", getpid(), getppid());
                        dedomena.pid = getpid();
                        //gia to signal
                        printf("Getpid  %d ", getpid());
                        strncpy(fileName, pp->d_name, 250);
                        fileName[254] = '\0'; // gia na peraso stin fuction
                        fileopen(fileName);
                        sleep(2); // exo valei polla sleep o logos eiani gia na vlepo pio arga ta apotelesmata
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
        else
            printf("wrong directory \n");
        (void)closedir(p);
    }
}

    void fileopen(char *array)
{
    FILE *fp = fopen(array, "r");
    if (fp == NULL)
    {
        printf("File Not Found!\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END); //edo einai gia to megethos tou arxeiou
    int res = ftell(fp);
    fclose(fp);
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
    int pos = dedomena->pos - 7;
    // edo gia ena logo exo bug kai den ksekinaei apo tin arxi mallon giati ektelite pio grigora i main apo to thread  omos me to -7 mporo kai to ftiaxno ligo..
    file = open(dedomena->filename, O_RDONLY);
    lseek(file, pos, SEEK_CUR); // afto einai gia to pointer pou na  ksekinisi to read
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

    dedomena->counter = dedomena->counter + count; // ta vazw sto struct
    close(file);
}

void thread(int res, char *array)
{
    FILE *fp;
    struct data dedomena[4];
    strcpy(dedomena->filename, array);
    dedomena->size = res; //afto mpourousa na to valo kai stin proigoumeni fuction 
    long t;
    dedomena->counter = 0;
    pthread_t *thread_handles;
    int statikanimata = 4;
    int thread_count = res / statikanimata; // px 12.5 // to res einai to size tou arxeiou!
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
        pthread_mutex_lock(&pt_lock);
        pthread_create(&thread_handles[t], NULL, spit, &dedomena);
        pthread_mutex_unlock(&pt_lock);
        dedomena->pos = thread_count + dedomena->pos;
    }

    for (t = 0; t < 4; t++)
        pthread_join(thread_handles[t], NULL);
    printf("ta TOTAL dedomena einai %d  kai to filename einai %s", dedomena->counter, dedomena->filename);
    fp = fopen("output.txt", "a");
    fprintf(fp, " to pid einai %d  to megethos einai %d kai to onoma einai  %s \n", getpid(), dedomena->counter, dedomena->filename); // ta vazo sto file
    free(thread_handles);
}
