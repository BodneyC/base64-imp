Base64 Enc/Dec
==============

There are plenty of libraries and other implementations of base64 encoders and decoders and by no means am I attempting to add anything to the concept. This is relly just a project to get me used to the OpenMP API which I am likely to be using a lot of next year.

## Use of Program

This program (currently) takes a string from the command line (no switches) and outputs the string in base64 so:

    $ main <string>

But I intend to make the program convert text files as well as output to a text file (instead of stdout). I also need a decoder I suppose.

## Compilation (TBC)

I assume the compilation and use of the program will be nothing more than `gcc main.c -o main -fopenmp` but as this project is hardly begun, it seems illogical to write about it as yet.

## Choice of Language

As OpenMP was designed with C (and FORTRAN) in mind, I chose C over C++. I know that OpenMP was only extended to some features of C and only then OpenMP 4.0 to go no further than C++98 (according to [this thread](https://stackoverflow.com/questions/13837696/can-i-safely-use-openmp-with-c11)); with complete honesty, I wouldn't know which concepts were from the different standards and so instead of having to figure out if the issue with my code was based on the C++ syntax, my use of OpenMP, *or* the non-compatibility between the standard and the API, I chose to just use C.
