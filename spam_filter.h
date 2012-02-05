#ifndef _SPAM_FILTER__
#define _SPAM_FILTER__

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <dirent.h>  // for reading all files in directory

using namespace std;

class SpamFilter
{
    map<string, int> spam_;
    map<string, int> ham_;
    vector <string> spam_files_;
    vector <string> ham_files_;
    long long spam_words_;
    long long ham_words_;
    void splitLine(vector<string>&, const string& line);
    void polish(string& str);
    void populateMap(vector <string>& tokens, bool isSpam);
    bool naiveBayesClassifier(vector <string>& tokens);

public:
SpamFilter():spam_words_(0), ham_words_(0){}
    bool isSpam(const string& file_path);
    void classify();
    void readFiles(string directory, vector <string>& files);
    void gatherTrainingFiles();
};

#endif
