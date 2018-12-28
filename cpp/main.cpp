#include <bits/stdc++.h>
#include "bst.hpp"

using namespace std;
using namespace snowapril;

void dump_tree(auto iter) {
    if (iter) {
        cout << *iter << ' ';
        dump_tree(iter - 1);
        dump_tree(iter + 1);
    }
}

int get_op_code(const string& command);

int main(void) {
    binary_search_tree<int> tree;
    string command;

    while (true) {
        cout << "1. insert, 2. remove, 3. size, 4. dump, 5. exit" << endl;
        cin >> command;

        int op_code = get_op_code(command);
        switch(op_code) {
        case 0:
            cout << "enter the value : ";
            cin >> op_code;
            tree.append(op_code);
            break;
        case 1:
            cout << "enter the value : ";
            cin >> op_code;
            tree.remove(op_code);
            break;
        case 2:
            cout << "tree size : " << tree.size() << endl;
            break;
        case 3:
            dump_tree(tree.begin());
            cout << endl;
            break;
        case 4:
            cout << "bye bye." << endl;
            return 0;
        default:
            cout << "you enter the wrong command. try again." << endl;
            break;
        }
    }

    return 0;
}

int get_op_code(const string& command) {
    static vector<string> command_list = {"insert", "remove", "size", "dump", "exit"};
    
    int length;
    if (isdigit(command[0]))
        length = command[0] - '0' - 1;
    else {
        auto iter = find(command_list.begin(), command_list.end(), command);
        length = distance(command_list.begin(), iter);
    }
    
    return length;
}