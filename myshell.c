/**
 * myshell.c - A Simple Linux Shell Implementation
 * 
 * Author: İbrahim Hakkı Yeler
 * Student ID: 64200036
 * University: İstanbul Medipol University
 * Department: Computer Engineering
 * 
 * Description:
 * This program implements a simple command-line shell that can execute
 * external commands. I use fork() to create child processes and execv()
 * to execute commands. The shell waits for each command to complete
 * before displaying the next prompt.
 * 
 * Features:
 * - Displays a custom shell prompt (mysh>)
 * - Executes external commands with arguments
 * - Built-in exit command to terminate the shell
 * - Handles EOF (Ctrl+D) gracefully
 * - Error handling for invalid commands
 * 
 * System Calls Used:
 * - fork()  : Creates a child process
 * - execv() : Replaces the child process with the specified program
 * - wait()  : Waits for the child process to complete
 * - fgets() : Reads user input from standard input
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

/**
 * parse_input - Parses user input into command and arguments
 * 
 * This function takes the raw input string from the user and breaks it down
 * into individual tokens (command and arguments) that can be passed to execv().
 * 
 * Steps:
 * 1. Remove the newline character from the input string
 * 2. Tokenize the input by spaces and tabs using strtok()
 * 3. Store each token in the args array
 * 4. NULL-terminate the args array (required by execv())
 * 
 * Parameters:
 *   input - The raw input string from the user
 *   args  - Array to store the parsed command and arguments
 * 
 * Returns: The number of arguments parsed
 */
int parse_input(char *input, char **args) {
    int i = 0;
    char *token;
    
    // Step 1: Remove newline character if present
    input[strcspn(input, "\n")] = 0;
    
    // Step 2-3: Tokenize the input by spaces and tabs, store in args array
    token = strtok(input, " \t");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }
    
    // Step 4: NULL-terminate the array (required by execv())
    args[i] = NULL;
    
    return i;
}

/**
 * execute_command - Executes a command using fork() and execv()
 * 
 * This function creates a child process to execute the command while the parent
 * process waits for it to complete. I use fork() to create the child process
 * and execv() to replace the child's memory with the new program.
 * 
 * Steps:
 * 1. Fork a new child process using fork()
 * 2. In the child process, attempt to execute the command with execv()
 * 3. Try multiple common paths if the command is not found with exact path
 * 4. In the parent process, wait for the child to complete using wait()
 * 
 * Path Resolution:
 * I try to find the command in the following order:
 *   - Exact path provided (if absolute path is given)
 *   - /bin/ directory
 *   - /usr/bin/ directory
 *   - /usr/local/bin/ directory
 * 
 * Parameters:
 *   args - NULL-terminated array of command and arguments
 * 
 * Returns: 0 on success, -1 on error
 */
int execute_command(char **args) {
    pid_t pid;
    int status;
    char path[MAX_INPUT];
    
    // Step 1: Fork a child process
    pid = fork();
    
    if (pid < 0) {
        // Fork failed - print error and return
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // Step 2: Child process - execute the command
        
        // Step 3: Try to execute with exact path first
        if (execv(args[0], args) == -1) {
            // If that fails, try /bin/ directory
            snprintf(path, sizeof(path), "/bin/%s", args[0]);
            if (execv(path, args) == -1) {
                // If that fails, try /usr/bin/ directory
                snprintf(path, sizeof(path), "/usr/bin/%s", args[0]);
                if (execv(path, args) == -1) {
                    // If that fails, try /usr/local/bin/ directory
                    snprintf(path, sizeof(path), "/usr/local/bin/%s", args[0]);
                    if (execv(path, args) == -1) {
                        // Command not found in any location
                        fprintf(stderr, "mysh: command not found: %s\n", args[0]);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    } else {
        // Step 4: Parent process - wait for child to complete
        if (wait(&status) == -1) {
            perror("wait");
            return -1;
        }
    }
    
    return 0;
}

/**
 * main - Main function that runs the shell loop
 * 
 * This is the entry point of the shell program. I implement an infinite loop
 * that repeatedly displays a prompt, reads user input, parses it, and executes
 * the requested command. The loop continues until the user enters "exit" or
 * presses Ctrl+D (EOF).
 * 
 * Shell Loop Steps:
 * 1. Display the shell prompt (mysh>)
 * 2. Read user input using fgets()
 * 3. Parse the input into command and arguments
 * 4. Check if the command is the built-in "exit" command
 * 5. If not "exit", execute the command using fork() and execv()
 * 6. Wait for the command to complete and repeat
 * 
 * Returns: 0 on successful exit
 */
int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    int num_args;
    
    // Main shell loop - repeat until user exits
    while (1) {
        // Step 1: Display the prompt
        printf("mysh> ");
        fflush(stdout);
        
        // Step 2: Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle EOF (Ctrl+D)
            printf("\n");
            break;
        }
        
        // Step 3: Parse the input into arguments
        num_args = parse_input(input, args);
        
        // Skip empty input (user just pressed Enter)
        if (num_args == 0) {
            continue;
        }
        
        // Step 4: Check for built-in exit command
        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting mysh...\n");
            break;
        }
        
        // Step 5-6: Execute the command and wait for it to complete
        execute_command(args);
    }
    
    return 0;
}

