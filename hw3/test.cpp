#include <iostream>
#include <algorithm>
#include <sys/syscall.h>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]){
    int mode = 1;
    if (argc == 2) mode = atoi(argv[1]);
    if(mode){
        cout << "n = ";
        int n; cin >> n;
        ofstream out;
        out.open("input.txt");
        out << n << " \n";
        for(int i=0;i<n;i++){
            out << (rand()%1000) << ' ';
        }
        out.close();
    }
    else{
        string output = "output_1.txt";
        ifstream in;
        for(int j=1;j<=8;j++){
            output[7] = char('0'+j);
            in.open(output);
            vector<int> a;
            int x;
            while(in>>x){
                a.push_back(x);
            }
            vector<int> b=a;
            sort(a.begin(), a.end());
            for(int i=0;i<a.size();i++){
                if(a[i] != b[i]){
                    cout << output << "Wrong!" << endl;
                    return 0;
                }
            }
            cout << output << " Good!" << endl;
            in.close();
        }
    }
    return 0;
}