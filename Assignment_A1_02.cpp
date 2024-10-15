#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Command{
    private:
    string type;
    string id;
    string op1;
    string op2;
    string lc;
    public:
        Command(string lc, string type, string id, string op1, string op2){
        this->type = type;
        this->id = id;
        this->op1 = op1;
        this->op2 = op2;
        this->lc = lc;
    }
    friend class Pass2;
};
class Pass2{
    fstream f;
    vector<Command> cmd;
    vector<string>symtab;
    vector<string>littab;
    public:
    Pass2(string input, string tabs){

        string line;
        string word;
        f.open(tabs, ios::in);
        getline(f, line);
        istringstream s(line);
        while(s>>word){
            symtab.push_back(word);
        }
        getline(f, line);
        istringstream ss(line);
        while(ss>>word){
            littab.push_back(word);
        }
        f.close();
        int i = 0;
        string lc = "";
        string type = "";
        string id = "";
        string op1 = "";
        string op2 = "";
        f.open(input, ios::in);
        bool execution = true;
        while(getline(f, line)){
            istringstream stream(line);
            if(line.substr(0, 3) == "---"){
                continue;
            }
            i=0;
            while(stream>>word){
                switch(i){
                    case 0:
                        lc = word;
                        i++;
                        break;
                    case 1:
                        type = word.substr(1,2);
                        id = word.substr(4, 2);
                        if(type == "IS" && id == "00"){
                            execution = false;
                        }
                        i++;
                        break;
                    case 2:
                        if(word.length() == 3){
                            op1 = word[1];
                        }
                        else if(word[1] == 'S'){
                            op1 = symtab[stoi(word.substr(3,2))-1];
                        }
                        else if(word[1] == 'L'){
                            op1 = littab[stoi(word.substr(3,2))-1];
                        }
                        else if(word[1] == 'C'){
                            op1 = word.substr(3,2);
                        }
                        i++;
                        break;
                    case 3:
                        if(word.length() == 3){
                            op2 = word[1];
                        }
                        else if(word[1] == 'S'){
                            op2 = symtab[stoi(word.substr(3,2))-1];
                        }
                        else if(word[1] == 'L'){
                            op2 = littab[stoi(word.substr(3,2))-1];
                        }
                        else if(word[1] == 'C'){
                            op2 = word.substr(3,2);
                        }
                        break;
                }
            }
                Command c = Command(lc, type, id, op1, op2);
                cmd.push_back(c);
                if(!execution){
                    break;
                }
        cout<<lc<<type<<id<<op1<<op2<<endl;
        }
    }

    void dispPass2(){
        for(Command c: cmd){
            if(c.type == "IS"){
                cout<<c.lc<<" + "<<c.id<<" "<<c.op1<<" "<<c.op2<<endl;
            }
        }
    }
    
};

int main(){
    Pass2 p = Pass2("input.txt", "tab.txt");
    p.dispPass2();
}