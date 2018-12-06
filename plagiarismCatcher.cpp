#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef struct StackNode {
    int index;
    struct StackNode *next;

} StackNode;

typedef struct StackLL {
    struct StackNode *top;
    StackLL(){
        top = NULL;
    }
    void push(int a){
        StackNode* add = new StackNode;
        add->index = a;
        add->next = top;
        top = add;
    }
} StackLL;

void update(vector< vector<int> > &matrix, int first, int second){
    if(first < second){
        matrix[first][second]++;
    }
    if (second < first){
        matrix[second][first]++;
    }
}

struct similars{
    int similarities;
    string file1;
    string file2;
};

bool compare(const similars &a, const similars &b)
{
    return a.similarities < b.similarities;
}

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
        files.push_back(dir + "/" + string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

//Takes in a vector of words, processes them, applies the hashing function, and edits the hash table
void hashFunction(vector<string> &key, StackLL hashTable[], int index){
    string wordEdit = "";
    for(int i = 0; i<key.size(); i++){
        wordEdit += key[i];
    }
    char x;
    unsigned long hash = 0;
    int multiplier = 1;
    for(int j= 0; j<wordEdit.length(); j++){
        hash += ((wordEdit.at(j))*(wordEdit.length()-j-1)*multiplier);
        multiplier = multiplier*37;
    }
    hash = hash%100000;
    hashTable[hash].push(index);
}

int main(int argc, char *argv[])
{
    int HASHLENGTH = 100000;
    //ordinarily would be a command prompt, keeping it simple for debugging, make sure you use the path for your own test folder
    string dir = argv[1];
    //number of words in a word chunk, also would normally be a command prompt
    int numWords = atoi(argv[2]);

    int numSimilarities = atoi(argv[3]);
    StackLL hashTable[100000];
    //each entry of this vector will hold the name of a file in the directory being processed
    vector<string> files = vector<string>();

    //fills the file vector with the names of the files in the directory
    getdir(dir,files);

    //iterates through each file name
    for(int i = 2; i < files.size(); i++){
        string currentFile = files[i]; //Holds the current file
        ifstream file; //input stream for the current file
        file.open(currentFile.c_str());
        if(!file.is_open()){
            cout << "File can not be accessed" <<endl;
        }
        string word; //will hold each word in the input file successively, to be placed into a vector
        word.clear(); //not totally sure why we need this tbh
        //bool flag = true; //
        vector<string> words; //vector that will hold a chunk of n words (decided by command prompt)
        for(int j = 0; j < numWords; j++){
            if(file >> word){ //checks if file has a next word
                words.push_back(word); //adds the word to the vector
            }
            else{
                j = numWords; //breaks out of the for loop
            }
        }
        if(words.size() == numWords){ //effectively checks if the input file has at least n words
            //if it doesn't, we don't have to do anything
            hashFunction(words, hashTable, i); //hashes the initial n words
            while(file >> word){ //loops while file has a next word and adds that word
                //if it does, remove the first word from the words vector, and hashes the remaining chunk
                words.push_back(word);
                words.erase(words.begin());
                hashFunction(words, hashTable, i);
            }
        }
    }
    vector<vector<int> > matrix(files.size(), vector<int>(files.size(), 0));
    for (int i = 0; i < HASHLENGTH; i++){ // filling in 2D matrix with number of similarities from hash table
        StackNode* curr = hashTable[i].top;
        if(curr != NULL){
            StackNode *second = curr->next;
            if(second != NULL){
                while(second != NULL){
                    while(second != NULL){
                        update(matrix, curr->index, second->index);
                        second = second->next;
                    }
                    curr = curr->next;
                    second = curr->next;
                }
            }
        }
    }

    vector<similars> arr; // vector of files that hae more similarities than specified in command line
    int iterator = 0;
    for(int i = 0; i < files.size(); i++){
        for(int j = i + 1; j < files.size(); j++){
            if (matrix[i][j] > numSimilarities) {
                arr.push_back(similars());
                arr[iterator].similarities = matrix[i][j];
                arr[iterator].file1 = files[i];
                arr[iterator].file2 = files[j];
                iterator++;
            }
        }
    }

    std::sort(arr.rbegin(), arr.rend(),compare); // sort vector of files that were supposed to be printed out
    for(int i = 0;i<iterator;i++){
        cout << arr[i].similarities << " " << arr[i].file1 << " " << arr[i].file2 << endl;
    } //print out sorted vector
    return 0;
}
