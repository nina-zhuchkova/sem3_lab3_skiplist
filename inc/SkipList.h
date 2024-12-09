#pragma once
#include <type_traits> // includes std::add_lvalue_reference_t, std::add_pointer_t
#include <functional> // includes std::less
#include <iterator>
#include <vector>
#include <List.h>
#include <utility> // includes std::pair
#include <Add.h> // random add

/*

template <typename T, typename Cmp = std::less<T>>
struct SkipList final{
public:
    using iterator          = BidirectionalIterator;
    using reverse_iterator  = ReverseIterator;
    using value_type        = T; 
    using reference         = std::add_lvalue_reference_t<T>; 
    using pointer           = std::add_pointer_t<T>;
    using size_type         = std::list::size_type;

    SkipList (); // default constructor for empty list

    template <typename It>
    SkipList (It beg, It end); // iterator constructor

    SkipList (SkipList<T, Cmp> const &src); // copy constructor

    SkipList<T, Cmp>& operator=(SkipList<T, Cmp> const &src); // copy assignment operator

    SkipList (SkipList<T, Cmp> &&src); // move constructor

    SkipList<T, Cmp>& operator=(SkipList<T, Cmp> &&src); // move assignment operator

    bool empty(); 

    size_type size();

    SkipList<T, Cmp>& insert(T element); // O(logN)

    template <typename It>
    SkipList<T, Cmp>& insert(It beg, It end);

    iterator find(T element);

    size_type count(T element);

    iterator lower_bound(T element);

    iterator upper_bound(T element);

    SkipList<T, Cmp>& clear();

    SkipList<T, Cmp>& erase(iterator it);

    SkipList<T, Cmp>& erase(iterator beg, iterator end);

    std::pair<iterator beg, iterator end> equal_range(T element);

    iterator begin();

    iterator end();

    reverse_iterator rbegin();

    reverse_iterator rend();

    ~SkipList ();
private:
    struct Node; // inner class for SkipList node
    struct Triple; // inner class for Node levels
    struct BidirectionalIterator;
    struct ReverseIterator;
    
    std::vector<std::list<Triple>> head;
    std::list<Node> nodes;
    Cmp c;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::Node final{
public:
    Node(T element): element(element), nexts() { }
private:
    T element;
    std::vector<SkipList<T, Cmp>::Triple*> nexts;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::Triple final{
public:
    Triple(unsigned idx, SkipList<T, Cmp>::Node *node): idx(idx), node(node) { }
private:
    unsigned idx;
    SkipList<T, Cmp>::Node *node;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::BidirectionalIterator final{
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = int;
    using value_type        = T;
    using pointer           = std::add_pointer_t<T>; 
    using reference         = std::add_lvalue_reference_t<T>; 

    BidirectionalIterator(): BidirectionalIterator(nullptr) { }
    explicit BidirectionalIterator(std::list<SkipList<T, Cmp>::Node> *current): current(current) { }

    BidirectionalIterator(BidirectionalIterator const &src): BidirectionalIterator(src.current) { }
    BidirectionalIterator& operator=(BidirectionalIterator const &src) {
        BidirectionalIterator tmp(src);
        std::swap(tmp.current, this->current);
        return *this;
    }        

    reference operator*() { return current->element; }
    pointer operator->() { return std::addressof(current->element); }

    BidirectionalIterator& operator++() { ++current; return *this; }
    BidirectionalIterator& operator--() { --current; return *this; }

    BidirectionalIterator operator++(int) { auto tmp(*this); ++(*this); return tmp; }
    BidirectionalIterator operator--(int) { auto tmp(*this); --(*this); return tmp; }

    bool operator==(BidirectionalIterator const &rha) { return this->current == rha.current; }
    bool operator!=(BidirectionalIterator const &rha) { return !(*this == rha); }

    std::bidirectional_iterator<std::list<SkipList<T, Cmp>::Node>> *current;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::ReverseIterator final{
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = int;
    using value_type        = T;
    using pointer           = std::add_pointer_t<T>; 
    using reference         = std::add_lvalue_reference_t<T>; 

    ReverseIterator(): ReverseIterator(nullptr) { }
    explicit ReverseIterator(std::list<SkipList<T, Cmp>::Node> *current): current(current) { }

    ReverseIterator(ReverseIterator const &src): ReverseIterator(src.current) { }
    ReverseIterator& operator=(ReverseIterator const &src) {
        ReverseIterator tmp(src);
        std::swap(tmp.current, this->current);
        return *this;
    }        

    reference operator*() { return current->element; }
    pointer operator->() { return std::addressof(current->element); }

    ReverseIterator& operator++() { ++current; return *this; }
    ReverseIterator& operator--() { --current; return *this; }

    ReverseIterator operator++(int) { auto tmp(*this); ++(*this); return tmp; }
    ReverseIterator operator--(int) { auto tmp(*this); --(*this); return tmp; }

    bool operator==(ReverseIterator const &rha) { return this->current == rha.current; }
    bool operator!=(ReverseIterator const &rha) { return !(*this == rha); }

    std::reverse_iterator<std::list<SkipList<T, Cmp>::Node>> *current;
};

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList () : head(), nodes(), c() { }

template <typename T, typename Cmp>
template <typename It>
SkipList<T, Cmp>::SkipList (It beg, It end) {
    while (beg != end) {
        this->insert(*beg++);
    }
}

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList (SkipList<T, Cmp> const &src) {
    auto current = src.rbegin();

    while (current != nullptr) {
        /*...*//*
    }
}

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::operator=(SkipList<T, Cmp> const &src) {
    if (std::addressof(src) == this) return *this;
    SkipList<T, Cmp> tmp(src); 
    std::swap(head, tmp.head);
    std::swap(nodes, tmp.nodes);
    std::swap(c, tmp.c);
    return *this;
}

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList (SkipList<T, Cmp> &&src):
    head(std::move(src.head)),
    nodes(std::move(src.nodes)),
    c(std::move(src.c)) { }

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::operator=(SkipList<T, Cmp> &&src) {
    if (this == std::addressof(src)) return *this;
    SkipList<T, Cmp> tmp(std::move(src));
    std::swap(head, tmp.head);
    std::swap(nodes, tmp.nodes);
    std::swap(c, tmp.c);
    return *this;
}

template <typename T, typename Cmp>
bool SkipList<T, Cmp>::empty() {
    return nodes.empty();
}

template <typename T, typename Cmp>
SkipList<T, Cmp>::size_type SkipList<T, Cmp>::size() {
    return nodes.size();
}

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::insert(T element) {
    auto node_to_insert_before = this->upper_bound(T);
    // сначала построить вектор (башню)
    // потом приклеить его к спискам &idx
    if (node_to_insert_before != nullptr) {
        auto inserted_node = nodes.insert(std::prev(node_to_insert_before), Node(T));
        auto current_triple = head[0].insert(std::prev(inserted_node)->nexts[0], Triple(0, inserted_node));
        inserted_node->nexts.push_back(current_triple);
        if (node_to_insert_bebore->element == T) { }
    } else {
        /*...*//*
    }
}

template <typename T, typename Cmp>
SkipList<T, Cmp>::iterator SkipList<T, Cmp>::lower_bound(T element) {
    if (head.empty()) {
        return SkipList<T, Cmp>::iterator(); // empty iterator
    }
    auto idx = head.size() - 1;
    auto current_triple = head.back(); // not iterator
    while   (idx != 0 || 
            current_triple != nullptr && 
            current_triple->node->element < element) {
        if (std::next(current_triple) == nullptr) {
            if (idx == 0) {
                return this->end();
            } else {
                --idx;
                current_triple = current_triple->node->nexts[idx];
            }
        }
    }
}

iterator upper_bound(T element);

*/