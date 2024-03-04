# Maze Solver

## Overview

Welcome to the Improved Maze Solver – a refined version of a maze-solving algorithm designed to analyze and navigate through complex mazes. This project has undergone enhancements and optimizations to achieve superior results in maze resolution. While I did not initiate this project, I have contributed to its improvement by refining existing functionalities and introducing new features for a more efficient maze-solving experience.

## Project Overview
The Improved Maze Solver is a C++ program leveraging image processing techniques, particularly OpenCV, to analyze maze images and determine an optimal path through them. The algorithm comprises several key steps, including image binarization, morphological operations, region and path identification, and performance evaluation. These enhancements aim to provide a robust solution for various maze structures.

## Prerequisites

Make sure you have OpenCV installed to compile and run the program. You can install OpenCV using the following command:

```bash
sudo apt-get install libopencv-dev
```

## Compilation

Compile the program using a C++ compiler. For example:

```bash
g++ -o mazeSolver mazeSolver.cpp `pkg-config --cflags --libs opencv`
```

## Usage

Run the compiled executable with the maze image file as a command-line parameter:

```bash
./mazeSolver <maze_image_file>
```

## Program Steps

1. **Binarization and Complement**
The program begins by converting the maze image to a binary format, distinguishing between light and dark areas. Additionally, a complement step enhances the representation of maze elements.

2. **Element Size Evaluation**
An intelligent element size evaluation has been introduced to adapt to different maze structures, ensuring accurate processing for optimal path determination.

3. **Labelization and Path Identification**
The algorithm labels connected regions in the maze, facilitating a clearer identification of paths. Morphological operations, including erosion and dilation, play a crucial role in enhancing the path visibility.

4. **Solution Calculation**
The Improved Maze Solver calculates the optimal solution path by considering the results obtained from morphological operations. The solution is represented as a visual map highlighting the navigable paths.

5. **Performance Evaluation**
An enhanced performance evaluation mechanism provides insights into the efficiency of the algorithm, measuring total execution time and clock cycles.

## Performance Metrics

- **Total Time:**
  Indicates the overall duration of the program from start to finish, expressed in seconds.

- **Clock Cycles:**
  Represents the total number of clock cycles required to execute the program.

## Output

The program saves the solution image in a file named "escape_result.png" for visual inspection.

Feel free to use and modify this program for your maze-solving needs!
