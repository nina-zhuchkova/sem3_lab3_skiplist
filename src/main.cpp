#include <SkipList.h>
#include <iostream>

int main() {
    int arr[] = {1, 3, 3, 2, 10};
    int arr1[] = {6, 11, 10, 6};
    std::cout << "construct skiplist (1) out of arr: ";
    for(auto &x : arr) {
        std::cout << x << " ";
    } std::cout << '\n';
    SkipList<int> l1(std::begin(arr), std::end(arr));
    std::cout << "skiplist (1):\n";
    l1.print();

    std::cout << "insert to skiplist (1) from arr: ";
    for(auto &x : arr1) {
        std::cout << x << " ";
    } std::cout << '\n';
    l1.insert(std::begin(arr1), std::end(arr1));
    std::cout << "skiplist (1):\n";
    l1.print();

    std::cout << "construct skiplist (2) by copying (1):\n";
    SkipList<int> l2(l1);
    std::cout << "skiplist (2):\n";
    l2.print();

    std::cout << "change skiplist (2) by inserting 0:\n(to show, that (2) is properly copied and is not linked with (1))\n";
    l2.insert(0);
    std::cout << "skiplist (1):\n";
    l1.print();
    std::cout << "skiplist (2):\n";
    l2.print();

    std::cout << "construct skiplist (3) by moving (1):\n";
    SkipList<int> l3(std::move(l1));
    std::cout << "skiplist (3):\n";
    l3.print();
    std::cout << "skiplist (1):\n";
    l1.print();

    std::cout << "clear (3):\n";
    l3.clear();
    std::cout << "skiplist (3):\n";
    l3.print();

    std::cout << "erase first 6 out of (2):\n";
    l2.erase(l2.find(6));
    std::cout << "skiplist (2):\n";
    l2.print();

    std::cout << "erase all 10 out of (2):\n";
    auto er = l2.equal_range(10);
    l2.erase(er.first, er.second);
    std::cout << "skiplist (2):\n";
    l2.print();

    return 0;
}