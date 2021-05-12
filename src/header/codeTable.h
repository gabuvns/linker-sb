#ifndef CODETABLE_H_
#define CODETABLE_H_

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::map;

enum LineType {
    header,
    relocation,
    definition,
    use,
    text
};

struct CategorizedLine {
    vector<string> lineContent;
    LineType lineType;
    CategorizedLine(vector<string> content, LineType lineType){
        this->lineContent = content;
        this->lineType = lineType;
    }
     CategorizedLine(){};
    
};

// ProgramTable are the individual files
struct ProgramTable{
    vector<CategorizedLine> categorizedLineList;
    int totalSize = 0;
    int textSize = 0;
    int dataSize = 0;
    int correctionFactor = 0;
};

struct Variable{
    string name;
    // original program counter
    int oPc = 0;
    // correction factor program counter
    int cfPc = 0;
} ;

struct CompleteCode{
    // program table are the files
    vector<ProgramTable> programTable;
    vector<Variable> globalDefinitionTable;
    vector<string> executableCode;
    // Each element will indicate the necessary correctionfFIl
    // For each program
};


#endif