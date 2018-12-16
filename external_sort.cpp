//
//  main.cpp
//  external_sort
//
//  Created by Семен Федотов on 16/12/2018.
//  Copyright © 2018 Семен Федотов. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>


class ExternalSortRunner {
public:
    ExternalSortRunner(const std::string& inputFilename,
                 const std::string& fstTempFilename,
                 const std::string& sndTempFilename)
        : inputFilename(inputFilename), fstTempFilename(fstTempFilename), sndTempFilename(sndTempFilename) {};
    
    ~ExternalSortRunner() {
        remove(fstTempFilename.c_str());
        remove(sndTempFilename.c_str());
    };
    
    void run();
    
    long long GetLinesNumber();
    void MakeSplit(long long& currentLineNumber,
                   const long long& currentBatchSize);
    void MergeTwoFiles(const long long& currentBatchSize);
private:
    std::string inputFilename;
    std::string fstTempFilename;
    std::string sndTempFilename;
};

long long ExternalSortRunner::GetLinesNumber() {
    std::ifstream in(inputFilename);
    
    std::string tempLine;
    long long linesNumber = 0;
    while (in.peek() != EOF) {
        std::getline(in, tempLine);
        ++linesNumber;
    }
    
    in.close();
    
    return linesNumber;
}

void ExternalSortRunner::MakeSplit(long long& currentLineNumber,
                             const long long& currentBatchSize) {

    std::ifstream inputFile(inputFilename);
    
    std::ofstream fstTempFile(fstTempFilename);
    std::ofstream sndTempFile(sndTempFilename);
    
    std::string currentLine;
    while (inputFile.peek() != EOF) {
        bool isFirst = (currentLineNumber / currentBatchSize) % 2 == 0? true: false;
        
        std::getline(inputFile, currentLine);
        if (isFirst) {
            fstTempFile << currentLine << "\n";
        }
        else {
            sndTempFile << currentLine << "\n";
        }
        ++currentLineNumber;
    }
    inputFile.close();
    fstTempFile.close();
    sndTempFile.close();
}

void ExternalSortRunner::MergeTwoFiles(const long long& currentBatchSize) {
    std::ofstream inputFileOut(inputFilename);
    
    std::ifstream fstTempIn(fstTempFilename);
    std::ifstream sndTempIn(sndTempFilename);

    // getting first lines
    std::string fstLine, sndLine;
    if (fstTempIn.peek() != EOF) {
        std::getline(fstTempIn, fstLine);
    }
    if (sndTempIn.peek() != EOF) {
        std::getline(sndTempIn, sndLine);
    }
    while (!fstTempIn.eof() && !sndTempIn.eof()) {
        long long fstPosition = 0;
        long long sndPosition = 0;
        while (fstPosition < currentBatchSize && sndPosition < currentBatchSize && !fstTempIn.eof()  && !sndTempIn.eof()) {
            if (fstLine < sndLine) {
                inputFileOut << fstLine << "\n";
                std::getline(fstTempIn, fstLine);
                ++fstPosition;
            }
            else {
                inputFileOut << sndLine << "\n";
                std::getline(sndTempIn, sndLine);
                ++sndPosition;
            }
        }
        // Start writing remainder
        while (!fstTempIn.eof() && (fstPosition < currentBatchSize || sndTempIn.eof())) {
            inputFileOut << fstLine << "\n";
            std::getline(fstTempIn, fstLine);
            ++fstPosition;
        }
        while (!sndTempIn.eof()  && (sndPosition < currentBatchSize || fstTempIn.eof())) {
            inputFileOut << sndLine << "\n";
            std::getline(sndTempIn, sndLine);
            ++sndPosition;
        }
    }
    inputFileOut.close();
    fstTempIn.close();
    sndTempIn.close();
}


void ExternalSortRunner::run() {
    long long linesNumber = GetLinesNumber();
    
    std::string currentLine;
    long long currentBatchSize = 1;
    while (currentBatchSize < linesNumber) {
        long long currentLineNumber = 0;
        
        MakeSplit(currentLineNumber, currentBatchSize);
        
        MergeTwoFiles(currentBatchSize);
        
        currentBatchSize *= 2;
    }
}


int main(int argc, const char * argv[]) {
    
    std::string inputFilename = argv[1];  // must be set;
    std::string fstTempFilename = argv[2];  // must be set;
    std::string sndTempFilename = argv[3];  // must be set;
    
    std::string needTimePrinting = "0";
    if (argc == 5) {
        needTimePrinting = argv[4];
    }
    
    auto externalSortRunner = ExternalSortRunner(inputFilename, fstTempFilename, sndTempFilename);
    
    auto startTime = std::chrono::steady_clock::now();
    externalSortRunner.run();
    auto endTime = std::chrono::steady_clock::now();

    auto timeSpent = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() * 0.000001;
    if (needTimePrinting == "1") {
        std::cout << "Time spent in seconds: " <<  timeSpent << "\n";
    }
    return 0;
}
