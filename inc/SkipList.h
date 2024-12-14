#pragma once
#include <type_traits> // includes std::add_lvalue_reference_t, std::add_pointer_t
#include <functional> // includes std::less
#include <iterator>
#include <vector>
#include <utility> // includes std::pair
#include <Add.h> // random add
#include <iostream>


template <typename T, typename Cmp = std::less<T>>
struct SkipList final{
private:
    struct Node; // inner class for SkipList node
    struct Triple; // inner class for Node levels
    struct BidirectionalIterator;
    struct ReverseIterator;
public:
    using iterator          = BidirectionalIterator;
    using reverse_iterator  = ReverseIterator;
    using value_type        = T; 
    using reference         = std::add_lvalue_reference_t<T>; 
    using pointer           = std::add_pointer_t<T>;
    using size_type         = unsigned;

    SkipList (); // default constructor for empty list

    //template <typename It>
    //SkipList (It beg, It end); // iterator constructor

    //SkipList (SkipList<T, Cmp> const &src); // copy constructor

    //SkipList<T, Cmp>& operator=(SkipList<T, Cmp> const &src); // copy assignment operator

    SkipList (SkipList<T, Cmp> &&src); // move constructor

    SkipList<T, Cmp>& operator=(SkipList<T, Cmp> &&src); // move assignment operator
    
    bool empty(); 

    size_type size();
    
    SkipList<T, Cmp>& insert(T const &element); // O(logN)

    //template <typename It>
    //SkipList<T, Cmp>& insert(It beg, It end);

    //iterator find(T const &element) const;

    //size_type count(T const &element) const;

    iterator lower_bound(T const &element) const;

    iterator upper_bound(T const &element) const;
    /*
    SkipList<T, Cmp>& clear();

    SkipList<T, Cmp>& erase(iterator it);

    SkipList<T, Cmp>& erase(iterator beg, iterator end);
*/
    std::pair<iterator, iterator> equal_range(T const &element) const;

    iterator begin() const;

    iterator end() const;

    reverse_iterator rbegin() const;

    reverse_iterator rend() const;

    ~SkipList () = default;
private:
    std::vector<std::unique_ptr<Triple>> head;
    std::vector<Triple*> tail;
    Cmp c;
    size_type nodes_size;
};


template <typename T, typename Cmp>
struct SkipList<T, Cmp>::Node final{
    Node(T const &element): element(element), nexts() { }
    
    T element;
    std::vector<SkipList<T, Cmp>::Triple*> nexts;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::Triple final{
    Triple(unsigned idx, SkipList<T, Cmp>::Node *node): next(nullptr), prev(nullptr), idx(idx), node(node) { }

    std::unique_ptr<Triple> next;
    Triple *prev;
    unsigned idx;
    std::shared_ptr<SkipList<T, Cmp>::Node> node;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::BidirectionalIterator final{
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = int;
    using value_type        = T;
    using pointer           = std::add_pointer_t<T>; 
    using reference         = std::add_lvalue_reference_t<T>; 

    BidirectionalIterator(): BidirectionalIterator(nullptr) { }
    explicit BidirectionalIterator(Triple *current): current(current) { }

    BidirectionalIterator(BidirectionalIterator const &src): BidirectionalIterator(src.current) { }
    BidirectionalIterator& operator=(BidirectionalIterator const &src) {
        BidirectionalIterator tmp(src);
        std::swap(tmp.current, this->current);
        return *this;
    }        

    reference operator*() { return current->node->element; }
    pointer operator->() { return std::addressof(current->node->element); }

    BidirectionalIterator& operator++() { current = current->next ? current->next.get() : nullptr; return *this; }
    BidirectionalIterator& operator--() { current = current->prev; return *this; }

    BidirectionalIterator operator++(int) { auto tmp(*this); ++(*this); return tmp; }
    BidirectionalIterator operator--(int) { auto tmp(*this); --(*this); return tmp; }

    bool operator==(BidirectionalIterator const &rha) { return this->current == rha.current; }
    bool operator!=(BidirectionalIterator const &rha) { return !(*this == rha); }

    Triple * current;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::ReverseIterator final{
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = int;
    using value_type        = T;
    using pointer           = std::add_pointer_t<T>; 
    using reference         = std::add_lvalue_reference_t<T>; 

    ReverseIterator(): ReverseIterator(nullptr) { }
    explicit ReverseIterator(Triple * current): current(current) { }

    ReverseIterator(ReverseIterator const &src): ReverseIterator(src.current) { }
    ReverseIterator& operator=(ReverseIterator const &src) {
        ReverseIterator tmp(src);
        std::swap(tmp.current, this->current);
        return *this;
    }        

    reference operator*() { return current->node->element; }
    pointer operator->() { return std::addressof(current->node->element); }

    ReverseIterator& operator++() { current = current->prev; return *this; }
    ReverseIterator& operator--() { current = current->next ? current->next.get() : nullptr; return *this; }

    ReverseIterator operator++(int) { auto tmp(*this); --(*this); return tmp; }
    ReverseIterator operator--(int) { auto tmp(*this); ++(*this); return tmp; }

    bool operator==(ReverseIterator const &rha) { return this->current == rha.current; }
    bool operator!=(ReverseIterator const &rha) { return !(*this == rha); }

    Triple * current;
};

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList () : head(), tail(), c(), nodes_size(0) { }

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList (SkipList<T, Cmp> &&src):
    head(std::move(src.head)),
    tail(std::move(src.tail)),
    c(std::move(src.c)),
    nodes_size(std::move(src.nodes_size)) { }

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::operator=(SkipList<T, Cmp> &&src) {
    if (this == std::addressof(src)) return *this;
    SkipList<T, Cmp> tmp(std::move(src));
    std::swap(head, tmp.head);
    std::swap(tail, tmp.tail);
    std::swap(c, tmp.c);
    std::swap(nodes_size, tmp.nodes_size);
    return *this;
}

#include <iostream>
template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::insert(T const &element) {
     // тут хуйня не смотри
    if (this->empty()) {
        std::unique_ptr<Triple> triple(new Triple{std::move(t), nullptr, nullptr});
        head.push_back(List<Triple>());
        head.back().push_back(Triple(0, &nodes.begin()));
        auto curr_triple = head.back().begin();
        curr_triple->node->push_back(&head.back().begin());
        for(auto idx = 1u; Add(); ++idx) {
            head.push_back(List<Triple>());
            head.back().push_back(Triple(idx, &nodes.begin()));
            curr_triple->node->push_back(&head.back().begin());
        }
        ++nodes_size;
        return *this;
    }
    
    return *this;
}

template <typename T, typename Cmp>
bool SkipList<T, Cmp>::empty() {
    return nodes_size == 0;
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::size_type SkipList<T, Cmp>::size() {
    return nodes_size;
}


template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::lower_bound(T const &element) const{
    if (head.empty()) {
        return iterator(); // empty iterator
    }
    auto idx = head.size() - 1;
    auto current_triple = iterator(head.back().get()); 
    if ((!c(*current_triple, element)) && (!c(element, *current_triple))) { //==
        return iterator(current_triple.current->node->nexts[0]);
    }
    while   (current_triple.current != nullptr) {
        if (std::next(current_triple).current == nullptr) {
            if (idx == 0) {
                return this->end();
            }
            --idx;
            current_triple = iterator(current_triple.current->node->nexts[idx]);
        } else if (c(element, *std::next(current_triple))) { //elem < next
            if (idx == 0) {
                return std::next(current_triple);
            }
            --idx;
            current_triple = iterator(current_triple.current->node->nexts[idx]);
        } else if (c(*std::next(current_triple), element)) { //next < elem
            ++current_triple;
        } else { //==
            return std::next(current_triple);
        }
    }
    return this->end();
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::upper_bound(T const &element) const {
    auto curr = this->lower_bound(element);
    while (curr != this->end() && (!c(element, *curr))) { ++curr; }
    return curr;
}

template <typename T, typename Cmp>
std::pair<typename SkipList<T, Cmp>::iterator, typename SkipList<T, Cmp>::iterator> SkipList<T, Cmp>::equal_range(T const &element) const {
    return std::pair(this->lower_bound(element), this->upper_bound(element));
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::begin() const { return head.empty() ? iterator() :iterator(head[0].get()); }

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::end() const { return iterator(); }

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::reverse_iterator SkipList<T, Cmp>::rbegin() const { return reverse_iterator(tail[0]); }

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::reverse_iterator SkipList<T, Cmp>::rend() const { return reverse_iterator(); }