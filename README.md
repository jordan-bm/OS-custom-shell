# Custom Shell Implementation

## Student Information
- **Name**: Jordan Burmylo-Magrann
- **RUID**: jjb420

## Description
Custom Unix shell implementation that supports the following built-in commands:

### Implemented Commands:
1. **cat** - Concatenate and display file contents
2. **pwd** - Print working directory
3. **mkdir** - Create new directories
4. **rmdir** - Remove empty directories
5. **echo** - Display text
6. **touch** - Create empty files or update timestamps
7. **wc** - Count lines, words, and characters in files
8. **head** - Display first 10 lines of a file
9. **grep** - Search for patterns in files
10. **history** - Display command history
11. **exit** - Exit the shell

## Building and Running

### Compile:
```bash
make
```

### Run:
```bash
make run
```

Or directly:
```bash
./shell
```

### Clean:
```bash
make clean
```

## Implementation Details

### Architecture:
- **Main loop**: Continuously reads and processes user input
- **Command parsing**: Tokenizes input into command and arguments
- **Command execution**: Dispatches to appropriate handler function
- **History management**: Maintains up to 100 previous commands

### System Calls Used:
- `getcwd()` - Get current working directory
- `mkdir()` - Create directory
- `rmdir()` - Remove directory
- `fopen()/fclose()` - File operations
- Standard I/O functions for file reading/writing

## Usage Examples
```bash
myshell> pwd
/home/user/workspace

myshell> mkdir test_dir
myshell> touch test.txt
myshell> echo Hello World
Hello World

myshell> cat test.txt
[file contents]

myshell> wc test.txt
10 50 300 test.txt

myshell> history
1 pwd
2 mkdir test_dir
3 touch test.txt

myshell> exit
```

## Notes
- Commands are implemented as built-in functions
- Maximum command length: 1024 characters
- Maximum arguments: 64