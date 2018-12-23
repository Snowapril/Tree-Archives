#ifndef BST_HPP
#define BST_HPP

#include <initializer_list>
#include <allocators>

namespace snowapril {

    template <typename Type>
    class bst_node_ {
    public:
        bst_node_(); // default constructor
        bst_node_(const Type&); // constructor with l-value data
        bst_node_(const Type&&); // constructor with r-value data
        bst_node_(bst_node<Type>&&) // move constructor
        bst_node_<Type> & operator=(bst_node_<Type>&&); // move assignment operator
        bst_node_(const bst_node_<Type>&); // copy constructor 
        bst_node_<Type> & operator=(const bst_node_<Type>&); // copy assignment operator
        ~bst_node_(); // destructor
    public:
        bst_node_<Type> *parent
        bst_node_<Type> *left_node, *right_node;
        Type value;
    };

    template <typename Type, class node_allocator = std::allocator< bst_node_< Type > > >
    class binary_search_tree {
    protected:
        bst_node_<Type> node_type;
    public:
        using value_type = Type;
        class iterator_base; 
        class downside_iterator;

        binary_search_tree(); // default constructor
        binary_search_tree(iterator_base const &); // constructor with one iterator
        binary_search_tree(std::iterator<Type> const &, std::iterator<Type> const &); //constructor with two standard iterators
        binary_search_tree(initializer_list<Type> const &); // constructor with l-value initializer_list
        binary_search_tree(initializer_list<Type>&&); // constructor with r-value initializer_list
        binary_search_tree(Type const *, Type const *); // constructor with two raw pointers
        binary_search_tree(binary_search_tree<Type, node_allocator> const &); // copy constructor 
        binary_search_tree<Type, node_allocator> & operator=(binary_search_tree<Type, node_allocator> const &); // copy assignment operator
        binary_search_tree(binary_search_tree<Type, node_allocator> &&); // move constructor
        binary_search_tree<Type, node_allocator> & operator=(binary_search_tree<Type, node_allocator> &&); // move assignment operator
        ~binary_search_tree(); // destructor

        class iterator_base {
        public:
            using value_type        = Type;
            using pointer           = Type*;
            using reference         = Type&;
            using size_type         = size_t;
            using difference_type   = ptrdiff_t;
        public:

        };

        class downside_iterator : public iterator_base {
        public:
            
        };
    };
}

#endif