#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

//Takes in a vector of words, processes them, applies the hashing function, and edits the hash table
void hashFunction(vector<string> &key, vector<vector <int>> hashTable){
    //Just leaving this blank for now, we can change it later
}

int main()
{
    //ordinarily would be a command prompt, keeping it simple for debugging, make sure you use the path for your own test folder
    string dir = "Insert string here";
    //number of words in a word chunk, also would normally be a command prompt
    int numWords = 5;

    vector<vector <int>> hashTable = vector<vector<int>>();
    //each entry of this vector will hold the name of a file in the directory being processed
    vector<string> files = vector<string>();

    //fills the file vector with the names of the files in the directory
    getdir(dir,files);

    //prints the inex of each file in the file vector along with the file name
    for (unsigned int i = 0;i < files.size();i++) {
        cout << i << files[i] << endl;
    }

    //iterates through each file name
    for(int i = 2; i < files.size(); i++){
        string currentFile = files[i]; //Holds the current file
        ifstream file; //input stream for the current file
        file.open(currentFile);
        string word;
        char x;
        word.clear();
        bool flag = true;
        vector<string> words;
        for(int j = 0; j < numWords; j++){
            if(file >> word){
                words.push_back(word);
            }
            else{
                j = numWords;
            }
        }
        if(words.size() == numWords){
            hashFunction(numWords, hashTable);
        }
    }
    return 0;
}
