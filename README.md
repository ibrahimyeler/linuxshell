# Homework Assignment 1: Building a Simple Linux Shell

**GitHub Repository:** [https://github.com/ibrahimyeler/linuxshell]

**Name:** İbrahim Hakkı Yeler  
**Student ID:** 64200036  
**University:** İstanbul Medipol University  
**Department:** Computer Engineering  

******************************************************************************

## Description
This project is a **simple Linux shell** program written in **C**.  
It allows the user to execute external commands and provides a built-in `exit` command to terminate the shell.  
The shell is implemented using fundamental UNIX system calls such as `fork()`, `execv()`, and `wait()`.

******************************************************************************

## Features
- Displays a custom shell prompt (`mysh>`)
- Executes external commands (e.g., `ls`, `date`, `echo`)
- Supports commands with multiple arguments (e.g., `ls -l`, `echo hello world`)
- Includes a built-in `exit` command
- Waits for each command to finish before showing the next prompt

********************************************************************************

## How to Compile
Use the following command to compile the program:

```bash
gcc -o myshell myshell.c -Wall
```

This will create an executable file named `myshell` in the current directory.

******************************************************************************

## How to Run
After compilation, run the shell with:

```bash
./myshell
```

You will see the `mysh>` prompt where you can enter commands.

******************************************************************************

## Command Examples

### Basic Commands
```
mysh> ls
myshell.c  README.md  myshell

mysh> date
Sun Oct 19 22:13:26 +03 2025

mysh> echo hello world
hello world
```

### Commands with Arguments
```
mysh> ls -l
total 96
-rw-r--r--  1 user  staff   2803 Oct 19 22:11 hw1_submission.zip
-rwxr-xr-x  1 user  staff  34328 Oct 19 22:11 myshell
-rw-r--r--  1 user  staff   2761 Oct 19 20:50 myshell.c
-rw-r--r--  1 user  staff   3591 Oct 19 20:52 README.md

mysh> pwd
/Users/macbookpro/Documents/hw1

mysh> echo this is a test
this is a test
```

### Exiting the Shell
```
mysh> exit
Exiting mysh...
```

You can also exit using `Ctrl+D` (EOF).

******************************************************************************

## Implementation Details

### Key Functions
- **`parse_input()`** - Parses user input into command and arguments using `strtok()`
- **`execute_command()`** - Uses `fork()` to create a child process and `execv()` to execute the command
- **`main()`** - Main loop that displays prompt, reads input, and handles the exit command

### System Calls Used
- **`fork()`** - Creates a child process
- **`execv()`** - Replaces the child process with the specified program
- **`wait()`** - Waits for the child process to complete
- **`fgets()`** - Reads user input from standard input

### Path Resolution
The shell attempts to find commands in the following locations:
1. Exact path provided (if absolute path is given)
2. `/bin/` directory
3. `/usr/bin/` directory
4. `/usr/local/bin/` directory

If the command is not found in any of these locations, an error message is displayed.

******************************************************************************

## AI Assistance Documentation

**This assignment was completed with AI assistance.**

- **AI Tool Used:** Claude AI (Anthropic) via Cursor IDE
- **Date:** October 19, 2025
- **Assistance Provided:**
  - Complete implementation of the shell program in C
  - Code structure following UNIX systems programming best practices
  - Implementation of `fork()`, `execv()`, and `wait()` system calls
  - Input parsing and command execution logic
  - Error handling for invalid commands
  - Comments and documentation
  - This README file with compilation instructions and examples

The student (İbrahim Hakkı Yeler) provided the assignment requirements, and the AI generated the complete solution including source code and documentation.

******************************************************************************

## Limitations
- Does not support background jobs (using `&`)
- Does not support I/O redirection (`>`, `<`, `>>`)
- Does not support piping (`|`)sadeqce 
- Does not support environment variable expansion
- Limited path resolution (only checks common directories)

******************************************************************************

## Notes
- The program does **not** use the `system()` function as per assignment restrictions
- All commands are executed in child processes using `fork()` and `execv()`
- The parent process waits for each child to complete before displaying the next prompt
