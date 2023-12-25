/*
Student No.: 110652019
Student Name: 林楷傑
Email: kjl0508.sc10@nycu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <openssl/sha.h>
#include <fstream>
#include <filesystem>
#include <unistd.h>
using namespace std;

unordered_map<string, filesystem::path> sha1_path;

string get_SHA1(string data){
    SHA_CTX c;
    unsigned char sha1[SHA_DIGEST_LENGTH];
    SHA1_Init(&c);
    SHA1_Update(&c, data.c_str(), data.size());
    SHA1_Final(sha1, &c);
    return string(reinterpret_cast<char*>(sha1));
}

void dfs_file(string path){
    if(filesystem::is_regular_file(path)){
        ifstream file(path, ios::binary);
        string file_contents((istreambuf_iterator<char>(file)),
                                 istreambuf_iterator<char>());
        string sha1_file = get_SHA1(file_contents);
        if(sha1_path.count(sha1_file)){
            filesystem::remove(path);
            filesystem::create_hard_link(sha1_path[sha1_file].c_str(), path.c_str());
        }
        else{
            sha1_path[sha1_file] = path;
        }
        return;
    }
    for (const auto& entry : filesystem::directory_iterator(path)) {
        dfs_file(entry.path());
    }
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    }
    string path = argv[1];
    dfs_file(path);
    return 0;
}