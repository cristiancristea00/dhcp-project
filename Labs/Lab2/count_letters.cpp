#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    map<char, int> letters1, letters2;
    vector<string> words;
    double t1, t2;

    for (int i = 32; i < 128; i++) {
        letters1[(char)i] = 0;           //initialize vector
        letters2[(char)i] = 0;           //initialize vector
    }

    for (int i = 1; i <= 100; i++) {
        string filename = "files/f" + to_string(i);
        char content[10000];
        FILE *file;

        file = fopen(filename.c_str(),"r");
        while (fgets(content, sizeof(content), file) != NULL){
                string temp(content);
                words.push_back(string(content));
        }
        fclose(file);
    }

    t1 = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < words.size(); i++) {
        for (int j = 0; j < words[i].size(); j++) {
            #pragma omp atomic
            letters1[words[i][j]]++;
        }
    }
    t2 = omp_get_wtime();

    cout << "Time to count words version 1: " << t2-t1 << endl;

    const int CHAR_RANGE = 128;

    t1 = omp_get_wtime();
    #pragma omp parallel
    {
        int local_letters[128] = {0};

        #pragma omp for schedule(guided, 2)
        for (int i = 0; i < words.size(); i++) {
            for (char c : words[i]) {
                local_letters[(unsigned char)c]++;
            }
        }
        
            for (int k = 32; k < 128; k++) {
                #pragma omp atomic
                letters2[k] += local_letters[k];
            }
    }
    t2 = omp_get_wtime();

    cout << "Time to count words version 2: " << t2-t1 << endl;

    for (int i = 32; i < 128; i++) {
        cout << (char)i << ": " << letters1[char(i)] << endl;
    }

    cout << endl;
    cout << endl;
    cout << endl;

    for (int i = 32; i < 128; i++) {
        cout << (char)i << ": " << letters2[char(i)] << endl;
    }
}
