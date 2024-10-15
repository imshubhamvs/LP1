#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

//MNT
class MNTNode
{
    string name;
    int pp;
    int kp;
    int kptab_pointer;
    int mdtp;
    public:
        MNTNode(string name, int pp, int kp, int kptab_pointer, int mdtp){
            this->name = name;
            this->kp = kp;
            this->pp = pp;
            this->kptab_pointer = kptab_pointer;
            this->mdtp = mdtp;
        }
    friend class MNT;
    friend class Pass2;
};

class MNT{
    vector<MNTNode> mnt_enteries;
    public:
    MNT(){}
    void add(MNTNode m){
        mnt_enteries.push_back(m);
    }
    MNTNode get(int index){
        return mnt_enteries[index];
    }
    int find(string name){
        for(int i = 0; i<mnt_enteries.size(); i++){
            if(mnt_enteries[i].name == name){
                return i;
            }
        }
        return -1;
    }
    int get_pointer(){
        return mnt_enteries.size();
    }
    void  print(){
        cout<<"name\tPP\tKP\tKPTABP\tMDTABP\n";
        for(MNTNode i : mnt_enteries){
            cout<<i.name<<"\t"<<i.pp<<"\t"<<i.kp<<"\t"<<i.kptab_pointer<<"\t"<<i.mdtp<<endl;
        }
    }
};

//KPTAB
class KPTabNode
{
    string name;
    string def;
    public:
        KPTabNode(string name, string def){
            this->name = name;
            this->def = def;
        }
        KPTabNode(){}
    friend class KPTab;
    friend class Pass2;
};
class KPTab
{
    vector<KPTabNode>kptab_entries;
    public:
        KPTab(){}
        void add(KPTabNode k){
            kptab_entries.push_back(k);
        }
        int find(string name){
            for(int i = 0; i < kptab_entries.size(); i++){
                if(kptab_entries[i].name == name){
                    return i;
                }
            }
            return -1;
        }
        KPTabNode get(int index){
            return kptab_entries[index];
        }
        int get_pointer(){
            return kptab_entries.size();
        }
        void  print(){
        cout<<"name\tdefaultValue\n";
        for(KPTabNode i : kptab_entries){
            cout<<i.name<<"\t"<<i.def<<endl;
        }
        }
};

//MDT
class MDT
{
    vector<string> mdtentries;
    public:
        MDT(){}
        void add(string c){
            mdtentries.push_back(c);
        }
        int get_pointer(){
            return mdtentries.size();
        }
        void  print(){
        for(string i : mdtentries){
            cout<<i<<endl;
        }
        }
        string get(int index){
            return mdtentries[index];
        }
};

class Pass2{
    MNT mnt = MNT();
    MDT mdt = MDT();
    KPTab kptab = KPTab();
    void callprocessor(istringstream &stream, MNTNode mn){
        vector<string> aptab;
        string ap_val;
        for(int i = 0; i<mn.pp; i++){
            stream>>ap_val;
            aptab.push_back(ap_val);
        }
        int kpstart = mn.kptab_pointer;
        string args;
        map<string, string>kps;
        while(stream>>args){
            ap_val = "";
            string kp = "";
            bool flag = false;
            for(char i : args){
                if(!flag)
                {
                    if(i == '='){
                    flag = true;
                    }
                    else{
                        kp += i;
                    }
                }
                else{
                    ap_val += i;
                }
            }
            kps[kp] = ap_val;
        }
        if(mn.kptab_pointer != -1)
        {
            KPTabNode node;
        for(int i = mn.kptab_pointer-1; i<(mn.kptab_pointer+mn.kp-1); i++){
            node = kptab.get(i);
            if(kps[node.name] != ""){
                aptab.push_back(kps[node.name]);
            }
            else{
                aptab.push_back(node.def);
            }
        }}
        string line;
        string code = "";
        string word;
        for(int i = mn.mdtp-1;;i++){
            code = "+ ";
            line = mdt.get(i);
            if(line == "MEND "){
                break;
            }
            else{
                istringstream stream(line);
                while(stream>>word){
                    if(word.substr(0, 2) == "(P"){
                        string val = word.substr(3,1);
                        int temp = stoi(val);
                        code += aptab[temp-1];
                        code += " ";
                    }
                    else{
                        code += word + " ";
                    }
                }
            }
            cout<<code<<endl;
        }
    }
    void pass2procesor(string input){
        fstream file;
        file.open(input, ios::in);
        string line;
        while(getline(file, line)){
            istringstream stream(line);
            string name;
            stream>>name;
            int index = mnt.find(name);
            if(index == -1){
                cout<<name<<" not found in mnt\n";
                return;
            }
            callprocessor(stream, mnt.get(index));
        }
    }
    public:
        Pass2(string filename){
            fstream file; 
            file.open(filename, ios::in);
            //initializing 
            string line;
            getline(file, line);
            istringstream stream(line);
            string word;
            while(stream>>word){
                string name = word;
                stream>>word;
                int pp = stoi(word);
                stream>>word;
                int kp = stoi(word);
                stream>>word;
                int mntp = stoi(word);
                stream>>word;
                int kpdtp = stoi(word);
                mnt.add(MNTNode(name, pp, kp, kpdtp, mntp));
            }
            getline(file, line);
            istringstream ss(line);

            while(ss>>word){
                string name = word;
                ss>>word;
                string def = word;
                kptab.add(KPTabNode(name, def));
            }
            while(getline(file, line)){
                mdt.add(line);
            }
            pass2procesor("input.txt");
        }
};

int main(){
    Pass2 p = Pass2("mnt.txt");
    
}