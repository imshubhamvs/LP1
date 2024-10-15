#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
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
    friend class KPTab;
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
};

class MacroPass1
{
    MNT mnt = MNT();
    KPTab kptab = KPTab();
    MDT mdt = MDT();
    vector<string> pntab;
    void macroprocessor(istringstream &stream, fstream &f){
        //processing definition
        string word;
        string line;
        string name;
        int pp = 0;
        int kp = 0;
        int kptabpointer = kptab.get_pointer();
        stream>>name;
        bool isKP = false;
        string parameter_name = "";
        string def = "";
        
        while(stream>>word){
            parameter_name = "";
            def = "";
            for(char i : word){
                if(!isKP){
                    if(i == '='){
                    isKP = true;
                    }
                    else{
                        parameter_name += i;
                    }
                }
                else{
                    def += i;
                }
            }
            if(isKP){
                kptab.add(KPTabNode(parameter_name, def));
                kp++;
            }
            else{
                pp++;
            }
            pntab.push_back(parameter_name);
        }
        MNTNode new_mnt_node = MNTNode(name, pp, kp, kptabpointer, mdt.get_pointer());
        mnt.add(new_mnt_node);
        //adding to mnt
        string codeLine = "";
        bool flag = false;
        while(getline(f, line)){
            istringstream stream(line);

            while(stream>>word){
                if(word[0] == '&'){
                    flag = false;
                    for(int i = 0; i<pntab.size(); i++){
                        if(pntab[i] == word){
                            codeLine += "(P,";
                            codeLine += to_string(i);
                            codeLine += ")";
                            flag = true;
                            break;
                        }
                    }
                    if(!flag){
                        codeLine += word;
                    }
                }
                else if(word == "MEND"){
                    mdt.add(word);
                    return;
                }
                else{
                    codeLine += word;
                }
            }
                mdt.add(codeLine);
        }
    }
    public:
        MacroPass1(string input){
            fstream file;
            file.open(input, ios::in);
            string line;
            string word;
            while(getline(file, line)){
                istringstream stream(line);
                while(stream>>word){
                    if(word == "MACRO"){
                        macroprocessor(stream, file);
                    }
                }
            }
            cout<<"---MNT---\n";
            mnt.print();
            cout<<"---KPTAB---\n";
            kptab.print();
            cout<<"---MDT---\n";
            mdt.print();
        }

};

int main(){
    MacroPass1 m = MacroPass1("input.txt");
    
    return 0;
}
