# GoOdDd pattern matcher 

# Purpose

A C++ tool designed to read an input file, shuffle text buffers randomly, and execute recursive backtracking searches to find complex structural patterns based on alternating case rules and triangular character frequency constraints.

Example Run:
<img width="1154" height="1484" alt="image" src="https://github.com/user-attachments/assets/98b8466c-09ad-4a1b-bc34-e7b32cd9247f" />
<img width="1154" height="1484" alt="image" src="https://github.com/user-attachments/assets/d0063fcd-ac12-4e45-a914-9565cd4fc8f0" />

# Pattern Checker Utility

This utility analyzes text strings or files to detect specific repeating character patterns. It has been updated to use a custom argument parsing system (`argz.hpp`) for better command-line flexibility.

# Command-Line Arguments

The program accepts the following arguments. Arguments that require a value should be followed by a space and the value.

  --input <file>        Specifies the path to the text file to be processed. 
                        Required if --keyword is not used.
  
  --keyword <string>    Directly tests a specific keyword or string instead of 
                        reading from a file. If this is provided, the program 
                        will check this string and immediately exit.
                        
  --iter <number>       The number of iterations (epochs) the program should run 
                        when processing a file. Required when using --input.
                        
  --chunk <number>      The size of the text chunk to sample from the file on 
                        each iteration. Must be at least 6. Required when using 
                        --input.
                        
  --rev                 An optional flag. If present, the program will reverse 
                        the input string (either the keyword or the file contents) 
                        before running the pattern check.

# Usage Modes

1. Keyword Mode
If you just want to test a single word or phrase, use the `--keyword` argument. 
You can optionally combine it with `--rev`.

Example:
```bash  
./pattern_check --keyword "AbcDefA"
./pattern_check --keyword "AfeDcbA" --rev
```

2. File Processing Mode
If you want to randomly sample chunks of a larger text file to find patterns, you 
must provide the input file, the number of iterations, and the chunk size.

Example:
```bash
  ./pattern_check --input document.txt --iter 10000 --chunk 20
  ./pattern_check --input document.txt --iter 5000 --chunk 15 --rev
```

# Notes

- If running on a Linux environment, you can interrupt a long-running file 
  processing task using Ctrl+C (SIGINT), and the program will gracefully exit 
  and print its current findings.
- The chunk size (  will throw an error.--chunk) must be greater than or equal to 6, or the program 


