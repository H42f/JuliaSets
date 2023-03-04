#include <fstream>
#include <iostream>
#include <complex>
#include <chrono>
#include <cmath>
#include <string>

#define DEBUG(X)

// global variables, dictionaries to choose from

constexpr std::size_t resolutions[]   = {100, 500, 1000, 5000,
                                         10000, 320, 480, 640,
                                         960, 1920, 640};
constexpr double x_lim                = 2;
constexpr double y_limits[]           = {2, 2, 2, 2,
                                         2, 1.125, 1.125, 1.125,
                                         1.125, 1.125, 1.3};
constexpr double step_sizes[]         = {0.01, 0.002, 0.001, 0.0002,
                                         0.0001, 1./320, 1./480, 1./640,
                                         1./960, 1./1920, 1./640};
constexpr std::size_t max_iteration   = 10000;
// cannot be constexpr qualified because of std::log
const double maxlog                   = std::log(max_iteration);
const double log3                     = std::log(3.);

// supported functions to be iterated

inline std::complex<double> cube (std::complex<double> &z, const std::complex<double> &c) {
    return std::pow(z, 3) + c;
}


int main(int argc, char *argv[]) {

    if (argc != 6) {
        std::cerr << "C++ called with wrong number of arguments :(" << std::endl;
        return 1;
    }
    std::string binary_file(argv[1]);
    // initialize xres, yres, c, step_size,
    double c_real = std::stod(binary_file);
    binary_file = argv[2];
    double c_imag = std::stod(binary_file);
    binary_file = argv[3];
    int resolution = std::stoi(binary_file);
    binary_file = argv[4];
    std::string csv_file = argv[5];

    const std::complex<double> c(c_real, c_imag);
    const double offset = 0.5 + std::sqrt(0.25 + std::abs(c));
    const double y_lim = y_limits[resolution];
    const double step_size = step_sizes[resolution];
    const int x_pixels = 4 * resolutions[resolution];
    const int y_pixels = std::round(2 * y_lim * resolutions[resolution]);


    // read existing file contents
    // important that the file contain nothing but previously computed data

    std::cout << "Reading existing values from file..." << std::endl;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

    std::ifstream is (binary_file, std::ios::binary);
    std::size_t doubles_written = 0;
    if (!is.good()) {
        std::cerr << "Could not open binary file to read existing data." << std::endl;
    }
    else {
        while(is.peek() != EOF) {
            is.ignore(sizeof(double));
            doubles_written++;
        }
    }
   is.close();

    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    std::cout << "Reading finished!" << std::endl;
    std::cout << "The reading process took "
              << ((long) (std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start)).count())/1000000
              << " milliseconds.\n" << std::endl;

    DEBUG(std::cout << doubles_written << " values have been read from the file\n" << std::endl;)


    // continue with the computation of values in the Julia set

    start = std::chrono::high_resolution_clock::now();
    if (doubles_written) std::cout << "Continuing ";
    else std::cout << "Starting ";
    std::cout << "the calculations and storing values in a binary file." << std::endl;

    double next = 0.;
    std::ofstream os (binary_file, std::ios::binary | std::ios::app);

    if (os.good()) {
        for (double i = -x_lim + step_size * (0.5 + doubles_written / y_pixels); i < x_lim; i += step_size) {
            for (double j = -y_lim + step_size * (0.5 + doubles_written % y_pixels); j < y_lim; j += step_size) {
                std::complex<double> current(i, j);
                double absolute = std::abs(current);
                std::size_t k = 0;
                while (absolute < 1e9 && k < max_iteration) {
                    current = cube(current, c);
                    absolute = std::abs(current);
                    k++;
                }
                // cool background but mathematically incorrect
                //if (absolute > offset) next = std::log(k * (maxlog / std::log (absolute)));
                if (absolute > offset) next = std::log((double) k + std::log(maxlog / std::log(absolute))/log3);
                else next = 0.;
                os.write((char *) &next, sizeof(double));
                DEBUG(doubles_written++;)
                DEBUG(if (!(doubles_written % 100000)) std::cout << doubles_written << " values have been computed in total.\n" << std::endl;)
            }
        }
    }
    else std::cerr << "Could not open binary file to store computed values." << std::endl;

    os.close();

    stop = std::chrono::high_resolution_clock::now();
    std::cout << "Calculations and saving finished!" << std::endl;
    std::cout << "These processes process took "
              << ((long) (std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start)).count())/1000000
              << " milliseconds.\n" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Converting the binary values into an easily readable .csv file..." << std::endl;
    is.open(binary_file, std::ios::binary);
    os.open(csv_file);
    for (std::size_t i = 0; i < x_pixels; i++) {
        for (std::size_t j = 0; j < y_pixels - 1; j++) {
            is.read((char *) &next, sizeof(double));
            os << next << ',';
        }
        is.read((char *) &next, sizeof(double));
        os << next << '\n';
    }
    stop = std::chrono::high_resolution_clock::now();
    std::cout << "Writing the values as comma-separated values finished!" << std::endl;
    std::cout << "The transcription took "
              << ((long) (std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start)).count())/1000000
              << " milliseconds.\n" << std::endl;

    return 0;
}