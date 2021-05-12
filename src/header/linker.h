#ifndef LINKER_H_
#define LINKER_H_
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

void beginLinking(ifstream &inFile, string outputFileName, int argNum);
void runLinking();
#endif