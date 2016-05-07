//
//  main.cpp
//  excuse_me
//
//  Created by Allison Kane on 5/6/16.
//  Copyright © 2016 Allison Kane. All rights reserved.
//

#include <iostream>
#include <stack>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

class myTreeNode
{
public:
    char Data;
    myTreeNode *childA;   //A's always go in child1
    myTreeNode *childT;   //T's always go in child2
    myTreeNode *childC;   //c's always go in child3
    myTreeNode *childG;   //G's always go in child4
    myTreeNode(char input)
    : Data (input)
    , childA(nullptr)       //nullifying childA, childT, childC, and childG
    , childT(nullptr)
    , childC(nullptr)
    , childG(nullptr)
    { }
    ~myTreeNode() {
        this->Data = ' ';
        if (this->childA) delete this->childA;      //the myTreeNode destructor will check to see if the child node exists and
        if (this->childT) delete this->childT;      //it will call the deconstructor on that child
        if (this->childC) delete this->childC;
        if (this->childG) delete this->childG;
    }
    
};

bool get_words_from_the_file(const char * my_file_name, vector<string> &vector_of_words);
bool get_the_reads_from_file(const char * my_file_name, vector<string> &reads);
bool write_out_the_vector_to_screen(vector<string> my_vector);
bool write_out_the_vector_to_file(vector<string> my_vector, char * my_file_name);

class myQuarttree
{
public:
    myTreeNode *Root;
    myQuarttree(){
        const char val = ' ';
        this->Root = this->CreateNode(val);
    }
    ~myQuarttree() {
        if (this->Root) delete Root;        //will recursively delete the nodes including the root node
    }
    
    
    myTreeNode * CreateNode(const char val)
    {
        char input = val;                   //copies the val into input
        return new myTreeNode(input);
    };
    
    myTreeNode * AppendNode(myTreeNode * parentNode, char val)
    {
        myTreeNode * temp = nullptr;            //creates a temp node for each child and puts them into a tree
        
        const char input = val;
        temp = CreateNode(input);
        
        switch(val)
        {
                
            case('a'):
                parentNode->childA = temp;
            case('t'):
                parentNode->childT = temp;
            case('c'):
                parentNode->childC = temp;
            case('g'):
                parentNode->childG = temp;
                
                break;
        }
        return parentNode;
    }
    bool CheckCharsInStr(string inStr)
    {
        bool flag = true;
        myTreeNode *currentNode = this->Root;                   //checks the characters in a string to put in tree
        for (int n = 0; n < 30; n++) {
            switch (inStr[n]) {
                case ('a'): // Adenine
                    (currentNode->childA && currentNode->childA->Data == inStr[n])
                    ? (currentNode = currentNode->childA) != 0 : flag = false;
                    break;
                case ('t'): // Thymine
                    (currentNode->childT && currentNode->childT->Data == inStr[n])
                    ? (currentNode = currentNode->childT) != 0 : (flag = false);
                    break;
                case ('c'): // Cytosine
                    (currentNode->childC && currentNode->childC->Data == inStr[n])
                    ? (currentNode = currentNode->childC) != 0 : (flag = false);
                    break;
                case ('g'): // Guanine
                    (currentNode->childG && currentNode->childG->Data == inStr[n])
                    ? (currentNode = currentNode->childG) != 0 : (flag = false);
                    break;
            } // End Switch
            if (!flag) break; // out of for loop
        }
        return flag;
    }
    
    vector<string> traverseTree(vector<string> input) {
        vector<string> temp;                  //creates a vector in order to traverse the tree
        for(auto i : input) {
            if(CheckCharsInStr(i)) {
                temp.push_back("Match Found!");
            } else {
                temp.push_back("Match not found!");
            }
        }
        return temp;
    }
    
};

int main()
{
    ofstream out;
    out.open("my_results.txt");
    myQuarttree tree;
    vector<string> words_in_genome;
    
    char genome_file_name [256];                 //= "my_genome.txt";
    
    
    if(!get_words_from_the_file(genome_file_name,words_in_genome))
    {
        cout << "Please enter the name of the file." << endl;
        cin.getline(genome_file_name, 256);
    }
    
    for (auto i : words_in_genome)
    {
        if ( i == words_in_genome[0])
        {
            if (tree.Root->Data == ' ' )
            {
                tree.Root->Data = (words_in_genome[0])[0];
            }
        }
        for (std::string::iterator it=i.begin(); it!=i.end(); ++it)
        {
            cout<< *it;
            cout<< '\n';
            tree.AppendNode(tree.CreateNode (*it), *it);
        }
    }
    
    vector<string> reads_to_be_tested;
    char reads_file_name [256];                 //make certain to place this file in the correct folder. Do not change path.
    if(!get_the_reads_from_file(reads_file_name,reads_to_be_tested))
    cout << "Please enter the name of the file." << endl;
    cin.getline(reads_file_name, 256);
   
    write_out_the_vector_to_file(tree.traverseTree(reads_to_be_tested), "my_results.txt");
    write_out_the_vector_to_file(tree.traverseTree(words_in_genome), "my_results.txt");
    out.close();
    //For each read, list its sequence and either "Yes" or "No".
    //This will indicate if it does or doesn't map to the genome.
    
}


bool get_words_from_the_file(char * my_file_name, vector<string> &vector_of_words)
{
    int i,j;
    int len=0;
    ifstream in;
    in.open(my_file_name);
    if(!in.is_open())
    {
        cout << "I could not find " << my_file_name << endl;
        cout << "Check the location.\n";
        return false;
    }
    
    char * my_word=new char [31];                              //this is a default, we'll be looking at words of size 30
    while(in.peek()!=EOF)
    {
        in>>my_word[0];
        len++;
    }                                                           //gets the length of the sequence
    in.clear(); in.close(); in.open(my_file_name);             //have to close and reopen file to reset filestream to beginning                     of file
    
    for(i=0; i<30; i++)
    {
        in>> my_word[i];
        if(my_word[i]<97) my_word[i]+=32;     //makes it lowercase
    }
    my_word[30]='\0';
    vector_of_words.push_back(my_word);
    
    for(i=1; i<(len-30-1); i++)   //read until the end of the file
    {
        //shift
        for(j=0; j<29; j++)
            my_word[j]=my_word[j+1];
            in>> my_word[29];
        if(my_word[29]<97)
            my_word[29]+=32;     //makes it lowercase
            my_word[30]='\0';
        cout << i << "\t" << my_word << endl; cout.flush();
        vector_of_words.push_back(my_word);
    }
    in.clear(); in.close();
    
    return true;
}
/*******************************************************************************
 This function takes the reads file name to read and reads each individual word
 in the file and stores each word in a vector.
 The vector is passed to this function as a parameter -- by reference such
 that the calling function has access to the vector of words.
 *******************************************************************************/
bool get_the_reads_from_file(char * my_file_name, vector<string> &reads, char type)
{
    int i;
    ifstream in;
    in.open(my_file_name);
    if(!in.is_open())
    {
        cout << "The read file " << my_file_name << " could not be opened.\nCheck the location.\n";
        return false;
    }
    
    char * word=new char [60];                              //this is a default, we'll be looking at words of size 10
    
    while(in.peek()!=EOF)
    {
        in.getline(word,60,'\n');
        for(i=0; i<30; i++)
        {
            if(word[i]<97)
                word[i]+=32;
        }     //makes it lowercase
        if (type == 'B' || type == 'b')
        {
            for (i = 0; i < 30; i++)
            {
                if (word[i] == 'a' || word[i] == 'g' || word[i] == 'r')
                    word[i] = 'r';                      // purine
                else
                    word[i] = 'y';                      // pyrimidine
            }
        }
        reads.push_back(word);
    }
    in.clear(); in.close();
    delete word;
    return true;
}

/*******************************************************************************
 This function writes out the vector of words to the screen. Written here to help
 you with debugging.
 *******************************************************************************/
bool write_out_the_vector_to_screen(vector<string> my_vector)
{
    int i;
    for(i=0; i<my_vector.size(); i++)
        cout << my_vector[i] << endl;
    return true;
}


/*******************************************************************************
 This function writes out the vector of words to file.
 *******************************************************************************/
bool write_out_the_vector_to_file(vector<string> my_vector, string * my_file_name)      //bool function that will create a file for the vector to write out to
{
    ofstream out;
    out.open("my_file_name");
    int i;
    for(i=0; i<my_vector.size(); i++)
        out << my_vector[i] << endl;
    out.clear();
    out.close();
    return true;
}
