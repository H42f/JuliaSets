import numpy as np
import matplotlib.pyplot as plt
import os
from subprocess import run
import sys

if __name__ == "__main__":

    # interpret user input:
    if len(sys.argv) != 5:
        print("Invalid number of arguments given!")
        quit(1)
    real = float(sys.argv[1])
    imaginary = float(sys.argv[2])
    power = int(sys.argv[3])
    if power < 1 or power > 4:
        print("Invalid power of polynomial given, must be between 2 and 4!")
        quit(1)
    resolution = sys.argv[4]
    if resolution == "poor":
        xres = 100
        yres = 100
        resolution = 0
    elif resolution == "low":
        xres = 500
        yres = 500
        resolution = 1
    elif resolution == "medium":
        xres = 1000
        yres = 1000
        resolution = 2
    elif resolution == "high":
        xres = 5000
        yres = 5000
        resolution = 3
    elif resolution == "best":
        xres = 10000
        yres = 10000
        resolution = 4
    elif resolution == "720p":
        xres = 1280
        yres = 720
        resolution = 5
    elif resolution == "Full HD":
        xres = 1920
        yres = 1080
        resolution = 6
    elif resolution == "Quad HD":
        xres = 2560
        yres = 1440
        resolution = 7
    elif resolution == "4K":
        xres = 3840
        yres = 2160
        resolution = 8
    elif resolution == "8K":
        xres = 7680
        yres = 4320
        resolution = 9
    elif resolution == "MacBook":
        xres = 2560
        yres = 1664
        resolution = 10
    else:
        print("No valid resolution given.")
        quit(1)
    if not os.path.isdir("./bin"):
        run("mkdir bin", shell=True)
    name = "{re:.6f}_{im:.6f}_{pow}_{res}".format(re=real, im=imaginary, pow=power, res=resolution)
    binary_file = "bin/" + name + ".bin"
    binary_file = os.path.join(os.getcwd(), binary_file)
    csv_file = name + ".csv"
    csv_file = os.path.join(os.getcwd(), csv_file)
    print(binary_file, csv_file)
    if not os.path.isfile(binary_file):
        run("touch {}".format(binary_file), shell=True)
    run("touch {}".format(csv_file), shell=True)

    # call C++ function to perform the necessary calculations
    command = "./src/Deg{pow}/JuliaSet{pow} {re} {im} {res}".format(re=real, im=imaginary, pow=power, res=resolution) \
              + ' ' + binary_file + ' ' + csv_file
    run(command, shell=True)

    # Plot the previously calculated values using matplotlib

    print("Starting to plot the values...")
    Data = np.loadtxt(csv_file, delimiter=',')
    resolutions = [100, 500, 1000, 5000, 10000, 320, 480, 640, 960, 1920, 640]
    x_lim = 2
    y_limits = [2, 2, 2, 2, 2, 1.125, 1.125, 1.125, 1.125, 1.125, 1.3]
    y_lim = y_limits[resolution]
    step_sizes = {0.01, 0.002, 0.001, 0.0002, 0.0001, 1./320, 1./480, 1./640, 1./960, 1./1920, 1./640}
    x = np.linspace(-x_lim, x_lim, xres + 1, endpoint=True)
    y = np.linspace(-y_lim, y_lim, yres + 1, endpoint=True)

    fig, ax = plt.subplots(1, 1, figsize=(10, 10 * y_limits[resolution]/2))
    ax.pcolormesh(x, y, np.transpose(Data))
    plt.axis('off')
    plot_path = os.path.join(os.getcwd(), "plots", name + ".png")
    plt.savefig(plot_path, dpi=2 * resolutions[resolution] / 5, bbox_inches='tight', pad_inches=0)
    print("Finished plotting the values. Cleaning up...")

    command = "rm {}".format(csv_file)
    run(command, shell=True)

    quit(0)