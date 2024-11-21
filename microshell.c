#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

const char color[8][30]=
{
    "\x1B[0m" , 
    "\x1B[32m", //green
    "\x1B[33m", //yellow
    "\x1B[34m", //blue
    "\x1B[35m", //magenta
    "\x1B[36m", //cyan
    "\x1B[37m", //white
    "\x1B[31m", //red
};
int temp_color = 0;

void help();
void change_color(char *command);
void cp(char *from, char *to);
void ls(char *path);
int main() {

    char command[1024];
    char path[1024];
    char *login = getenv("USER");
    char *args [1024];
    while (1) 
    {
        getcwd(path, sizeof(path));
        printf("\n[%s] %s$ ", path, login);
        fgets(command, 1024, stdin);
        command[strcspn(command, "\n")] = 0;

        int i = 0;
        char *p = strtok(command, " ");
        while (p != NULL)
        {
            args[i++] = p;
            p = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0) 
        {
            break;
        } 
        else if(strcmp(args[0], "cd") == 0) 
        {
            if(chdir(args[1]) == -1)
                printf("%sError: wrong direction%s", color[7],color[temp_color]);
        } 
        else if(strcmp(args[0], "help") == 0)
        {
            help();
        }
        else if (strcmp(args[0], "colors") == 0)
        {   
            change_color(args[1]);
        }
        else if (strcmp(args[0], "clear") == 0)
        {
            printf("\e[1;1H\e[2J");
        }
        else if (strcmp(args[0], "cp") == 0)
        {
            cp(args[1], args[2]);
        }
        else if(strcmp(args[0], "ls") == 0)
        {
            if (args[1] == NULL)
                ls(path);
            else
                ls(args[1]);
        }
        else
        {
            pid_t pid;
            pid = fork();
            if (pid == 0)
            {
                execvp(args[0],args);
                printf("%sError: command not found\n%s", color[7],color[temp_color]);
                exit(1);
            }
            else
            {
                wait(NULL);
            }
        }
    }
    return 0;
}

void help()
{
    printf("%s *** Autor powłoki: Bartosz Owczarek ***\n", color[4]);
    printf("%s\nDostępne komendy to:\n", color[1]);
    printf("%s-exit\n", color[temp_color]);
    printf("-cd \n");
    printf("-colors\n");
    printf("-colors help\n");
    printf("-clear\n");
    printf("-cp\n");
}

void change_color(char *command)
{
    int x = atoi(command);
    if (strcmp(command, "help") == 0)
    {
        printf("%sType 1 to set green font color\n", color[1]);
        printf("%sType 2 to set yellow font color\n", color[2]);
        printf("%sType 3 to set blue font color\n", color[3]);
        printf("%sType 4 to set magenta font color\n", color[4]);
        printf("%sType 5 to set cyan font color\n", color[5]);
        printf("%sType 6 to set white font color%s\n", color[6], color[temp_color]);
    }
    else if ((x>=0) && (x<=7))
    {
        printf("%s",color[x]);
        temp_color = x;
    }
    else
        printf("%sError: command not found\n%s", color[7],color[temp_color]);
}

void cp(char *from, char *to)
{
    int   c;
    FILE *frm;
    FILE *copy; 

    frm = fopen (from, "r");
    copy = fopen (to, "w");
    
    while ((c = fgetc(frm)) != EOF)
        fputc (c, copy);
    fclose (frm);
    fclose (copy);
}
void ls(char *path)
{
    DIR *d = opendir(path);
    struct dirent *dir;
    if(d)
    {
        while((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}
