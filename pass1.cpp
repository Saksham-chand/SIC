#include<iostream>
#include<fstream>
#include<vector>
#include <iomanip>
#include <string>
#include<algorithm>
using namespace std;

void parseLine(string &line,string&label,string&instruction,string&operand)
{
    int l=line.length();
    string temp="";
    bool foundLabel=false;
    for(int i=0;i<l;i++)
    {
        if(line[i]==' '&&!foundLabel&&label.empty())
        {
            label=temp;
            temp="";
            foundLabel=true;
            continue;
        }
        else if(line[i]==' '&&instruction.empty())
        {
            instruction=temp;
            temp="";
            continue;
        }
        else if(line[i]!=' ')
        temp+=line[i];
    }
    operand=temp;
}

vector<pair<string,int>>pass1(ifstream &inputFile,int &e,int&start_add,int&loc_ctr)
{
    vector<pair<string,int>>symtab;
    string line;
    bool started=false;
    while(getline(inputFile,line))
    {
        string label="",instruction="",operand="";
        parseLine(line,label,instruction,operand);
        operand.erase(operand.find_last_not_of(" \n\r\t")+1);
        instruction.erase(instruction.find_last_not_of(" \n\r\t")+1);
        if(instruction=="END")
        break;
        if(instruction=="START")
        {
            start_add=stoi(operand,nullptr,16);
            loc_ctr=start_add;
            started=true;
            continue;
        }
        if(!started)
        {
            e=1;
            cout<<"No Starting of the Program\n";
            return symtab;
        }
        if(!label.empty())
        {
            if(find_if(symtab.begin(), symtab.end(),
                      [&label](const pair<string, int>& p) {
                          return p.first == label; // Condition to match
                      })!=symtab.end())
            {
                e=1;
                cout<<"Duplicate Values\n";
                return symtab;
            }
            else
            symtab.push_back({label,loc_ctr});
        }
        if(!(instruction=="WORD"||instruction=="RESW"||instruction=="RESB"||instruction=="BYTE"))
        loc_ctr+=3;
        else if(instruction=="RESW")
        loc_ctr+=3*stoi(operand);
        else if(instruction=="RESB")
        loc_ctr+=stoi(operand);
        else if(instruction=="WORD")
        loc_ctr+=3;
        else if(instruction=="BYTE")
        {
            if(operand[0]=='C')
            loc_ctr+=operand.length()-3;
            else if(operand[0]=='X')
            loc_ctr+=(operand.length()-3)/2;
        }
        else
        {
            e=1;
            cout<<"Invalid Instruction";
            return symtab;
        }
    }
    return symtab;
}

int main()
{
    ifstream inputFile("Input.txt");
    if(!inputFile)
    {
        cout<<"Error opening in File";
        return 1;
    }
    int e=0,start_add=0,length=0;
    vector<pair<string,int>>SYMTAB=pass1(inputFile,e,start_add,length);
    if(e)
    {
        return 1;
    }
    ofstream outFile("Symtab.txt");
    for(auto x:SYMTAB)
    {
        outFile<<x.first<<"\t"<<hex<<uppercase<<setw(4)<<setfill('0')<<x.second<<endl;
    }
    inputFile.close();
    return 0;
}