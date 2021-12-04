#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "sanitize.hpp"

using namespace std;

class TextGen {
private:
    map<string, vector<string>> markov_chain; //map usage is mandatory!
    map<string, vector<string>>::iterator it;

public:
    TextGen(const unsigned int seed);
    void readIn(const string & filename);
    bool isEndPunctuation(const char character);
    vector<string> getValues(const string & key);
    string generateSentence();
    map<string, vector<string>> get_markovChain();
};

//TextGen's constructor has been created for you
TextGen::TextGen(const unsigned int seed)
{
    srand(seed);//seeds our random number generator
}

//Create the rest of the functions below...
void TextGen::readIn(const string & filename)
{
    ifstream myfile;
    myfile.open(filename);
    string text1= "^";
    string text2;
    
    while(myfile >> text2)
    {
        sanitize(text2);
        
        if(isEndPunctuation(text2[text2.size() - 1]) == true)
        {
            string punctuation(1, text2[text2.size() - 1]);
            text2 = text2.substr(0, text2.size() - 1);
            markov_chain[text2].push_back(punctuation);
            markov_chain[punctuation].push_back("$");
            text1 = "^";
        }
        else
        {
            markov_chain[text1].push_back(text2);
            text1 = text2;
        }
    }
    myfile.close();
    
    //debug(markov_chain);
}


bool TextGen::isEndPunctuation(const char character)
{
    if(character == '.' || character == '?' || character == '!')
    {
        return true;
    }
    return false;
}


vector<string> TextGen::getValues(const string & key)
{
    return markov_chain.at(key);
}


string TextGen::generateSentence()
{
    string temp1 = "^";
    string temp2 = "";
    string sentence = "";
    
    while(temp2 != "$")
    {
        for(it = markov_chain.begin(); it != markov_chain.end(); it++ )
        {
            if (it->first == temp1)
            {
                temp2 =  it->second[rand() % it->second.size()];
                if(temp2 == "$")
                {
                    string punctuation(1, sentence[sentence.size() - 1]);
                    sentence = sentence.substr(0, sentence.size() - 1);
                    return sentence;
                }
                else
                {
                    sentence += temp2;
                    sentence += " ";
                    temp1 = temp2;
                }
            }
        }
    }
    return sentence;
}


map<string, vector<string>> TextGen::get_markovChain()
{
    return markov_chain;
}
