#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Opcode
{
public:
    string type;
    int code;
    Opcode(string t, int c) : type(t), code(c) {}
};

class OPTAB
{
public:
    map<string, Opcode *> mp;

    OPTAB()
    {
        mp["STOP"] = new Opcode("IS", 0);
        mp["ADD"] = new Opcode("IS", 1);
        mp["SUB"] = new Opcode("IS", 2);
        mp["MULT"] = new Opcode("IS", 3);
        mp["MOVER"] = new Opcode("IS", 4);
        mp["MOVEM"] = new Opcode("IS", 5);
        mp["COMP"] = new Opcode("IS", 6);
        mp["BC"] = new Opcode("IS", 7);
        mp["DIV"] = new Opcode("IS", 8);
        mp["READ"] = new Opcode("IS", 9);
        mp["PRINT"] = new Opcode("IS", 10);
        //
        mp["START"] = new Opcode("AD", 1);
        mp["END"] = new Opcode("AD", 2);
        mp["ORIGIN"] = new Opcode("AD", 3);
        mp["EQU"] = new Opcode("AD", 4);
        mp["LTORG"] = new Opcode("AD", 5);
        //
        mp["DC"] = new Opcode("DL", 1);
        mp["DS"] = new Opcode("DL", 2);
    }

    ~OPTAB()
    {
        for (auto &entry : mp)
        {
            delete entry.second;
        }
    }
};

class SymbolTable
{
public:
    map<string, int> symtab;
    map<string, int> symIndex;
    int symCounter = 1; // Counter to assign symbol numbers

    void insert(const string &label, int address)
    {
        if (!exists(label))
        {
            symtab[label] = address;
            symIndex[label] = symCounter++;
        }
        else
        {
            symtab[label] = address; // Update address if already exists
        }
    }

    bool exists(const string &label)
    {
        return symtab.find(label) != symtab.end();
    }

    int getAddress(const string &label)
    {
        return symtab[label];
    }

    int getSymbolNumber(const string &label)
    {
        return symIndex[label];
    }

    void print()
    {
        for (const auto &entry : symtab)
        {
            cout << entry.first << " -> " << entry.second << endl;
        }
    }
};

class PASS1
{
public:
    OPTAB optab;
    SymbolTable symtab;
    map<string, int> regTab;

    PASS1()
    {
        regTab["AREG"] = 1;
        regTab["BREG"] = 2;
        regTab["CREG"] = 3;
    }

    fstream f;

    void getPass1()
    {
        f.open("input.txt", ios::in);
        if (!f.is_open())
        {
            cerr << "Error opening file" << endl;
            return;
        }

        string line;
        int locationCounter = 0;

        while (getline(f, line))
        {
            istringstream iss(line);
            string label, opcode, operand1, operand2;

            // Read the first word and check if it's an opcode
            iss >> label;
            if (optab.mp.count(label) > 0)
            {
                // If the first word is an opcode, treat it as such and shift the rest
                opcode = label;
                label = "";
                iss >> operand1 >> operand2;
            }
            else
            {
                // Otherwise, treat it as a label, then read the opcode and operands
                iss >> opcode >> operand1 >> operand2;
            }

            // Handle label
            if (!label.empty() && opcode != "START")
            {
                if (!symtab.exists(label))
                {
                    symtab.insert(label, locationCounter);
                }
                else
                {
                    // Update address if it was a forward reference
                    if (symtab.getAddress(label) == -1)
                    {
                        symtab.insert(label, locationCounter);
                    }
                }
            }

            // Handle opcode
            if (opcode == "START")
            {
                // Handle START directive
                locationCounter = stoi(operand1);
                cout << "(AD, 1)\t(C, " << operand1 << ")\n";
            }
            else if (opcode == "END")
            {
                // Handle END directive
                cout << "(AD, 2)\n";
                break;
            }
            else if (optab.mp.count(opcode) > 0)
            {
                Opcode *op = optab.mp[opcode];
                cout << "(" << op->type << ", " << op->code << ")\t";

                if (opcode == "DC")
                {
                    // Handle DC
                    // Operand1 is constant
                    cout << "(C, " << operand1 << ")\n";
                    locationCounter += 1;
                }
                else if (opcode == "DS")
                {
                    // Handle DS
                    cout << "(C, " << operand1 << ")\n";
                    locationCounter += stoi(operand1);
                }
                else
                {
                    // For other opcodes
                    locationCounter += 1;

                    // Handle operand1
                    if (!operand1.empty())
                    {
                        if (regTab.count(operand1) > 0)
                        {
                            // Operand1 is a register
                            cout << "(" << regTab[operand1] << ")\t";
                        }
                        else
                        {
                            // Operand1 is a symbol
                            if (!symtab.exists(operand1))
                            {
                                symtab.insert(operand1, -1);
                            }
                            cout << "(S, " << symtab.getSymbolNumber(operand1) << ")\t";
                        }
                    }

                    // Handle operand2
                    if (!operand2.empty())
                    {
                        if (regTab.count(operand2) > 0)
                        {
                            // Operand2 is a register
                            cout << "(" << regTab[operand2] << ")";
                        }
                        else
                        {
                            // Operand2 is a symbol
                            if (!symtab.exists(operand2))
                            {
                                symtab.insert(operand2, -1);
                            }
                            cout << "(S, " << symtab.getSymbolNumber(operand2) << ")\t";
                        }
                    }
                    cout << endl;
                }
            }
            else
            {
                // Handle undefined opcode
                cerr << "Error: Unknown opcode " << opcode << endl;
            }
        }

        f.close();
        cout << "Symbol Table:\n";
        symtab.print();
    }
};

int main()
{
    PASS1 pass1;
    pass1.getPass1();
    return 0;
}