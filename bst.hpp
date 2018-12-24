#ifndef BST_HPP
#define BST_HPP

#include <initializer_list>
#include <memory>

namespace snowapril {

    template <typename Type>
    class bst_node_ {
    public:
        bst_node_(); // default constructor
        bst_node_(Type const &); // constructor with l-value data
        bst_node_(Type&&); // constructor with r-value data
        bst_node_(bst_node_<Type>&&); // move constructor
        bst_node_<Type> & operator=(bst_node_<Type>&&); // move assignment operator
        bst_node_(bst_node_<Type> const &); // copy constructor 
        bst_node_<Type> & operator=(bst_node_<Type> const &); // copy assignment operator
        ~bst_node_(); // destructor

        bool operator==(bst_node_ const &) const;
        bool operator!=(bst_node_ const &) const;
    public:
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
        template <typename GenericIterator>
        binary_search_tree(std::iterator<GenericIterator, Type> const &, std::iterator<GenericIterator, Type> const &); //constructor with two standard iterators
        binary_search_tree(std::initializer_list<Type> const &); // constructor with l-value initializer_list
        binary_search_tree(std::initializer_list<Type>&&); // constructor with r-value initializer_list
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
            iterator_base();
            iterator_base(node_type*);

            Type&   operator*()     const;
            T*      operator->()    const;
        protected:
            node_type *node;
        };

        class downside_iterator : public iterator_base {
        public:
            downside_iterator();
            downside_iterator(node_type*);
        public:
            bool                operator==(downside_iterator const &) const;
            bool                operator!=(downside_iterator const &) const;
            downside_iterator&  operator++();
            downside_iterator&  operator--();
            downside_iterator&  operator+=(unsigned int);
            downside_iterator&  operator-=(unsigned int);
        };
        public:
            bool empty() const;
            bool is_root(downside_iterator const &) const;
            size_type size() const;
            downside_iterator begin() const;
            downside_iterator end() const;
            size_type depth(downside_iterator const &) const;
            size_type height(downside_iterator const &) const;
            downside_iterator erase(downside_iterator const &);
            downside_iterator remove(Type const &);
            downside_iterator remove(Type&&);
            void append(downside_iterator const &, Type&&);
            void append(downside_iterator const &, Type const &);
        private:
            node_type* root;
            size_type  num_node;
    };
}

#endif