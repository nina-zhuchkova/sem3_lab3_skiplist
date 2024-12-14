#pragma once
#include <type_traits> // includes std::add_lvalue_reference_t, std::add_pointer_t
#include <memory> // includes smart pointers
#include <iterator>
#include <iostream>

template <typename T>
struct List final {
private:
    /* внутренний класс для списочной пары */
    struct Node final {
        T element;
        std::unique_ptr<Node> next;
        Node *prev;
    };

    std::unique_ptr<Node> head;
    Node *tail;
public:
    struct BidirectionalIterator final {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = int;
        using value_type        = T;
        using pointer           = std::add_pointer_t<T>; /* aka T* */
        using reference         = std::add_lvalue_reference_t<T>; /* aka T& s*/

        BidirectionalIterator(): BidirectionalIterator(nullptr) { }
        explicit BidirectionalIterator(Node *current): current(current) { }

        BidirectionalIterator(BidirectionalIterator const &src): BidirectionalIterator(src.current) { }
        BidirectionalIterator& operator=(BidirectionalIterator const &src) {
            BidirectionalIterator tmp(src);
            std::swap(tmp.current, this->current);
            return *this;
        }        

        reference operator*() { return current->element; }
        pointer operator->() { return std::addressof(current->element); }
        pointer operator&() { return std::addressof(current->element); }

        BidirectionalIterator& operator++() { current = current->next ? current->next.get() : nullptr; return *this; }
        BidirectionalIterator& operator--() { current = current->prev; return *this; }

        BidirectionalIterator operator++(int) { auto tmp(*this); ++(*this); return tmp; }
        BidirectionalIterator operator--(int) { auto tmp(*this); --(*this); return tmp; }

        bool operator==(BidirectionalIterator const &rha) { return this->current == rha.current; }
        bool operator!=(BidirectionalIterator const &rha) { return !(*this == rha); }

        Node *current;
    };

    struct ReverseIterator final {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = int;
        using value_type        = T;
        using pointer           = std::add_pointer_t<T>; /* aka T* */
        using reference         = std::add_lvalue_reference_t<T>; /* aka T& s*/

        ReverseIterator(): ReverseIterator(nullptr) { }
        explicit ReverseIterator(Node *current): current(current) { }

        ReverseIterator(ReverseIterator const &src): ReverseIterator(src.current) { }
        ReverseIterator& operator=(ReverseIterator const &src) {
            ReverseIterator tmp(src);
            std::swap(tmp.current, this->current);
            return *this;
        }        

        reference operator*() { return current->element; }
        pointer operator->() { return std::addressof(current->element); }
        pointer operator&() { return std::addressof(current->element); }

        ReverseIterator& operator++() { current = current->prev ? current->prev.get() : nullptr; return *this; }
        ReverseIterator& operator--() { current = current->next; return *this; }

        ReverseIterator operator++(int) { auto tmp(*this); ++(*this); return tmp; }
        ReverseIterator operator--(int) { auto tmp(*this); --(*this); return tmp; }

        bool operator==(ReverseIterator const &rha) { return this->current == rha.current; }
        bool operator!=(ReverseIterator const &rha) { return !(*this == rha); }

        Node *current;
    };
    using iterator          = BidirectionalIterator;
    using reverse_iterator  = ReverseIterator;
    using value_type        = T; 
    using reference         = std::add_lvalue_reference_t<T>; 
    using pointer           = std::add_pointer_t<T>;

    List(): head(nullptr), tail(nullptr) { }
    
    template <typename It>
    List(It beg, It end): List() {
        while (beg != end) {
            this->push_back(*beg++);
        }
    }

    List (List<T> const &src): List(src.begin(), src.end()) { }

    List<T>& operator=(List<T> const &src) {
        if (std::addressof(src) == this) return *this;
        List<T> tmp(src); 
        std::swap(head, tmp.head);
        std::swap(tail, tmp.tail);
        return *this;
    }

    List (List<T> &&src): head(std::move(src.head)), tail(std::move(src.tail)) { }

    List<T>& operator=(List<T> &&src) {
        if (this == std::addressof(src)) return *this;
        List<T> tmp(std::move(src));
        std::swap(head, tmp.head);
        std::swap(tail, tmp.tail);
        return *this;
    }

    iterator begin() const { return iterator(head.get()); }
    iterator end() const { return iterator(); }
    reverse_iterator rbegin() const { return reverse_iterator(tail); }
    reverse_iterator rend() const { return reverse_iterator(); }

    List& push_front(T const &t) {
        std::unique_ptr<Node> node(new Node{std::move(t), nullptr, nullptr});
        if (!head) {
            head = std::move(node);
            tail = head.get();
        } else {
            head->prev = node.get();
            node->next = std::move(head);
            head = std::move(node);
        }
        return *this;
    }

    List& push_back(T const &t) {
        std::unique_ptr<Node> node(new Node{std::move(t), nullptr, nullptr});
        if (!head) {
            head = std::move(node);
            tail = head.get();
        } else {
            node->prev = tail;
            tail->next = std::move(node);
            tail = tail->next.get();
        }
        return *this;
    }

    List& push_after(T const &t, iterator it) {
        std::unique_ptr<Node> node(new Node{std::move(t), std::move(it.current->next), it.current});
        it.current->next = std::move(node);
        if (it.current->next->next) { it.current->next->next->prev = it.current->next.get(); }
        return *this;
    }

    List& erase(iterator it) {
        if (!it.current->prev and !it.current->next) {
            head.release();
            tail = nullptr;
            return *this;
        }

        if (it.current->prev) {
            head = std::move(it.current->next);
            return *this;
        }

        if (!it.current->next) {
            tail = it.current->prev;
            tail->next.release();
            return *this;
        }

        it.current->next->prev = it.current->prev;
        it.current->prev->next = std::move(it.current->next);

        return *this;
    }

    List& erase(iterator beg, iterator end) {
        while (beg != end) {
            auto next_it = std::next(beg);
            this->erase(beg);
            beg = next_it;
        }
        return *this;
    }
};