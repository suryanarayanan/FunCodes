#include "spam_filter.h"

long double  min_double = 3.3621e-100L;

using namespace std;

/* read files in a folder */
void SpamFilter::readFiles(string directory, vector <string>& files)
{
    string str;
    DIR *pDIR;
    struct dirent *entry;
    if((pDIR=opendir(directory.c_str())))
    {
        while((entry = readdir(pDIR)))
        {
            string filename(entry->d_name);
            if(filename != "." && filename != "..")
            {
                files.push_back(directory+filename);
            }
        }
        closedir(pDIR);
    }
}

/* construct tokens from spam and ham files */
void SpamFilter::classify()
{
    ifstream ifs;
    string line;
    vector <string> spamTokens;
    vector <string> hamTokens;
    /* classify the spam contents and ham contents */
    for(string file : spam_files_)
    {
        ifs.open(file.c_str(),ios_base::in);
        /* read every single line from spam and tokenize */
        while(getline(ifs, line))
        {
            splitLine(spamTokens, line);
        }
        ifs.close();
    }
    for(string file : ham_files_)
    {
        ifs.open(file.c_str(),ios_base::in);
        /* read every line from ham and tokenize */
        while(getline(ifs, line))
        {
            splitLine(hamTokens, line);
        }
        ifs.close();
    }
    populateMap(spamTokens, true);
    populateMap(hamTokens, false);
    return;
}

/* just sand the string and make it a word */
void SpamFilter::polish(string& str)
{
    int len = int(str.length());
    char c;
    int start = -1;
    int end = -1;

    /* omit html tags */
    if(str[0] == '<' || str[0] == '(' || str[0] == '[')
    {
        str = "";
        return;
    }


    for(int i = 0; i < len; ++i)
    {
        c = str[i];
        if(!((c >= 'A' && c <= 'Z')|| (c >= 'a' && c <= 'z')))
            continue;
        else {start = i; break;}
    }
    for(int i = len -1 ; i >= start ; --i)
    {
        c = str[i];
        if(!((c >= 'A' && c <= 'Z')|| (c >= 'a' && c <= 'z')))
            continue;
        else {end = i; break;}
    }
    if(start != -1)
        str = str.substr(start, end - start +1);

    else
    {
        str = "";
        return;
    }
    /* omit common words */
    if(str == "a" || str == "the" || str == "this" || str == "in" || str == "in" || str == "and" || str == "for" || str =="From" || str == "Received"|| str == "from" || str == "by")
    {
        str = "";
        return;
    }
    return;
}

/* tokenizer */
void SpamFilter::splitLine(vector <string>& tokens, const string& line)
{
    string token;
    istringstream is(line);
    while(is>>token)
    {
        polish(token);
        if(token != "")
            tokens.push_back(token);
    }
}

/* construct spam and ham tokens in the table */
void SpamFilter::populateMap(vector <string>& tokens, bool isSpam)
{
    if(isSpam)
        spam_words_+= int(tokens.size());
    else
        ham_words_+= int(tokens.size());

    for(string token : tokens)
    {
        if(isSpam)
            spam_[token]++;
        else
            ham_[token]++;
    }
    return;
}

/* Simple Naive Bayes classifier */
bool SpamFilter::naiveBayesClassifier(vector <string>& tokens)
{
    long double spamNB = 1.L;
    long double hamNB = 1.L;
    long double spamProb = ((long double)(spam_words_)/(spam_words_ + ham_words_));
    long double hamProb = ((long double)(ham_words_)/(spam_words_ + ham_words_));
    for(string tok : tokens)
    {
        long double total = spam_[tok] + ham_[tok];
        if(spam_words_ && spam_[tok])
            spamNB *= ((long double)(spam_[tok])/spam_words_);
        else
            spamNB *= ((spam_[tok] + 1.)/(2. + spam_words_)); // m = 2, prior prob = .5
    }

    for(string tok : tokens)
    {
        long double total = spam_[tok] + ham_[tok];
        if(total && ham_[tok])
            hamNB *= ((long double)(ham_[tok])/ham_words_);
        else
            hamNB *= ((ham_[tok] + 1.)/(2. + ham_words_));
    }

    if(hamProb*hamNB < spamProb*spamNB)
        return true;
    return false;
}

/* classify whether the file is a spam or not */

bool SpamFilter::isSpam(const string& file_path)
{
    string line;
    ifstream ifs(file_path);
    vector <string> tokens;
    if(!ifs.good())
        cout<<"Invalid file"<<endl;
    while(ifs>>line)
    {
        splitLine(tokens, line);
    }
    return naiveBayesClassifier(tokens);
}

void SpamFilter::gatherTrainingFiles()
{
    readFiles("./spam/", spam_files_);
    readFiles("./ham/", ham_files_);
}

int main()
{
    string file_path;
    SpamFilter sf;
    /* read all the training files in spam folder and ham folder */
    cout<<"Gathering training data..."<<endl;
    sf.gatherTrainingFiles();
    cout<<"Classifying..."<<endl;
    /* construct the tables of spam tokens and ham tokens */
    sf.classify();
    cout<<"Test run..."<<endl;
    /* classify the validation set */
    vector <string> spam_files;
    vector <string> ham_files;
    sf.readFiles("./validation/spam/", spam_files);
    sf.readFiles("./validation/ham/", ham_files);

    int actual_spam_count = spam_files.size();
    int classified_spam_count = 0;
    int actual_ham_count = ham_files.size();
    int classified_ham_count = 0;

    for(string files : spam_files)
    {
        if(sf.isSpam(files))
            classified_spam_count++;
    }

    for(string files : ham_files)
    {
        if(!sf.isSpam(files))
            classified_ham_count++;
    }
    cout<<"spam mismatches : "<<actual_spam_count - classified_spam_count<<endl;
    cout<<"ham mismatches : "<<actual_ham_count - classified_ham_count<<endl;
    return 0;
}
