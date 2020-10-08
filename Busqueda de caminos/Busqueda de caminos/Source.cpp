#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    string line;
    

    vector < int > v;
    ifstream fin;

    fin.open(("example.txt"));

    if (fin.is_open()) {
        while (getline(fin, line)) {
            stringstream ss(line);
            while (getline(ss, line, ',')) {
                v.push_back(stoi(line));
                
            }
        }

        int i = 0;
        
        for (int i = 0; i < v.size(); i++) {
            cout << v.at(i) ;
        }
        
        
    }
}