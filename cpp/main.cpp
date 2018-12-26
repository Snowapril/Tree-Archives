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
    binary_search_tree<int> tree { 1, 3, 10, 31};

    dump_tree(tree.begin());
    return 0;
}