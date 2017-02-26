//
//  ImageMedianFilter.cpp
//  ImageSmoothingWithMatrices
//
//  Created by Bahadır Yurtkulu on 17.02.2017.
//  Copyright © 2017 Bahadir. All rights reserved.
//

/*

This program is a simple image median filtering program.
It takes image matrix from given text file, read it and apply median filtering to it with the filter size which is decleared by user. 

*/


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "strutils.h"

using namespace std;




/////////////////////////////////////////////////////////////////

//This function creates vector to be smoothed later.
//It reads the numbers from the file from left to right and writes them to the matrix, 2D vector, one by one.

void writeToVector (ifstream& file, vector<vector<int>>& matrix) {
    string line, numberStr;
    int number;
    int rowNumber = 0;
    int columnNumber = 0;
    vector<int> newColumn; //insert this to the matrix to create a new column.
    
    while (getline(file, line)) {
        istringstream iss(line);
        matrix.push_back(newColumn);
        
        while (iss >> numberStr) {
            number = atoi(numberStr);
            
            matrix.at(rowNumber).push_back(number);
            
            columnNumber++;
        }
        rowNumber++;
        columnNumber = 0;

    }
}






/////////////////////////////////////////////////////////////////

// Checks the file if numbers inside of it forms a matrix or not.
// Checks if all rows has the same number of elements inside.
// Checks the numbers. Numbers must be in the range [0, 255]. Of course numbers must be number too :)
//

bool isMatrix(ifstream& file) {
    int firstLineColumnNumber = 0;
    int currentColumnNumber = 0;
    string line;
    string number;
    
    getline(file, line);
    istringstream iss(line);
    
    while (iss >> number) { // take the line and read it word by word (in our case number by number).
        firstLineColumnNumber++;
        
        if (atoi(number) < 0 || atoi(number) > 255) { // check the range [0, 255]
            cout << "This matrix contains invalid integer! Exiting..." << '\n';
            return false;
        }
        if (atoi(number) == 0) { // atoi returns '0' if string does not contain integer. To handle it checks every '0' returning element.
            for (int i = 0; i < number.length(); i++) {
                if(!(isdigit(number[i]))) { // isdigit() function is for checking if it is a number or not.
                    cout << "Matrix contains non-integer value! Exiting..." << '\n';
                    return false;
                }
                
            }
        }
        
    }
    
    file.clear();
    file.seekg(0);
    line = "";
    
    // Checks if it is in the matrix format or not.
    while(getline(file, line)) {
        istringstream iss(line);
        while (iss >> number) {
            currentColumnNumber++;
            
        }
        
        if (currentColumnNumber != firstLineColumnNumber) {
            cout << "This is not in a matrix format! Exiting..." << '\n';
            return false;
        }
        currentColumnNumber = 0;
    }
    return true;
}


///////////////////////////////////////////////////////////////////////

// This function applies madian filtering to the given matrix element.
// It checks if the median filtering process stays inside of the matrix.
// For example it will not include matrix[-3][-5] into the median filtering process.
// The function writes all the surrounding elements to a vector, medianList.
// Sorts the medianList.
// Calculates the median. Returns the median.
//

int calculateMedian (const vector<vector<int>> matrix, const int filterSize, const int rowIndex, const int columnIndex) {
    
    vector<int> medianList;
    int startRow = rowIndex - filterSize / 2;
    int startCol = columnIndex - filterSize / 2;
    int median;
    
    if (startRow < 0) {
        startRow = 0;
    }
    if (startCol < 0) {
        startCol = 0;
    }
    
    for (int i = 0; (i < filterSize) && ((startRow + i) < matrix.size()) && ((startRow + i) <= (rowIndex + (filterSize/2))); i++) {
        
        for (int j = 0; j < filterSize && (startCol + j) < matrix[0].size() && ((startCol + j) <= (columnIndex + (filterSize/2))); j++) {
            
            medianList.push_back(matrix[startRow + i][startCol + j]);
        }
    }
    
    sort(medianList.begin(), medianList.end());

    unsigned long sizeMedianList = medianList.size();
    
    if (sizeMedianList % 2 == 1) {
        
        median = medianList[(sizeMedianList/2)];
        
    }
    else {
        
        median = (medianList[(sizeMedianList/2) - 1] + medianList[(sizeMedianList/2)]) / 2;
    }
    
    return median;
}



/*================================
 ========= MAIN FUNCTION =========
 ================================*/

int main() {
    
    int filterSize;
    string filterSizeStr;
    string matrixFileName;
    ifstream matrixFile;
    vector<vector<int>> matrix;
    
    /////////////////////////////////////////////////////
    // User inputs the necessary info to run the program.
    // Such as filter size, input file name.
    //
    
    cout << "Welcome to the Image Filtering Program. \n\nPlease Enter a positive odd number as filter size: \n";
    cin >> filterSizeStr;
    filterSize = atoi(filterSizeStr);
    
    while (filterSize % 2 == 0 || filterSize < 0) {
        cout << "Come on! This is not a positive odd number :) Please enter an POSITIVE ODD number: \n";
        cin >> filterSizeStr;
        filterSize = atoi(filterSizeStr);
    }
    
    cout << "Great! We're good to go. Now please enter the valid file name which includes the matrix you want to smooth: \n";
    cin >> matrixFileName;
    matrixFile.open(matrixFileName);
    while (matrixFile.fail()) {
        cout << "Oops! We had an error while openning the file. Check the file name and try again.\n-->  ";
        matrixFile.clear();
        cin >> matrixFileName;
        matrixFile.open(matrixFileName);
    }
    
    if (!(isMatrix(matrixFile))) {
        return 0;
    }
    
    matrixFile.clear();
    matrixFile.seekg(0);
    
    writeToVector(matrixFile, matrix);
    
    matrixFile.close(); // matrix file is not necessary anymore.
    
    vector<vector<int>> copyMatrix = matrix; // Create a copy matrix to calculate every elements median independent from others.
    
    
    
    
    
    for (int i = 0; i < matrix.size(); i++) {
        
        for (int j = 0; j < matrix[0].size(); j++) {
            
            copyMatrix[i][j] = calculateMedian(matrix, filterSize, i, j);
        }
    }
    
    matrix = copyMatrix;
    
    cout << '\n';
    cout << "Ok! Here it is... \n\nFILTERED MATRIX" << '\n';
    cout << '\n';
    
    // Print the filtered matrix.
    
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
        cout << matrix[i][j] << " ";
        }
        cout << '\n';
    }
    cout << '\n';
    
    return 0;
}



