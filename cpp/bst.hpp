#ifndef BST_HPP
#define BST_HPP

/**
* @file      bst.hpp
* @author    snowapril
* @date      2018-12-24 (ongoing)
* @brief     custom binary search tree data structure which almost similar to STL.
* @details   header only Binary Search Tree data structure. provide only downside_iterator.
             when given downside_iterator, you can get it's left child using decrement operator 
             and it's right child using increment operator.
* @see       
* @reference http://tree.phi-sci.com/
*/

#include <initializer_list>
#include <memory>
#include <queue>
#include <vector>
#include "tree_exceptions.hpp"

namespace snowapril {

    template <typename Type>
    class bst_node_ {
    public:
        bst_node_() = default; // default constructor
        bst_node_(Type const &); // constructor with l-value data
        bst_node_(Type&&); // constructor with r-value data
        bst_node_(bst_node_<Type>&&); // move constructor
        bst_node_<Type> & operator=(bst_node_<Type>&&); // move assignment operator
        bst_node_(bst_node_<Type> const &); // copy constructor 
        bst_node_<Type> & operator=(bst_node_<Type> const &); // copy assignment operator

        bool operator==(bst_node_ const &) const;
        bool operator!=(bst_node_ const &) const;
    public:
        bst_node_<Type> *parent_node = nullptr;
        bst_node_<Type> *left_node   = nullptr;
        bst_node_<Type> *right_node  = nullptr;
        Type value;
    };

    template <typename Type, class node_allocator = std::allocator< bst_node_< Type > > >
    class binary_search_tree {
    protected:
        using node_type = bst_node_<Type>;
    public:
        using value_type      = Type;
        using pointer         = Type*;
        using reference       = Type&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        class iterator_base; 
        class downside_iterator;

        binary_search_tree() = default; // default constructor
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
            protected:
                using node_type = bst_node_<Type>;
            public:
                using value_type        = Type;
                using pointer           = Type*;
                using reference         = Type&;
                using size_type         = size_t;
                using difference_type   = ptrdiff_t;
            public:
                iterator_base() = default;
                iterator_base(node_type*);

                Type&   operator*()     const;
                Type*   operator->()    const;
            protected:
                node_type *node = nullptr;
            };

            class downside_iterator : public iterator_base {
            public:
                downside_iterator() = default;
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
            bool                empty() const;
            bool                is_root(downside_iterator const &) const;
            size_type           size() const;
            downside_iterator   begin() const;
            size_type           depth(downside_iterator const &) const;
            size_type           height(downside_iterator const &) const;
            downside_iterator   erase(downside_iterator);
            downside_iterator   remove(Type const &);
            downside_iterator   remove(downside_iterator, Type const &);
            void append(Type const &);
            void append(downside_iterator, Type const &);
        private:
            pointer _internal_remove(pointer, Type const &);
            pointer _internal_append(pointer, Type const &);
        private:
            node_allocator alloc;
            node_type* root     = nullptr;
            size_type  num_node = 0U;
    };

    template <typename Type>
    bst_node_<Type>::bst_node_(Type const & _l_value) : value(_l_value) { }   

    template <typename Type>
    bst_node_<Type>::bst_node_(Type&& _r_value) : value(move(_r_value)) { }

    template <typename Type>
    bst_node_<Type>::bst_node_(bst_node_<Type>&& _r_node) {
        left_node   = _r_node.left_node;
        right_node  = _r_node.right_node;
        parent_node = _r_node.parent_node;
        value       = move(_r_node.value);
        _r_node.left_node = (_r_node.right_node (_r_node.parent_node = nullptr));
    }

    template <typename Type>
    bst_node_<Type> & bst_node_<Type>::operator=(bst_node_<Type>&& _r_node) {
        if (this != &_r_node) {
            left_node   = _r_node.left_node;
            right_node  = _r_node.right_node;
            parent_node = _r_node.parent_node;
            value       = move(_r_node.value);
            _r_node.left_node = (_r_node.right_node (_r_node.parent_node = nullptr));
        }
        return *this;
    }

    template <typename Type>
    bst_node_<Type>::bst_node_(bst_node_<Type> const & _l_node) {
        value = _l_node.value;
        if (_l_node.left_node)  left_node  = new bst_node_(_l_node.value);
        if (_l_node.right_node) right_node = new bst_node_(_l_node.value);
    }

    template <typename Type>
    bst_node_<Type> & bst_node_<Type>::operator=(bst_node_<Type> const & _l_node) {
        if (this != &_l_node) {
            value = _l_node.value;
            if (_l_node.left_node)  left_node  = new bst_node_(_l_node.value);
            if (_l_node.right_node) right_node = new bst_node_(_l_node.value);
        }

        return *this;
    }

    template <typename Type>
    bool bst_node_<Type>::operator==(bst_node_ const & _node) const {
        return value == _node.value;
    }
    template <typename Type>
    bool bst_node_<Type>::operator!=(bst_node_ const & _node) const {
        return value != _node.value;
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::binary_search_tree(iterator_base const & _iter) {
        if (root) {
            alloc.destroy(root);
            alloc.deallocate(root, 1);
        }
        root = alloc.allocate(1, 0);
        root = alloc.construct(*_iter);
    }

    template <typename Type, class node_allocator>
    template <typename GenericIterator>
    binary_search_tree<Type, node_allocator>::binary_search_tree(std::iterator<GenericIterator, Type> const & _begin_iter, std::iterator<GenericIterator, Type> const & _end_iter) {
        if (root) {
            alloc.destroy(root);
            alloc.deallocate(root, 1);
        }
        for_each(_begin_iter, _end_iter, [this](const Type& _value) {
            this->append(_value);
        });        
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::binary_search_tree(std::initializer_list<Type> const & _i_list) {
        if (root) {
            alloc.destroy(root);
            alloc.deallocate(root, 1);
        }
        for (const auto& _value : _i_list) {
            this->append(_value);
        }
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::binary_search_tree(std::initializer_list<Type>&& _r_i_list) {
        if (root) {
            alloc.destroy(root);
            alloc.deallocate(root, 1);
        }
        auto i_list = move(_r_i_list);
        for (const auto& _value : i_list) {
            this->append(_value);
        }
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::binary_search_tree(Type const *_begin_iter, Type const *_end_iter) {
        if (root) {
            alloc.destroy(root);
            alloc.deallocate(root, 1);
        }

        for (Type const *iter = _begin_iter; iter != _end_iter; ++iter) {
            this->append(*iter);
        }
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::binary_search_tree(binary_search_tree<Type, node_allocator> const & _l_tree) {
        if (_l_tree.root) {
            root = alloc.allocate(1, 0);
            alloc.construct(root, *_l_tree.root);
        }
        num_node = _l_tree.num_node;
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator> & binary_search_tree<Type, node_allocator>::operator=(binary_search_tree<Type, node_allocator> const & _l_tree) {
        if (this != &_l_tree) {
            if (_l_tree.root) {
                root = alloc.allocate(1, 0);
                alloc.construct(root, *_l_tree.root);
            }
            num_node = _l_tree.num_node;
        }
        return *this;
    }
    
    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::binary_search_tree(binary_search_tree<Type, node_allocator> && _r_tree) {
        root = _r_tree.root;
        _r_tree.root = nullptr;
        num_node = _r_tree.num_node;
        _r_tree.num_node = 0U;
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator> & binary_search_tree<Type, node_allocator>::operator=(binary_search_tree<Type, node_allocator> && _r_tree) {
        if (this != _r_tree) {
            root = _r_tree.root;
            _r_tree.root = nullptr;
            num_node = _r_tree.num_node;
            _r_tree.num_node = 0U;
        }
        return *this;
    } 

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::~binary_search_tree() {
        if (root) {
            alloc.destroy(root);
            alloc.deallocate(root, 1);
        }
    }

    template <typename Type, class node_allocator>
    bool binary_search_tree<Type, node_allocator>::empty() const {
        return num_node == 0U;
    }
    
    template <typename Type, class node_allocator>
    bool binary_search_tree<Type, node_allocator>::is_root(downside_iterator const & _iter) const {
        return root == *_iter;
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::size_type binary_search_tree<Type, node_allocator>::size() const {
        return num_node;
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::begin() const {
        return downside_iterator(root);
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::size_type binary_search_tree<Type, node_allocator>::depth(downside_iterator const & _iter) const {    
        size_type   ret_depth    = 0U;
        pointer     next_pointer = *_iter;

        while (next_pointer != root) {
            next_pointer = next_pointer->parent_node;
            ++ ret_depth;
            if (next_pointer == nullptr) throw different_tree_exception("different_tree_exception : tree instance and given iterator are mismatched.");
        }

        return ret_depth;
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::size_type binary_search_tree<Type, node_allocator>::height(downside_iterator const & _iter) const {
        return depth(_iter) + 1U;
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::erase(downside_iterator _iter) {
        pointer parent_node = _iter->parent_node;
        if (parent_node->left_node == *_iter)
            parent_node->left_node = nullptr;
        else
            parent_node->right_node = nullptr;

        std::vector<pointer> will_be_deleted;
        std::queue<pointer>  q;
        q.push(*_iter);

        while (!q.empty()) {
            pointer p = q.front();
            q.pop();

            will_be_deleted.push_back(p);
            if (p->left_node)  q.push(p->left_node);
            if (p->right_node) q.push(p->right_node);
        }
        num_node -= will_be_deleted.size();

        for(auto& node : will_be_deleted) alloc.destroy(node);
        alloc.deallocate(&will_be_deleted[0], will_be_deleted.size());
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::remove(Type const &_value) {
        
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::remove(downside_iterator _iter, Type const &_value) {

    }

    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::pointer binary_search_tree<Type, node_allocator>::_internal_remove(pointer _node, Type const &_value) {
        
    }

    template <typename Type, class node_allocator>
    void binary_search_tree<Type, node_allocator>::append(Type const &_value) {
        
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::pointer binary_search_tree<Type, node_allocator>::_internal_append(pointer _node, Type const &_value) {
        
    }

    template <typename Type, class node_allocator>
    void binary_search_tree<Type, node_allocator>::append(downside_iterator _iter, Type const &_value) {

    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::iterator_base::iterator_base(node_type* _node) : node(_node) { }

    template <typename Type, class node_allocator>
    Type& binary_search_tree<Type, node_allocator>::iterator_base::operator*() const {
        return *node;
    }

    template <typename Type, class node_allocator>
    Type* binary_search_tree<Type, node_allocator>::iterator_base::operator->() const {
        return node;
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::downside_iterator::downside_iterator(node_type* _node) : iterator_base(_node) { }

    template <typename Type, class node_allocator>
    bool binary_search_tree<Type, node_allocator>::downside_iterator::operator==(downside_iterator const &_iter) const {
        return this->node == *_iter;
    }

    template <typename Type, class node_allocator>
    bool binary_search_tree<Type, node_allocator>::downside_iterator::operator!=(downside_iterator const &_iter) const {
        return this->node != *_iter;
    }

    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator&  binary_search_tree<Type, node_allocator>::downside_iterator::operator++() {
        this->node = (this->node)->right;
        return *this;
    }

    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator&  binary_search_tree<Type, node_allocator>::downside_iterator::operator--() {
        this->node = (this->node)->left;
        return *this;
    }

    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator&  binary_search_tree<Type, node_allocator>::downside_iterator::operator+=(unsigned int num) {
        while (num--) {
            ++(*this);
        }
        return *this;
    }

    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator&  binary_search_tree<Type, node_allocator>::downside_iterator::operator-=(unsigned int num) {
        while (num--) {
            --(*this);
        }
        return *this;
    }

}

#endif