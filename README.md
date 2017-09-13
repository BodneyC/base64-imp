Base64 Enc/Dec
==============

There are plenty of libraries and other implementations of base64 encoders and decoders and by no means am I attempting to add anything to the concept. This is really just a project to get me used to the OpenMP API which I am likely to be using a lot of next year.

## Use of Program

This program (currently) takes one of two switches from the command line, either '-d' for decode or '-e' for encode both are followed by the string to en/de-code.

    $ encb64 [-d|-e] <string>

I also intend to make the program convert text files as well as output to a text file (instead of stdout). I also need a decoder I suppose.

## Compilation (TBC)

I assume the compilation and use of the program will be nothing more than `g++ encb64.cc -o encb64 -fopenmp` but as this project is hardly begun, it seems illogical to write about it as yet.

## Choice of Language

OpenMP was designed with C (and FORTRAN) in mind, but I chose C++ over C. I know that OpenMP has been extended to some features of C++ and only then OpenMP 4.0 to go no further than C++98 (according to [this thread](https://stackoverflow.com/questions/13837696/can-i-safely-use-openmp-with-c11)) and my initial worry was that I would have to figure out if the issue with my code was based on the C++ syntax, my use of OpenMP, *or* the non-compatibility between the standard and the API, but I feel that picking C++ will get me a little more deep into the background of the language (and the API).
