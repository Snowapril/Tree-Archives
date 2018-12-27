#include <bits/stdc++.h>
#include "bst.hpp"

using namespace std;
using namespace snowapril;

void dump_tree(auto iter) {
    if (iter) cout << *iter << endl;
    dump_tree(iter - 1);
    dump_tree(iter + 1);
}

int main(void) {
    vector<int> vec = {1, 3, 10, 31};
    binary_search_tree<int> tree(vec.begin(), vec.end());
    
    tree.remove(10);
    tree.remove(1);
    //tree.remove(3);
    return 0;
}