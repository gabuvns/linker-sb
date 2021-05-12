#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <sstream> 
#include <vector>
#include <algorithm>
#include <map>
#include "./header/linker.h"
#include "./header/codeTable.h"

using std::cout;
using std::endl;
using std::stoi;
using std::ifstream;
using std::string;
using std::vector;
using std::map;


// These trim functions and only them, are not mine and were originally avaiable at:
// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring 
// Access: 8/03/2021
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}
// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
    ltrim(s);
    rtrim(s);
    
}
////////////////////////////////////////////////////////////////////////////////////
void printStr(vector<string> aux){for(auto i : aux){cout << i << " ";}cout << endl;}


// Every file will be loaded in memory
CompleteCode completeCode;
void printGlobalDefinitionTable(){
    for(auto i : completeCode.globalDefinitionTable){
        
        cout << "PUSHED ITEM: " << i.name <<endl;
        cout << "original pc: " << i.oPc << endl;
        cout << "variable pc: " << i.cfPc << endl;
        cout <<endl;
    }
    
}
LineType findLineType(vector<string> tokenizedLine){
    string auxStr = "";
    auxStr = tokenizedLine.at(0);
    
    LineType auxLineType;
    // header
    if(auxStr == "H:"){
        auxLineType = header;
    }
    // relocation bits
    else if(auxStr == "R:"){
        auxLineType = relocation;
    }
    // definition
    else if(auxStr == "D:"){
        auxLineType = definition;
    }
    // use
    else if(auxStr == "U:"){
        auxLineType = use;
    }
    // text
    else if(auxStr == "T:"){
        auxLineType = text;
        
    }
    return auxLineType;
    
}
CategorizedLine composeCategorizedLine(vector<string> tokenizedLine){
    CategorizedLine categorizedLine;
    categorizedLine.lineContent = tokenizedLine;
    categorizedLine.lineType = findLineType(tokenizedLine);
    return categorizedLine;
}
// First we tokenize the line
vector<string> tokenizeLine(string readLine){
    trim(readLine);
    vector<string> auxStr;
    string aux = "";
    int sizeCounter = 0;    
    for(auto i : readLine){
        if((i == ' ' || i=='\n')){
            auxStr.push_back(aux);
            aux.clear();
        }
        else if(sizeCounter == readLine.size() - 1 ){
            aux+=i;
            auxStr.push_back(aux);
            aux.clear();
        }
        else{
            aux+=i;
        }
        sizeCounter++;
    }
    
    return auxStr;
}

int getTotalSize(ProgramTable auxProgramTable){
    // We know size will always be the second element on the header, the third element is text only size, from there we can get where data begins
    return stoi(auxProgramTable.categorizedLineList.at(1).lineContent.at(1));
}

// Adds correctionFactor to completeCode based on each file size;
int getCorrectionFactor(){
    int cf = 0;
    // If not empty indicates it is not the first code
    if(!completeCode.programTable.empty()){
        // We set cf as 1 because we want to get the next avaiable address
        cf = 0;
        for(auto i : completeCode.programTable){
            cf+=i.totalSize;   
        }
    }
    return cf;
}
// Creates basic structure for each file || Loads everyhing into memory
void composeProgramTable(ifstream &inFile){
    ProgramTable auxProgramTable;
    while(!inFile.eof()){
        string readLine;
        getline(inFile,readLine,'\n');
        try{
            CategorizedLine categorizedLine;
            vector<string> tokenizedLine = tokenizeLine(readLine);
            if(!tokenizedLine.empty()){
                categorizedLine = composeCategorizedLine(tokenizedLine);
                auxProgramTable.categorizedLineList.push_back(categorizedLine);
            }
            
        }
        catch (int error){
            cout << "Program ended with error.\n";
        }
        
    }
    int totalSize = getTotalSize(auxProgramTable); 
    
    auxProgramTable.totalSize = totalSize;   
    
    // We sum 1 because it must indicate the next address of the code
    auxProgramTable.correctionFactor = getCorrectionFactor();
    completeCode.programTable.push_back(auxProgramTable);  
}

string removeStrInvalidChar(string auxStr){
    while(((int) auxStr.back() < 48 && (int) auxStr.back() > 57) && (int)auxStr.back() != 43){
        auxStr.pop_back();
    }
    return auxStr;
}
void composeGlobalDefinitionTable(){
    for(auto &j : completeCode.programTable.back().categorizedLineList){
        Variable auxVar;
        if(j.lineType == definition){
            
            auxVar.name = j.lineContent.at(1);
            auxVar.oPc = stoi(j.lineContent.back());
            auxVar.cfPc = auxVar.oPc + completeCode.programTable.back().correctionFactor;
        
            completeCode.globalDefinitionTable.push_back(auxVar);
        }
    }
}

void beginLinking(ifstream &inFile){
    composeProgramTable(inFile);
    composeGlobalDefinitionTable();
    
    cout << "Linking Complete" <<endl;
} 


bool isNumber(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

void changeUseAddress(vector<string> use, vector<ExecutableCode> &executableCode, int whichFile){
    string auxVarName;
    if(use.size() == 3){
        auxVarName = use.at(1);  
        // We get where the code is used
        // Removes "+"
        string auxStr = use.back().substr(0, use.size()-1);

        int useAddress = stoi(auxStr) + completeCode.programTable.at(whichFile).correctionFactor;
        for(auto i : completeCode.globalDefinitionTable){
            if(auxVarName == i.name){
                // We do this so we don't replicate
                executableCode.at(useAddress).hasBeenModified = 1;
                executableCode.at(useAddress).text = std::to_string(i.cfPc);
                break;
            }
        }
    }
    else{
        cout << "Oh no, something that should never have happenned, happened, contact code author aka github.com/gabuvns\n" <<endl;
    }
    
}

void changeAddress(string relBits, vector<ExecutableCode> &executableCode, int whichFile){
    // Safety measure
    for(int i = 0; i < relBits.size(); i++){
        // If 1 then it is relative address
        int dontChangeAgain =0;
        int tmpCf = completeCode.programTable.at(whichFile).correctionFactor;
        if(relBits.at(i) == '1' && !executableCode.at(i+tmpCf).hasBeenModified){
                int auxInt =stoi(executableCode.at(i+tmpCf).text); 
                auxInt+=tmpCf;
                executableCode.at(i+tmpCf).text = std::to_string(auxInt);
            
            
        }
    }
    
}
string runLinking() {
    // Iterate trough uses 
    vector<ExecutableCode> executableCode;
    for(auto  &i : completeCode.programTable){
        for(auto  &j : i.categorizedLineList){
            if(j.lineType == text){
                for(auto &k : j.lineContent){
                    if(isNumber(k)){
                        ExecutableCode aux;
                        aux.text = k;
                        executableCode.push_back(aux);                        
                    }
                }          
            }
        }
    }
    int whichFile =0;
    for(auto  &i : completeCode.programTable){
        for(auto  &j : i.categorizedLineList){
            if(j.lineType == use){
                // Send the x+ where the variable is used
                changeUseAddress(j.lineContent, executableCode, whichFile);
            }
        }
        whichFile++;
    }
    whichFile = 0;
    for(auto  &i : completeCode.programTable){
        for(auto &j :i.categorizedLineList){
            if(j.lineType == relocation){
                changeAddress(j.lineContent.at(1), executableCode, whichFile);
            }
        }
        whichFile++;
    }
    // Finally create the outpput string
    string outputCode;
    int sizeCounter = 0;
    for(auto i : executableCode){
        if(sizeCounter != executableCode.size()-1){
            outputCode+= i.text;
            outputCode+= " ";
        }
        else{
            outputCode+=i.text;
            
        }
        sizeCounter++;
        
    }
    return outputCode;
}