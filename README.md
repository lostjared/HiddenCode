# Pattern Matcher 

<img width="1355" height="1050" alt="image" src="https://github.com/user-attachments/assets/3e73701b-ad6f-4aa2-88bc-747327f29911" />

# Purpose

A C++ tool designed to read an input file, shuffle text buffers randomly, and execute recursive backtracking searches to find complex structural patterns based on alternating case rules and triangular character frequency constraints.

Example Run:

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

  --random              Use a random buffer for pattern matching. Requires iter and chunk.

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
- The chunk size (  will throw an error.--chunk) must be greater than or equal to 6, or the program will return an error.

# Experiment:

Hidden Code Pattern Analysis: Random Data Experiment

## Overview

This experiment was designed to test the statistical inevitability of specific structural letter patterns (such as the 1:2:3 sequence ratios and alternating casing rules found in earlier pattern-matching tests) appearing in arbitrary datasets. The primary goal was to determine whether the discovery of these structural acrostics in text implies intentional placement (intelligent design) or if they are simply a natural consequence of probability and combinatorial odds.

## Methodology

To test this, the pattern-searching algorithm was run against completely randomized string data rather than structured human language.

The test was executed using the custom C++ pattern-matching utility with the following parameters to simulate data shuffling over multiple passes:

```bash
./hidden-code --random --iter 1000 --chunk 20
```

By generating random character strings and applying the exact same backtracking search algorithms and frequency verifications used on standard text, we can measure if the target structures naturally emerge from the noise.

# Results
As documented in the console output (refer to image_2d717a.jpg), the algorithm successfully identified multiple valid pattern matches within the randomized data.

Examples of matched sequences include:

GwWtTt

VwwXxx

MxXqQq

YuUsSs

VkKdDd

The utility verified the Actual:Expected:Character counts and casing rules for each, returning True across the board. The patterns formed perfectly despite the input string being a meaningless, randomly generated sequence.

# Conclusion

The results of this experiment demonstrate that the appearance of these specific structural sequences is not indicative of intentional cryptographic placement. Instead, it is a mathematical inevitability.

Given the constraints of the alphabet, the odds of data shuffling, and the law of large numbers, running a rigorous pattern-matching algorithm over enough data will eventually yield recognized sequences by pure chance. The patterns are an artifact of probability and the sheer volume of parsed data rather than a hidden, deliberately constructed code.

