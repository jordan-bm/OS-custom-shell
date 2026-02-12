#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_HISTORY 100

// Global history storage
char *history[MAX_HISTORY];
int history_count = 0;

// Function declarations
void add_to_history(char *cmd);
void display_prompt();
void execute_command(char **args);

// Built in command functions
void cmd_pwd();
void cmd_echo(char **args);
void cmd_cat(char **args);
void cmd_mkdir(char **args);
void cmd_rmdir(char **args);
void cmd_touch(char **args);
void cmd_wc(char **args);
void cmd_head(char **args);
void cmd_grep(char **args);
void cmd_history();

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    
    while (1) {
        display_prompt();
        
        // Read input
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Skip empty input
        if (strlen(input) == 0) {
            continue;
        }
        
        // Add to history
        add_to_history(input);
        
        // Parse input into arguments
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        
        // Check for exit command
        if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            break;
        }
        
        // Execute command
        execute_command(args);
    }
    
    // Cleanup history
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
    
    return 0;
}

/**
 * Display the shell prompt
 */
void display_prompt() {
    printf("myshell> ");
    fflush(stdout);
}

/**
 * Add command to history
 * @param cmd The command string to add
 */
void add_to_history(char *cmd) {
    if (history_count < MAX_HISTORY) {
        history[history_count] = malloc(strlen(cmd) + 1);
        strcpy(history[history_count], cmd);
        history_count++;
    } else {
        // Shift history and add new command
        free(history[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            history[i] = history[i + 1];
        }
        history[MAX_HISTORY - 1] = malloc(strlen(cmd) + 1);
        strcpy(history[MAX_HISTORY - 1], cmd);
    }
}

/**
 * Execute the given command
 * @param args Array of command arguments (NULL-terminated)
 */
void execute_command(char **args) {
    if (args[0] == NULL) {
        return;
    }
    
    // Check for built-in commands
    if (strcmp(args[0], "pwd") == 0) {
        cmd_pwd();
    } else if (strcmp(args[0], "echo") == 0) {
        cmd_echo(args);
    } else if (strcmp(args[0], "cat") == 0) {
        cmd_cat(args);
    } else if (strcmp(args[0], "mkdir") == 0) {
        cmd_mkdir(args);
    } else if (strcmp(args[0], "rmdir") == 0) {
        cmd_rmdir(args);
    } else if (strcmp(args[0], "touch") == 0) {
        cmd_touch(args);
    } else if (strcmp(args[0], "wc") == 0) {
        cmd_wc(args);
    } else if (strcmp(args[0], "head") == 0) {
        cmd_head(args);
    } else if (strcmp(args[0], "grep") == 0) {
        cmd_grep(args);
    } else if (strcmp(args[0], "history") == 0) {
        cmd_history();
    } else {
        printf("Command not found: %s\n", args[0]);
    }
}

/**
 * Print current working directory
 */
void cmd_pwd() {
    char cwd[MAX_INPUT];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

/**
 * Echo arguments to stdout
 * @param args Array of strings to echo
 */
void cmd_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s", args[i]);
        if (args[i + 1] != NULL) {
            printf(" ");
        }
    }
    printf("\n");
}

/**
 * Display contents of file(s)
 * @param args Array containing filename(s)
 */
void cmd_cat(char **args) {
    if (args[1] == NULL) {
        printf("Usage: cat <filename>\n");
        return;
    }
    
    for (int i = 1; args[i] != NULL; i++) {
        FILE *file = fopen(args[i], "r");
        if (file == NULL) {
            perror(args[i]);
            continue;
        }
        
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }
        
        fclose(file);
    }
}

/**
 * Create directory
 * @param args Array containing directory name(s)
 */
void cmd_mkdir(char **args) {
    if (args[1] == NULL) {
        printf("Usage: mkdir <dirname>\n");
        return;
    }
    
    for (int i = 1; args[i] != NULL; i++) {
        if (mkdir(args[i], 0755) == -1) {
            perror(args[i]);
        }
    }
}

/**
 * Remove empty directory
 * @param args Array containing directory name(s)
 */
void cmd_rmdir(char **args) {
    if (args[1] == NULL) {
        printf("Usage: rmdir <dirname>\n");
        return;
    }
    
    for (int i = 1; args[i] != NULL; i++) {
        if (rmdir(args[i]) == -1) {
            perror(args[i]);
        }
    }
}

/**
 * Create empty file or update timestamp
 * @param args Array containing filename(s)
 */
void cmd_touch(char **args) {
    if (args[1] == NULL) {
        printf("Usage: touch <filename>\n");
        return;
    }
    
    for (int i = 1; args[i] != NULL; i++) {
        FILE *file = fopen(args[i], "a");
        if (file == NULL) {
            perror(args[i]);
        } else {
            fclose(file);
        }
    }
}

/**
 * Count lines, words, and characters in file
 * @param args Array containing filename
 */
void cmd_wc(char **args) {
    if (args[1] == NULL) {
        printf("Usage: wc <filename>\n");
        return;
    }
    
    FILE *file = fopen(args[1], "r");
    if (file == NULL) {
        perror(args[1]);
        return;
    }
    
    int lines = 0, words = 0, chars = 0;
    char ch;
    int in_word = 0;
    
    while ((ch = fgetc(file)) != EOF) {
        chars++;
        if (ch == '\n') {
            lines++;
        }
        if (ch == ' ' || ch == '\n' || ch == '\t') {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            words++;
        }
    }
    
    printf("%d %d %d %s\n", lines, words, chars, args[1]);
    fclose(file);
}

/**
 * Display first 10 lines of file
 * @param args Array containing filename
 */
void cmd_head(char **args) {
    if (args[1] == NULL) {
        printf("Usage: head <filename>\n");
        return;
    }
    
    FILE *file = fopen(args[1], "r");
    if (file == NULL) {
        perror(args[1]);
        return;
    }
    
    char buffer[1024];
    int line_count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && line_count < 10) {
        printf("%s", buffer);
        line_count++;
    }
    
    fclose(file);
}

/**
 * Search for pattern in file
 * @param args Array containing pattern and filename
 */
void cmd_grep(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: grep <pattern> <filename>\n");
        return;
    }
    
    FILE *file = fopen(args[2], "r");
    if (file == NULL) {
        perror(args[2]);
        return;
    }
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, args[1]) != NULL) {
            printf("%s", buffer);
        }
    }
    
    fclose(file);
}

/**
 * Display command history
 */
void cmd_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}