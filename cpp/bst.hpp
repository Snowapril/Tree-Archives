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
        bst_node_(bst_node_<Type>*, Type const &); // constructor with parent pointer and l_value data.
        bst_node_(bst_node_<Type>*, Type&&); // constructor with parent pointer and r_value data.
        bst_node_(bst_node_<Type>&&); // move constructor
        bst_node_<Type> & operator=(bst_node_<Type>&&); // move assignment operator
        bst_node_(bst_node_<Type> const &); // copy constructor 
        bst_node_<Type> & operator=(bst_node_<Type> const &); // copy assignment operator
        ~bst_node_(); //destructor
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
                friend class binary_search_tree<Type, node_allocator>;
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
                //return reference of value.
                Type&   operator*()     const;
                //return pointerr of value.
                Type*   operator->()    const;
                //for testing ( node == nullptr ) at the out of this scope.
                explicit operator bool() const;
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
                //return downside_iterator which located at (given value) times right step from this iterator.
                const downside_iterator operator+(unsigned int);
                //return downside_iterator which located at (given value) times left  step from this iterator.
                const downside_iterator operator-(unsigned int);
                //return downside_iterator of right node.
                downside_iterator&  operator++();
                //return downside_iterator of left  node.
                downside_iterator&  operator--();
                downside_iterator&  operator+=(unsigned int);
                downside_iterator&  operator-=(unsigned int);
                explicit operator bool() const;
            };
        public:
            //return whether if tree is empty.
            bool                empty() const;
            //return whether if given iterator representate root node.
            bool                is_root(downside_iterator const &) const;
            //return the number of node in this tree.
            size_type           size() const;
            //return downside_iterator of root node.
            downside_iterator   begin() const;
            //return the depth of given iterator in this tree.
            size_type           depth(downside_iterator const &) const;
            //return the height(depth + 1) of given iterator in this tree.
            size_type           height(downside_iterator const &) const;
            //remove sub-tree where given iterator is root node.
            downside_iterator   erase(downside_iterator);
            //remove node which is matched with given value in this tree.
            downside_iterator   remove(Type const &);
            //remove node which is matched with given value in the sub-tree where given iterator is root node.
            downside_iterator   remove(downside_iterator, Type const &);
            //append node with given value in the tree.
            void append(Type const &);
            //append node with given value in the sub-tree where given iterator is root node.
            void append(downside_iterator, Type const &);
        private:
            //implementation of method which removes node in the tree.
            void _internal_remove(node_type*, Type const &);
            //implementation of method which appends node in the tree.
            void _internal_append(node_type*, Type const &);
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
    bst_node_<Type>::bst_node_(bst_node_<Type> *parent, Type const &_l_value) : parent_node(parent), value(_l_value) { }
    template <typename Type>
    bst_node_<Type>::bst_node_(bst_node_<Type> *parent, Type &&_r_value) : parent_node(parent), value(move(_r_value)) { }

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

    #include <iostream>
    using namespace std;

    template <typename Type>
    bst_node_<Type>::~bst_node_() {
        cout << "destructor : " << value << endl;
        if (left_node)  delete left_node;
        if (right_node) delete right_node;
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
    binary_search_tree<Type, node_allocator>::binary_search_tree(iterator_base const &_iter) {
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
        node_type*  next_pointer = _iter.node;

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
        node_type* parent_node = _iter.node->parent_node;
        if (parent_node->left_node == _iter.node)
            parent_node->left_node = nullptr;
        else
            parent_node->right_node = nullptr;

        std::queue<node_type*>  q;
        q.push(_iter.node);
        while (!q.empty()) {
            node_type* p = q.front();
            q.pop();
            --num_node;
            if (p->left_node)  q.push(p->left_node);
            if (p->right_node) q.push(p->right_node);
            alloc.destroy(p);
            alloc.deallocate(p, 1);
        }
        return downside_iterator(parent_node);
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::remove(Type const &_value) {
        if (root) {
            _internal_remove(root, _value);
        }
    }
    
    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::remove(downside_iterator _iter, Type const &_value) {
        if (_iter.node) {
            _internal_remove(_iter.node, _value);
        }
    }

    template <typename Type, class node_allocator>
    void binary_search_tree<Type, node_allocator>::_internal_remove(node_type* _node, Type const &_value) {
        //dsafdsfsdfdsf //TODO : here implementation
    }

    template <typename Type, class node_allocator>
    void binary_search_tree<Type, node_allocator>::append(Type const &_value) {
        if (root) {
            _internal_append(root, _value);
        }
        else {
            root = alloc.allocate(1, 0);
            alloc.construct(root, _value);
        }
    }
    
    template <typename Type, class node_allocator>
    void binary_search_tree<Type, node_allocator>::append(downside_iterator _iter, Type const &_value) {
        if (_iter.node) {
            _internal_append(_iter, _value);
        }
        else {
            _iter.node = alloc.allocate(1,  0);
            alloc.construct(_iter.node, _value);
        }
    }
    
    template <typename Type, class node_allocator>
    void binary_search_tree<Type, node_allocator>::_internal_append(node_type* _node, Type const &_value) {
        node_type* prev_node;
        while (_node) {
            prev_node = _node;
            if (_node->value > _value) {
                _node = _node->left_node;
            }
            else if (_node->value == _value) {
                return;
            }
            else {
                _node = _node->right_node;
            }
        }

        _node = prev_node;
        if (_node->value > _value) {
            _node->left_node = alloc.allocate(1, 0);
            alloc.construct(_node->left_node, _node, _value);
        }
        else {
            _node->right_node = alloc.allocate(1, 0);
            alloc.construct(_node->right_node, _node, _value);
        }
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::iterator_base::iterator_base(node_type* _node) : node(_node) { }

    template <typename Type, class node_allocator>
    Type& binary_search_tree<Type, node_allocator>::iterator_base::operator*() const {
        return node->value;
    }

    template <typename Type, class node_allocator>
    Type* binary_search_tree<Type, node_allocator>::iterator_base::operator->() const {
        return &node->value;
    }

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::iterator_base::operator bool() const {
        return node != nullptr;
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
        if (this->node) {
            this->node = (this->node)->right_node;
        }
        return *this;
    }

    template <typename Type, class node_allocator>
    typename binary_search_tree<Type, node_allocator>::downside_iterator&  binary_search_tree<Type, node_allocator>::downside_iterator::operator--() {
        if (this->node) {
            this->node = (this->node)->left_node;
        }
        return *this;
    }

    template <typename Type, class node_allocator>
    const typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::downside_iterator::operator+(unsigned int num) {
        downside_iterator ret_iter = *this;
        while (num--) {
            ++(ret_iter);
        }
        return ret_iter;
    }
    
    template <typename Type, class node_allocator>
    const typename binary_search_tree<Type, node_allocator>::downside_iterator binary_search_tree<Type, node_allocator>::downside_iterator::operator-(unsigned int num) {
        downside_iterator ret_iter = *this;
        while (num--) {
            --(ret_iter);
        }
        return ret_iter;
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

    template <typename Type, class node_allocator>
    binary_search_tree<Type, node_allocator>::downside_iterator::operator bool() const {
        return this->node != nullptr;
    }
}

#endif