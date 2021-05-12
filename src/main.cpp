/*
Author: Carlos Gabriel Vilas Novas Soares
*/
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include "./header/linker.h"
using std::cout;
using std::endl;
using std::ifstream;
using std::string;


void createExecutableFile(string finalCode, string fileName){
    cout << fileName <<endl;
    if(!fileName.empty()){
        fileName = fileName + ".out"; 
    }
    std::ofstream outputFile;
    outputFile.open(fileName);
    outputFile << finalCode;
    cout << finalCode <<endl;
    outputFile.close();
}
int main(int argc, char **argv) {
    string executableFileName;
    if(argc >= 2){
        for(int i =1; i < argc; i++){
            string fileName;
            string outputFileName;

            //Check initial input arguments 
        
            fileName = argv[i];
            // OpenFile
            ifstream inFile(fileName);
            if(!inFile.is_open()){
                cout << "Error opening file." << endl;
                cout <<"Tried to open: '" << fileName << "'." << endl;
            }
            // This will load everything into program memory
            beginLinking(inFile);
            executableFileName += fileName.substr(0, fileName.size() - 4);
            inFile.close();     
        }
        // Now that the files have been properly loaded , we can actually link everyhing
        
        createExecutableFile(runLinking(), executableFileName);
        
        
    
            
    }
    
    else{
        cout << "Invalid number of arguments." << endl;
        return 1;
    }

    return 0;
}