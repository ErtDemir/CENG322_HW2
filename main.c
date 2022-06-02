// Name : Ertuğrul Demir ID: 260201059 
// Our program not working with undefined command 
// If you use undefined command, bye command cannot work . You should use Ctrl+C.
// Ex: write exit -> nothing happened then write bye -> again nothing happened.
// I cannot solve this problem, sorry.
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 10 // Define size of list


void  parse(char *line, char **argv){ //Parse function  (I got from internet)
 
    while (*line != '\0') {
        while (*line == ' ' || *line == '\t' || *line == '\n')
 
            *line++ = '\0';
        *argv++ = line;
 
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
            line++;
    }
    *argv = '\0';       
}
 

int get_argc(char** argv){ // Get argument count number function
    int count = 0;
    for(int i=0;i < 10;i++){
        if(argv[i]!=NULL)
        count++;
    }
    return count;}



void dir() //dir function
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Dir: %s\n", cwd);
}

void cd(char* path){ //cd function
    if(path == NULL){
    chdir(getenv("HOME"));}
    else{
    chdir(path);}
}

void history(char **history_list, int history_count){ //Show history function
    for (int i = 0; i < history_count; i++)
    {
        printf("%d-%s\n",i,history_list[i]);
    }
    
}

//Add new element to queue (I got from internet but actualy It was easy )
unsigned add_history( const char *command ,int HISTORY_MAX_SIZE, char **history_list,unsigned history_count)
{
   if (history_count < HISTORY_MAX_SIZE) {
        history_list[history_count++] = strdup( command );
   } else {
        free( history_list[0] );
        for (unsigned index = 1; index < HISTORY_MAX_SIZE; index++) {
            history_list[index - 1] = history_list[index];
        }
        history_list[HISTORY_MAX_SIZE - 1] = strdup( command );
    }
    return history_count;
}


void command(char* cmd,char** argv){ // Not built-in command 
    execvp(cmd,argv);
}

void bye() // Exit Command
{
    exit(0);
}

void run() //Result function
{
    // Variable declaration
    char *history_list[SIZE];
    unsigned history_count = 0;
    while (1) // Infinite Loop
    {   
        char* argv[10];   //Creating input empty variables
        char choice[100];
        int argc;
        int flag = 0;

        for(int i=0;i<10;i++){ // Clean the past input from argv
            argv[i] = NULL;
        }

        printf("Myshell >");    //Print the shell name
        fgets(choice,1000,stdin); //Get the input from user
        if (choice[strlen(choice)-1] == '\n') // Clear the last character if it is \n
            choice[strlen(choice)-1] = '\0';

        parse(choice, argv);   // Parse input from string to list
        argc = get_argc(argv); //Get argument count for last input control

        // Add command to history list and increment history count by 1
        history_count = add_history(argv[0],SIZE,history_list,history_count); 

        //Checking input part
        if(!strcmp(argv[argc-1],"&")){ // Checking last argument is "&"
            argv[argc-1]=NULL;
            flag = 1;
        }
        
        if(!strcmp(argv[0], "bye")) //Checking command bye
        {
            bye();        
        }
        else if(!strcmp(argv[0],"dir")) //Checking command dir
        {
            dir();
        }
        else if(!strcmp(argv[0],"cd")) //Checking command cd
        {
            cd(argv[1]);
        }
        else if(!strcmp(argv[0],"history")) //Checking command history
        {
            history(history_list,history_count);
        }
        else //Checking command is not a built-in command 
            {
            int pid = fork();
            if(pid == 0) //Child Process
            { 
                command(argv[0],argv);
            }
            else if(flag == 0) //Checking flag for "&" 
            {
                wait(NULL); //Wait child proccess
            }
        }   
    }
}


int main() { // Main Function
    run();
    return 0;
}