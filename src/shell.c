//
//  Created by Rohit Raikhy on 7/18/20.
//  Copyright © 2020 Rohit Raikhy. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<readline/readline.h>

// Set buffer size for input. Set history size for history function to save commands.
#define BUFFER_SIZE 80
#define HISTORY_SIZE 100

// Set for the clear command.
#define clear() printf("\033[H\033[J")

// array for history command.
char* historyLog [HISTORY_SIZE];
// count for history index.
int historyIndex = 0;

// Create a signal handler
// Returns nothing
// System call is invoked to end the shell
void sigint_handler(int sig){
    // Ask yourself why 35? This is the number of bytes to write.
    write(1,"\nmini-shell terminated\n",35);
    exit(0);
}

// exit function for shell.
// system call is invoked to exit the child process.
void exitShell() {
    exit(0);
}

// history function for shell.
// shows the history of commands the user has type.
void history() {
    
    char** commands;
    printf("\nHistory of commands: \n\n");
    
    for(int i = 0; i < historyIndex; i ++) {
        printf("%s\n", historyLog[i]);
    }
}

// Shows details of built in commands of the mini shell.
// Returns nothing.
void helpFunction() {
    
    char* builtInFunctions [5];
    builtInFunctions[1] = "exit";
    builtInFunctions[2] = "help";
    builtInFunctions[3] = "history";
    builtInFunctions[4] = "cd";
    
    printf("\nWelcome to the Help Command.\n\n");
    printf("\nThese are the options of commands that are build in:\n\n");
    
    printf("-------------------------------------------------\n");
    printf("-------------------------------------------------\n");
    printf("**************** Commands Build in **************\n");
    printf("-------------------------------------------------\n");
    printf("-------------------------------------------------\n");
    
    printf("\n* %s\n",builtInFunctions[1]);
    printf("\nThis command will exit the mini shell.\n");
    printf("\n* %s\n",builtInFunctions[2]);
    printf("\nThis command opens up the help doc for built in commands.\n");
    printf("\n* %s\n",builtInFunctions[3]);
    printf("\nThis command will show the history of commands entered.\n");
    printf("\n* %s\n",builtInFunctions[4]);
    printf("\nThis command will change directories.\n\n");

}

// cd function for the minishell.
// returns an int determing if function works as expected.
int cdFunction(char* path) {
    int result;
    result = chdir(path);
    return result;
}


// get the users input from mini shell.
// returns an array of strings of the users input.
char** getUserInput() {
    
    char* input = malloc(sizeof(char) * BUFFER_SIZE);
    fgets(input, BUFFER_SIZE, stdin);
    
    // check to see if input is empty.
    if(strcmp(input, "") == 0) {
        return NULL;
    }
    
    // fgets will end on new line so need to replace new line char with something else.
    
    if(strchr(input, '\n') != NULL) {
        char* newLineChar = strchr(input, '\n');
        *newLineChar = '\0';
    }

    char* token = strtok(input, " ");
    char** myargv = malloc(sizeof(char) * BUFFER_SIZE);
    
    int i = 0;
    while(token != NULL) {
        myargv[i] = token;
        token = strtok (NULL, " ");
        i += 1;
    }
    return myargv;
}
    
// prints username to the screen.
// returns nothing.
void printPrompt() {
    printf("mini-shell -%s> ", getenv("USER"));
}


// main: program starts here.
int main(){

    // Install our signal handler
    signal(SIGINT, sigint_handler);
    
    while(1) {
        printPrompt();
        char** myargv = getUserInput();
            
        if(myargv[0] == NULL) {
            continue;
            }
            
        historyLog[historyIndex] = myargv[0];
        historyIndex += 1;
        
       if(strcmp(myargv[0], "exit") == 0) {
           free(myargv);
           exitShell();
       }
            
        if(strcmp(myargv[0], "history") == 0) {
            history();
            continue;
        }
            
        if(strcmp(myargv[0], "help") == 0) {
            helpFunction();
            continue;
        }
            
        if(strcmp(myargv[0], "cd") == 0) {
            int result = cdFunction(myargv[1]);
            if(result == 0) {
                continue;
            } else {
                printf("Directory is not valid.\n");
                continue;
            }
        }

        int pid = fork();
        if(pid == 0){
        // Executes command from child then terminates our process
            int returnCode = execvp(myargv[0],myargv);
            if(returnCode < 0) {
                printf("Command not found--Did you mean something else? %s\n", myargv[0]);
                exit(returnCode);
        }
            //exit(returnCode);
        }else {
            wait(NULL); // This call makes the parent wait on its children.
    }
    }
    printf("Finally, print from the parent\n");
    return 0;
}

