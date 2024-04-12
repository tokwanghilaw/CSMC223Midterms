#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

int registers[8] = {0};
int memory[100];
int counter = 0;

int getRegisterIndex(const string& reg) {
    char lastChar = reg.back();
    int index = lastChar - '0';
    if (index < 0 || index > 7) {
        throw runtime_error("Invalid register name: " + reg);
    }
    return index;
}

void load(const string& reg, int value) {
    int index = getRegisterIndex(reg);
    registers[index] = value;
}

void store(int address, int value) {
    if (address < 0 || address >= 100) {
        throw runtime_error("Invalid memory address: " + to_string(address));
    }
    memory[address] = value;
}

void deletee(const string& reg) {
    int index = getRegisterIndex(reg);
    registers[index] = 0;
}

void add(const string& reg1, const string& reg2, const string& storeregister) {
    int index1 = getRegisterIndex(reg1);
    int index2 = getRegisterIndex(reg2);
    int storeIndex = getRegisterIndex(storeregister);
    registers[storeIndex] = registers[index1] + registers[index2];
}

void subtract(const string& reg1, const string& reg2, const string& storeregister) {
    int index1 = getRegisterIndex(reg1);
    int index2 = getRegisterIndex(reg2);
    int storeIndex = getRegisterIndex(storeregister);
    registers[storeIndex] = registers[index1] - registers[index2];
}

void multiply(const string& reg1, const string& reg2, const string& storeregister) {
    int index1 = getRegisterIndex(reg1);
    int index2 = getRegisterIndex(reg2);
    int storeIndex = getRegisterIndex(storeregister);
    registers[storeIndex] = registers[index1] * registers[index2];
}

void divide(const string& reg1, const string& reg2, const string& storeregister) {
    int index1 = getRegisterIndex(reg1);
    int index2 = getRegisterIndex(reg2);
    int storeIndex = getRegisterIndex(storeregister);
    if (registers[index2] == 0) {
        throw runtime_error("Division by zero");
    }
    registers[storeIndex] = registers[index1] / registers[index2];
}

void print(const string& reg, ostream& out) {
    int index = getRegisterIndex(reg);
    out << reg << ": " << registers[index] << endl;
}

int main() {
    try {
        ifstream infile("source.txt");
        if (!infile.is_open()) {
            throw runtime_error("Error opening file: source.txt");
        }

        vector<string> instructions;
        string line;
        while (getline(infile, line)) {
            instructions.push_back(line);
        }

        infile.close();

        counter = 0;
        ofstream outfile("output.txt");
        while (counter < instructions.size()) {
            stringstream ss(instructions[counter]);
            string opcode, operand1, operand2, storeregister;
            ss >> opcode;

            if (opcode == "LOAD") {
                ss >> operand1 >> operand2;
                load(operand1, stoi(operand2));
            } else if (opcode == "STORE"){
                ss >> operand1 >> operand2;
            } else if (opcode == "DEL"){
                ss >> operand1 >> operand2;
            } else if (opcode == "ADD") {
                ss >> operand1 >> operand2 >> storeregister;
                add(operand1, operand2, storeregister);
            } else if (opcode == "SUB") {
                ss >> operand1 >> operand2 >> storeregister;
                subtract(operand1, operand2, storeregister);
            } else if (opcode == "MUL") {
                ss >> operand1 >> operand2 >> storeregister;
                multiply(operand1, operand2, storeregister);
            } else if (opcode == "DIV") {
                ss >> operand1 >> operand2 >> storeregister;
                divide(operand1, operand2, storeregister);
            } else if (opcode == "PRINT") {
                ss >> operand1;
                print(operand1, outfile);
            } else {
                throw runtime_error("Invalid instruction: " + opcode);
            }
            counter++;
        }

        outfile.close();
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
