#pragma once
#include <type_traits> // includes std::add_lvalue_reference_t, std::add_pointer_t
#include <functional> // includes std::less
#include <vector>
#include <memory> // includes smart pointers
#include <utility> // includes std::pair

template <typename T, typename Cmp = std::less<T>>
struct SkipList final{
public:
    using iterator = NodeIterator;
    using reverse_iterator = ReverseIterator;
    using value_type = T; 
    using reference = std::add_lvalue_reference_t<T>; 
    using pointer = std::add_pointer_t<T>;
    using size_type = unsigned;

    SkipList (); // default constructor for empty list

    template <typename It>
    SkipList (It begin, It end); // iterator constructor

    SkipList (SkipList<T, Cmp> const &copy); // copy constructor

    SkipList<T, Cmp>& operator=(SkipList<T, Cmp> const &src); // copy assignment operator

    SkipList (SkipList<T, Cmp> &&copy); // move constructor

    SkipList<T, Cmp>& operator=(SkipList<T, Cmp> &&src); // move assignment operator

    bool empty(); 

    size_type size();

    SkipList<T, Cmp>& insert(T value); // O(logN)

    template <typename It>
    SkipList<T, Cmp>& insert(It begin, It end);

    iterator find(T value);

    size_type count(T value);

    iterator lower_bound(T value);

    iterator upper_bound(T value);

    SkipList<T, Cmp>& clear();

    SkipList<T, Cmp>& erase(iterator it);

    SkipList<T, Cmp>& erase(iterator begin, iterator end);

    std::pair<iterator begin, iterator end> equal_range(T value);
private:
    struct Node final; // inner class for SkipList node
    struct Triple final; // inner class for Node levels
    struct BidirectionalIterator final;
    struct ReverseIterator final;
    
    std::vector<Triple*> head;
    Cmp c;
    size_type size;
};