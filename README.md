# commandline-interface-win

## Introduction

Simple command line interface in C. The project is inspired by the git and getopt in Linux. 

## Feature 

* Parse the options and pass the variables to it.

* The non-options will be moved to the back of `argv`.

* Simple usage message.

## Usage

Please read the sample code. 


## Limitation

* Must use `=` to pass the value to the option. For example, must use `cmd.exe --name=name.txt` instead of `cmd.exe --name name.txt`. Just want to keep it simple.
