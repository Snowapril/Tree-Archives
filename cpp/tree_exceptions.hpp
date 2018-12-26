#ifndef TREE_EXCEPTIONS_HPP
#define TREE_EXCEPTIONS_HPP

#include <stdexcept>

namespace snowapril {
    class different_tree_exception : public std::runtime_error {
    public:
        explicit different_tree_exception(const std::string& _what_arg) : std::runtime_error(_what_arg) {};
        explicit different_tree_exception(const char *_what_arg)        : std::runtime_error(_what_arg) {};
        virtual ~different_tree_exception() throw() {};
    };
}

#endif