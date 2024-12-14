#pragma once
#include <type_traits> // includes std::add_lvalue_reference_t, std::add_pointer_t
#include <functional> // includes std::less
#include <iterator>
#include <vector>
#include <utility> // includes std::pair
#include <Add.h> // random add
#include <memory>
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

    template <typename It>
    SkipList (It beg, It end); // iterator constructor

    SkipList (SkipList<T, Cmp> const &src); // copy constructor

    SkipList<T, Cmp>& operator=(SkipList<T, Cmp> const &src); // copy assignment operator

    SkipList (SkipList<T, Cmp> &&src); // move constructor

    SkipList<T, Cmp>& operator=(SkipList<T, Cmp> &&src); // move assignment operator
    
    bool empty() const; 

    size_type size() const;
    
    SkipList<T, Cmp>& insert(T const &element); // O(logN)

    template <typename It>
    SkipList<T, Cmp>& insert(It beg, It end);

    iterator find(T const &element) const;

    size_type count(T const &element) const;

    iterator lower_bound(T const &element) const;

    iterator upper_bound(T const &element) const;
    
    SkipList<T, Cmp>& clear();

    SkipList<T, Cmp>& erase(iterator it);

    SkipList<T, Cmp>& erase(iterator beg, iterator end);

    std::pair<iterator, iterator> equal_range(T const &element) const;

    iterator begin() const;

    iterator end() const;

    reverse_iterator rbegin() const;

    reverse_iterator rend() const;

    void print() const;

    ~SkipList () = default;
private:
    std::vector<std::shared_ptr<Triple>> head;
    std::vector<std::shared_ptr<Triple>> tail;
    Cmp c;
    size_type nodes_size;
};


template <typename T, typename Cmp>
struct SkipList<T, Cmp>::Node final{
    Node(T const &element): element(element), nexts() { }
    
    T element;
    std::vector<std::shared_ptr<Triple>> nexts;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::Triple final{
    Triple( std::shared_ptr<Triple> next, 
            std::weak_ptr<Triple> prev, 
            unsigned idx, 
            std::shared_ptr<Node> node): 
            next(next), 
            prev(prev), 
            idx(idx), 
            node(node) { }

    std::shared_ptr<Triple> next;
    std::weak_ptr<Triple> prev;
    unsigned idx;
    std::shared_ptr<Node> node;
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::BidirectionalIterator final{
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = int;
    using value_type        = T;
    using pointer           = std::add_pointer_t<T>; 
    using reference         = std::add_lvalue_reference_t<T>; 

    BidirectionalIterator(): BidirectionalIterator(nullptr) { }
    explicit BidirectionalIterator(std::shared_ptr<Triple> current, bool past_the_end = false): current(current), past_the_end(past_the_end) { }

    BidirectionalIterator(BidirectionalIterator const &src): BidirectionalIterator(src.current, src.past_the_end) { }
    BidirectionalIterator& operator=(BidirectionalIterator const &src) {
        BidirectionalIterator tmp(src);
        std::swap(tmp.current, this->current);
        std::swap(tmp.past_the_end, this->past_the_end);
        return *this;
    }        

    reference operator*() { 
        if (past_the_end || !current) throw (std::out_of_range("Deferencing is impossiple")); 
        return current->node->element;
    }

    pointer operator->() {
        if (past_the_end || !current) throw (std::out_of_range("Deferencing is impossiple")); 
        return std::addressof(current->node->element); 
    }

    BidirectionalIterator& operator++() { 
        if (past_the_end) throw (std::out_of_range("Iterator increment is out of range")); 
        if (current) {
            if (!current->next) {
                past_the_end = true;
            } else {
                current = current->next;
            }
        }
        return *this;
    }

    BidirectionalIterator& operator--() { 
        if (past_the_end) { past_the_end = false; return *this; }
        if (current) { current = current->prev.lock(); } 
        else {std::out_of_range("Iterator decrement is out of range");}
        return *this;
    }

    BidirectionalIterator operator++(int) { auto tmp(*this); ++(*this); return tmp; }
    BidirectionalIterator operator--(int) { auto tmp(*this); --(*this); return tmp; }

    bool operator==(BidirectionalIterator const &rha) { return this->current == rha.current && this->past_the_end == rha.past_the_end; }
    bool operator!=(BidirectionalIterator const &rha) { return !(*this == rha); }

    std::shared_ptr<Triple> get_current() {
        return past_the_end ? nullptr : current;
    }

    std::shared_ptr<Triple> current;
    bool past_the_end;  // если оператор -- past the end, то итератор хранит
                        // указатель на последний элемент списка, но не дает к нему обратиться
                        // ( past_the_end = true )
};

template <typename T, typename Cmp>
struct SkipList<T, Cmp>::ReverseIterator final{
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = int;
    using value_type        = T;
    using pointer           = std::add_pointer_t<T>; 
    using reference         = std::add_lvalue_reference_t<T>; 

    ReverseIterator(): ReverseIterator(nullptr) { }
    explicit ReverseIterator(std::shared_ptr<Triple> current, bool past_the_end = false): current(current), past_the_end(past_the_end) { }

    ReverseIterator(ReverseIterator const &src): ReverseIterator(src.current, src.past_the_end) { }
    ReverseIterator& operator=(ReverseIterator const &src) {
        ReverseIterator tmp(src);
        std::swap(tmp.current, this->current);
        std::swap(tmp.past_the_end, this->past_the_end);
        return *this;
    }        

    reference operator*() { 
        if (past_the_end || !current) throw (std::out_of_range("Deferencing is impossiple")); 
        return current->node->element;
    }

    pointer operator->() {
        if (past_the_end || !current) throw (std::out_of_range("Deferencing is impossiple")); 
        return std::addressof(current->node->element); 
    }

    ReverseIterator& operator++() { 
        if (past_the_end) throw (std::out_of_range("Iterator increment is out of range")); 
        if (current) {
            if (!current->prev.lock()) {
                past_the_end = true;
            } else {
                current = current->prev.lock();
            }
        }
        return *this;
    }

    ReverseIterator& operator--() { 
        if (past_the_end) { past_the_end = false; return *this; }
        if (current) { current = current->next; } 
        else {std::out_of_range("Iterator decrement is out of range");}
        return *this;
    }

    ReverseIterator operator++(int) { auto tmp(*this); --(*this); return tmp; }
    ReverseIterator operator--(int) { auto tmp(*this); ++(*this); return tmp; }

    bool operator==(ReverseIterator const &rha) { return this->current == rha.current; }
    bool operator!=(ReverseIterator const &rha) { return !(*this == rha); }

    std::shared_ptr<Triple> get_current() {
        return past_the_end ? nullptr : current;
    }

    std::shared_ptr<Triple> current;
    bool past_the_end;
};

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList () : head(), tail(), c(), nodes_size(0) { }

template <typename T, typename Cmp>
template <typename It>
SkipList<T, Cmp>::SkipList (It beg, It end): SkipList() {
    while (beg != end) {
        this->insert(*beg++);
    }
}

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList (SkipList<T, Cmp> const &src): SkipList() { 
    if (src.empty()) {return;}
    auto current = src.begin();
    std::shared_ptr<Node> current_node = nullptr;
    head = std::vector<std::shared_ptr<Triple>>(src.head.size(), nullptr);
    tail = std::vector<std::shared_ptr<Triple>>(src.tail.size(), nullptr);
    while (current != src.end()) {
        if (!current_node || *current != current_node->element) { //новое значение
            current_node = std::make_shared<Node>(Node(*current));
            for (auto idx = 0u; idx < current.get_current()->node->nexts.size(); ++idx) {
                std::shared_ptr<Triple> newTriple = std::make_shared<Triple>(Triple(nullptr, 
                                                                                std::weak_ptr(tail[idx]), 
                                                                                idx, 
                                                                                current_node));
                current_node->nexts.push_back(newTriple);
                if (tail[idx]) { 
                    tail[idx]->next = newTriple; 
                } else {
                    head[idx] = newTriple;
                }
                tail[idx] = newTriple;
            }
        } else { //дубликат значения
            std::shared_ptr<Triple> newTriple = std::make_shared<Triple>(Triple(nullptr, 
                                                                                std::weak_ptr(tail[0]), 
                                                                                0, 
                                                                                current_node));
            tail[0]->next = newTriple;
            tail[0] = newTriple;
        }
        ++nodes_size;
        ++current;
    }
}

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::operator=(SkipList<T, Cmp> const &src) {
    if (std::addressof(src) == this) return *this;
    SkipList<T, Cmp> tmp(src); 
    std::swap(head, tmp.head);
    std::swap(tail, tmp.tail);
    std::swap(c, tmp.c);
    std::swap(nodes_size, tmp.nodes_size);
    return *this;
}

template <typename T, typename Cmp>
SkipList<T, Cmp>::SkipList (SkipList<T, Cmp> &&src):
    head(std::move(src.head)),
    tail(std::move(src.tail)),
    c(std::move(src.c)),
    nodes_size(std::move(src.nodes_size)) { 
        src.nodes_size = 0;
    }

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

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::insert(T const &element) {
    if(this->empty()) { //добавление первого элемента
        auto newNode = std::make_shared<Node>(Node(element));
        auto newTriple = std::make_shared<Triple>(Triple(   nullptr, 
                                                            std::weak_ptr<Triple>(), 
                                                            0, 
                                                            newNode));
        newNode->nexts.push_back(newTriple);
        tail.push_back(newTriple);
        head.push_back(newTriple);
        ++nodes_size;
        return *this;
    }
    auto lb = this->lower_bound(element);
    if ((!lb.past_the_end) && (!c(element, *lb))) { // если элемент уже есть в списке
        auto next = std::next(lb);
        auto newTriple = std::make_shared<Triple>(Triple(   next.get_current(), 
                                                    lb.get_current(), 
                                                    0, 
                                                    lb.get_current()->node));
        if (next.get_current()) {
            next.get_current()->prev = newTriple;
        } else {
            tail[0] = newTriple;
        }
        lb.get_current()->next = newTriple;
        ++nodes_size;
        return *this;
    } 

    // если нашего элемента еще не было
    auto newNode = std::make_shared<Node>(Node(element));
    auto prev = (lb.get_current() == head[0]) ? reverse_iterator() : reverse_iterator(std::prev(lb).get_current());
    auto next = lb;
    auto idx = 0u;
    while (true) {
        std::shared_ptr<Triple> newTriple = std::make_shared<Triple>(Triple(    next.get_current(), 
                                                                                prev.get_current(), 
                                                                                idx, 
                                                                                newNode));
        newNode->nexts.push_back(newTriple);

        if (next.get_current()) {
            next.get_current()->prev = newTriple;
        } else {
            tail[idx] = newTriple;
        }
        if (prev.get_current()) {
            prev.get_current()->next = newTriple;
        } else {
            head[idx] = newTriple;
        }
        //проверяем, дошли ли до верха
        if (idx == head.size() - 1 || !Add()) {
            break;
        }
        while (prev.get_current() && prev.get_current()->node->nexts.size() - 1 == idx) {
            ++prev;
        }
        if (prev.get_current()) {
            prev = reverse_iterator(prev.get_current()->node->nexts[idx+1]);
        }
        while (next.get_current() && next.get_current()->node->nexts.size() -1 == idx) {
            ++next;
        }
        if (next.get_current()) {
            next = iterator(next.get_current()->node->nexts[idx+1]);
        }
        ++idx;
    }
    if (idx == head.size() - 1 && Add()) {
        auto newTriple = std::make_shared<Triple>(Triple(   nullptr, 
                                                            std::weak_ptr<Triple>(), 
                                                            idx, 
                                                            newNode));
        newNode->nexts.push_back(newTriple);
        tail.push_back(newTriple);
        head.push_back(newTriple);
    }                          
    ++nodes_size;
    return *this;
    
}

template <typename T, typename Cmp>
template <typename It>
SkipList<T, Cmp>& SkipList<T, Cmp>::insert(It beg, It end) {
    while (beg != end) {
        this->insert(*beg++);
    }
    return *this;
}

template <typename T, typename Cmp>
bool SkipList<T, Cmp>::empty() const {
    return nodes_size == 0;
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::size_type SkipList<T, Cmp>::size() const {
    return nodes_size;
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::find(T const &element) const {
    auto lower_bound = this->lower_bound(element);
    if (!lower_bound.get_current() || c(element, *lower_bound)) { return this->end(); }
    return lower_bound;
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::size_type SkipList<T, Cmp>::count(T const &element) const {
    auto curr = this->lower_bound(element);
    auto upper_bound = this->upper_bound(element);
    unsigned count = 0;
    while (curr != upper_bound) { ++count; ++curr; }
    return count;
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::lower_bound(T const &element) const{
    if (head.empty()) {
        return iterator(); // empty iterator
    }
    auto idx = head.size() - 1;
    auto current_triple = iterator(head[idx]); 
    while (c(element, *current_triple)) { // elem < curr
        if (idx == 0) {
            return current_triple;
        }
        --idx;
        current_triple = iterator(head[idx]);
    }
    if (!c(*current_triple, element)) { //elem == curr
        return iterator(current_triple.get_current()->node->nexts[0]);
    }
    while (current_triple.get_current()) {
        while (!std::next(current_triple).get_current()) {
            if (idx == 0) {
                return this->end();
            }
            --idx;
            current_triple = iterator(current_triple.get_current()->node->nexts[idx]);
        } 
        if (c(element, *std::next(current_triple))) { //elem < next
            if (idx == 0) {
                return iterator(std::next(current_triple).get_current()->node->nexts[0]);
            }
            --idx;
            current_triple = iterator(current_triple.get_current()->node->nexts[idx]);
        } else if (c(*std::next(current_triple), element)){ //next < elem
            ++current_triple;
        } else { //next == elem
            return iterator(std::next(current_triple).get_current()->node->nexts[0]);
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
SkipList<T, Cmp>& SkipList<T, Cmp>::clear() {
    *this = SkipList<T, Cmp>();
    return *this;
}

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::erase(iterator it) { 
    // чтобы не терять балланс структуры, при удалении главного элемента при наличии дубликатов 
    // будем передавать Node первому из них
    if (!it.get_current() || this->empty()) { return *this; }
    if (it.get_current()->node->nexts[0] != it.get_current()) { //дубликат
        std::prev(it).get_current()->next = it.get_current()->next;
        if (it.get_current()->next) {
            std::next(it).get_current()->prev = it.get_current()->prev;
        } else {
            tail[0] = it.get_current()->prev.lock();
        } 
        it.get_current()->~Triple();
        --nodes_size;
        return *this;
    }
    if (std::next(it).get_current() && *std::next(it) == *it) { //главный узел с дубликатами
        it.get_current()->node->nexts[0] = std::next(it).get_current(); // передаем ноду дубликату
        if (it.get_current()->prev.lock()) {
            std::prev(it).get_current()->next = it.get_current()->next;
        } else {
            head[0] = it.get_current()->next;
        }
        if (it.get_current()->next) {
            std::next(it).get_current()->prev = it.get_current()->prev;
        } else {
            tail[0] = it.get_current()->prev.lock();
        }
        it.get_current()->~Triple();
        --nodes_size;
        return *this;
    }
    //главный узел без дубликатов
    auto size = it.get_current()->node->nexts.size();
    std::shared_ptr<Triple> curr_triple;
    while (size) {
        curr_triple = it.get_current()->node->nexts[size - 1];
        if (curr_triple->prev.lock()) {
            std::prev(it).get_current()->next = curr_triple->next;
        } else {
            head[size - 1] = curr_triple->next;
        }
        if (curr_triple->next) {
            std::next(it).get_current()->prev = curr_triple->prev;
        } else {
            tail[size - 1] = curr_triple->prev.lock();
        }
        it.get_current()->node->nexts.pop_back();
        size = it.get_current()->node->nexts.size();
        curr_triple->~Triple();
    }
    while (!head.back()) {
        head.pop_back();
    }
    while (!tail.back()) {
        tail.pop_back();
    }
    --nodes_size;
    return *this;
}

template <typename T, typename Cmp>
SkipList<T, Cmp>& SkipList<T, Cmp>::erase(iterator beg, iterator end) {
    if (this->empty()) { return *this; }
    auto curr = beg;
    while (beg != end) {
        beg++;
        this->erase(curr);
        curr = beg;
    }
    return *this;
}

template <typename T, typename Cmp>
std::pair<typename SkipList<T, Cmp>::iterator, typename SkipList<T, Cmp>::iterator> SkipList<T, Cmp>::equal_range(T const &element) const {
    return std::pair(this->lower_bound(element), this->upper_bound(element));
}

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::begin() const { return head.empty() ? iterator() : iterator(head[0]); }

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::iterator SkipList<T, Cmp>::end() const { return tail.empty() ? iterator() : iterator(tail[0], true); }

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::reverse_iterator SkipList<T, Cmp>::rbegin() const { return tail.empty() ? reverse_iterator() : reverse_iterator(tail[0]); }

template <typename T, typename Cmp>
typename SkipList<T, Cmp>::reverse_iterator SkipList<T, Cmp>::rend() const { return head.empty() ? reverse_iterator() : reverse_iterator(head[0], true); }

template <typename T, typename Cmp>
void SkipList<T, Cmp>::print() const{
    if (this->empty()) {
        std::cout << "empty list\n\n";
        return;
    }
    for (auto it = this->begin(); it != this->end(); ++it) {
        if (it.get_current()->node->nexts[0] != it.get_current()) { //дубликат узла
            std::cout << *it << '\t';
            for(auto i = 1u; i < head.size(); ++i) {
                std::cout << "|" << '\t';
            }
        } else {
            for(auto i = 0u; i < it.get_current()->node->nexts.size(); ++i) {
                std::cout << *it << '\t';
            } 
            for(auto i = it.get_current()->node->nexts.size(); i < head.size(); ++i){
                std::cout << "|" << '\t';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}