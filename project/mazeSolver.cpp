#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
Mat src, label_dst, erosion_dst, dilation_dst;
int rows, cols;

Mat binarization(Mat m){

  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      if(m.at<unsigned char>(i,j) < 147)
      m.at<unsigned char>(i,j) = 0;
      else
      m.at<unsigned char>(i,j) = 1;
    }
  }
  return m;
}

Mat difference(Mat dil, Mat ero){

  for(int i = 0; i < dil.rows; i++){
    for(int j = 0; j < dil.cols; j++){
      dil.at<unsigned char>(i,j) = dil.at<unsigned char>(i,j) - ero.at<unsigned char>(i,j);
    }
  }
  return dil;
}

Mat hopeErode(Mat m, int element_size){

  int n = 0;
  Mat eroded = m.clone();

  while(n < element_size/2){
    for(int i = 1; i < m.rows -1 ; i++){
      for(int j = 1; j < m.cols -1; j++){

        if(m.at<unsigned char>(i,j) && m.at<unsigned char>(i+1,j) && m.at<unsigned char>(i+1,j+1) &&
        m.at<unsigned char>(i,j+1) && m.at<unsigned char>(i-1,j+1) && m.at<unsigned char>(i-1,j) &&
        m.at<unsigned char>(i-1,j-1) && m.at<unsigned char>(i,j-1) && m.at<unsigned char>(i+1,j-1));

        else{
          eroded.at<unsigned char>(i,j) = 0;
          eroded.at<unsigned char>(i+1,j)= 0;
          eroded.at<unsigned char>(i+1,j+1)= 0;
          eroded.at<unsigned char>(i,j+1)= 0;
          eroded.at<unsigned char>(i-1,j+1)= 0;
          eroded.at<unsigned char>(i-1,j)= 0;
          eroded.at<unsigned char>(i-1,j-1)= 0;
          eroded.at<unsigned char>(i,j-1)= 0;
          eroded.at<unsigned char>(i+1,j-1)= 0;
        }
      }
    }
    n++;
    m = eroded.clone();
  }
  return eroded;
}

Mat hopeDilate(Mat m, int element_size) {
  int i,j,n = 0;
  Mat dilated = m.clone();

  while (n < element_size/2) {
    for(i = 1; i < m.rows-1; i++) {
      for (j = 1; j < m.cols-1; j++) {
        if(!m.at<unsigned char>(i,j) && !m.at<unsigned char>(i+1,j) && !m.at<unsigned char>(i+1,j+1) &&
        !m.at<unsigned char>(i,j+1) && !m.at<unsigned char>(i-1,j+1) && !m.at<unsigned char>(i-1,j) &&
        !m.at<unsigned char>(i-1,j-1) && !m.at<unsigned char>(i,j-1) && !m.at<unsigned char>(i+1,j-1));
        else {
          dilated.at<unsigned char>(i-1,j-1) = 1;
          dilated.at<unsigned char>(i-1,j) = 1;
          dilated.at<unsigned char>(i-1,j+1) = 1;
          dilated.at<unsigned char>(i,j-1) = 1;
          dilated.at<unsigned char>(i,j) = 1;
          dilated.at<unsigned char>(i,j+1) = 1;
          dilated.at<unsigned char>(i+1,j-1) = 1;
          dilated.at<unsigned char>(i+1,j) = 1;
          dilated.at<unsigned char>(i+1,j+1) = 1;
        }
      }
    }
    m = dilated.clone();
    n++;
  }
  return dilated;
}

// direction vectors
const int dx[] = {+1, 0, -1, 0};
const int dy[] = {0, +1, 0, -1};

//function to check in all directions (4-conn)
void checkNearByte(int current_label, int r, int c) {
  if (r < 0 || r == rows) return; // out of bounds
  if (c < 0 || c == cols) return; // out of bounds
  if (label_dst.at<unsigned char>(r,c) || !src.at<unsigned char>(r,c)) return; // already labeled or not marked with 1 in src

  // mark the current cell
  label_dst.at<unsigned char>(r,c) = current_label;

  // recursively mark the neighbors
  for (int direction = 0; direction < 4; ++direction)
  checkNearByte(current_label, r + dx[direction], c + dy[direction]);
}

//function to identify different regions of the labirinth
void find_components() {
  int component = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      if (!label_dst.at<unsigned char>(i,j) && src.at<unsigned char>(i,j)) {
        checkNearByte(++component, i, j);
      }
    }
}


int main(int argc, char*argv[]){
  int64 tin, t1, t2, tfi;
  double timing;
  int64 clock;

  if(argc != 2) {
    fprintf(stderr, "Wrong number of parameters: ./mazeSolver <file>\n");
    exit(-1);
  }

  int i, j, element_size;

  t1 = getTickCount();

  src = imread(argv[1], IMREAD_REDUCED_GRAYSCALE_2);
  rows = src.rows;
  cols = src.cols;
  src = binarization(src);

  //Complememt
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){
      src.at<unsigned char>(i,j) = (src.at<unsigned char>(i,j) - 1) * (-1);
    }
  }

  //Evaluation of the element size based on the maze to be solved
  //Three sides must be checked to be sure to find at least one way out
  int sum_var = 0;
  for(j = 0; j < cols; j++) {
    sum_var += src.at<unsigned char>(0,j);
  }
  element_size = cols - sum_var;

  if(!element_size) {
    sum_var = 0;
    for(i = 0; i < rows; i++) {
      sum_var += src.at<unsigned char>(i,0);
    }
    element_size = rows - sum_var;
  }

  if(!element_size) {
    sum_var = 0;
    for(j = 0; j < cols; j++) {
      sum_var += src.at<unsigned char>(rows-1,j);
    }
    element_size = cols - sum_var;
  }

  // labelization
  label_dst = Mat::zeros(rows, cols, CV_8UC1);
  find_components();

  int trackToFollow = 1;
  for(i = 0; i<rows; i++)
  for(j = 0 ; j<cols; j++){
    if(label_dst.at<unsigned char>(i,j) != trackToFollow)
    label_dst.at<unsigned char>(i,j) = 0;
    else {
      label_dst.at<unsigned char>(i,j) = 1;
    }
  }
  dilation_dst = hopeDilate(label_dst, element_size);
  erosion_dst = hopeErode(dilation_dst, element_size);
  Mat solution = dilation_dst.clone();
  solution = difference(dilation_dst, erosion_dst);

  t2 = getTickCount();
  timing = (t2-t1)/getTickFrequency();
  clock = t2 - t1;
  std::cout << "\nTotal time (s): " << timing << '\n';
  std::cout << "\nTotal n clock cycles: " << clock << '\n';

  return 0;

}
