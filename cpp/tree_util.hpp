#ifndef TREE_UTIL_HPP
#define TREE_UTIL_HPP

#ifdef _TREE_LOG
#include <iostream>
#define LOG(msg, exp) { std::cout << msg << " ==> " << #exp << "(" << exp << ")" << std::endl; }
#else
#define LOG(msg, exp) 
#endif

#endif