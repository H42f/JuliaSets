# Julia Sets


## About

This project was created as a playground for anyone who takes pleasure in the beauty of fractals.

It was written in C++ and Python with the intention of a user calling CreateJuliaSet.py with corresponding command line arguments to have their fractal of choice rendered in ./plots. For performance reasons, 'CreateJuliaSet.py' will then call a C++ function stored in ./src for the necessary calculations — after all, the true beauty of a fractal remains obscured until you explore the infinite features of its self-similarity and the default choices for resoltions follow accordingly, as you will see below.

Currently, the Python interface supports generating functions of the form f(z) = z^n + c, c ∈ {2, 3, 4}. For these, C++ programs are provided in which the corresponding functions are inlined for additional execution speed. They are found in ./src/Degn. Other functions can easily be implemented by using the code template ./src/JuliaSet.cpp, see section 'Experimental features'.


## Prerequisites and Build

Python: Tested using version Python 3.9. In this project the libraries os, sys, subprocess, numpy and matplotlib. It is essential for the program to run properly that these be installed properly.
C++:    This project includes makefiles for each project which you might wish to build. If you can use g++ and GNU make, you can simply navigate to the corresponding folder in your shell and type in 'make'. If not so, choose a compiler and compile each .cpp file ('JuliaSet.cpp' needs only be compiled if you wish to use other than the default functions) to a program of the corresponding name.
    Note: During the testing phase, the clang compiler showed problems when dealing with the file streams. You may be required to choose a different one.
Shell:  The python script is designed to use the common commands of a Unix shell. If you are using this project from Windows, please open the 'CreateJuliaSet.py' code and replace the expressions 'touch', 'mkdir' and the pathname separator '/' as neccessary.

## Usage

The 'CreateJuliaSet.py' script takes four command line arguments:
* The first argument corresponds to the real part of the complex number c.
* The second argument corresponds to the imaginary part of the complex number c.
* The third argument corresponds to the degree of the default polynomial to be used and can take on the values 2, 3 or 4.
* The fourth argument corresponds to the desired resolution of the final picture. There are choices for generic square pictures as well as screen size related ones. Please choose from the following:
    - poor      ->        400  by   400  pixels  (1:1)
    - low       ->       2000  by  2000  pixels  (1:1)
    - medium    ->       4000  by  4000  pixels  (1:1)
    - high      ->      20000  by 20000  pixels  (1:1)
    - best      ->      40000  by 40000  pixels  (1:1)
    - 720p      ->       1280  by   720  pixels  (16:9)
    - Full HD   ->       1920  by  1080  pixels  (16:9)
    - Quad HD   ->       2560  by  1440  pixels  (16:9)
    - 4K        ->       3840  by  2160  pixels  (16:9)
    - 8K        ->       7680  by  4320  pixels  (16:9)
    - MacBook   ->       2560  by  1664  pixels  (20:13)

    Make sure to remember escaping whitespaces if your shell would otherwise interpret them as and argument separator!
    
While using the project, you might observe two files being created, one of them being only temporary. The first one is a binary file in which the results of the calculations are stored such that they won't have to be calculated again when you wish to plot them again, maybe because you'd like to try a different colormap and you are rerunning the python script. Or you choose a high resolution option and can't finish the calculations in a single setting. In this case, no data will be lost upon interrupting the script and the process can be continued at any time with the same command. The binaries can be deleted at any time without any consequences on the program, safe for the loss of data. The second file is a .csv file C++ creates to communicate with the Python script. After successful rendering of the final image, it will be deleted automatically.

If you choose the 'best', please make sure to have a sufficient amount of RAM to render 12.8 GB of data. You may want to delete the binaries afterward to save storage. Also, be aware that as long as multithreading is not yet supported by the program (will be added soon and this section will disappear), the entire process may take up several hours (but as seen before, you can pause it at any time and also, you can of course use your pc otherwise during that period of time).

## Experimental features
### This requires you to be acquainted with some basics of programming.

1. If you wish to use functions other than the standard polynomials, see the code template './src/JuliaSet.cpp'. Add a function in the way the standard polynomials represent as examples and add a switch case to direct the function pointer at it. Recompile the source code. Create a binary and a .csv file to store the values in. Call the program in accordance with its command line arguments as can be extracted from the code. Additional arguments are the function to use and the paths to the .bin and the .csv files.
2. Originating from a bug, you will find a line of code calculating the value of the Julia Set that is currently commented out right above the actual one that is to be used under normal circumstances. It has a comment corresponding to this section. This line can be used to replace the normal one. It will give you a result that is not mathematically accurate, but draws some wavelike behaviour in the peripheral areas of the plot and, to my mind, actually make it more interesting to look at. 


## License

This project is to be treated according to the MIT License. You are welcome to make changes to the code in a local copy of it. Have fun playing with it!
