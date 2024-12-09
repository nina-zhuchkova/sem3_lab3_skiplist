#include <SkipList.h>
#include <List.h>
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    List<int> l(std::begin(arr), std::end(arr));
    List<int> l2(l);

    for (auto &x : l)
        std::cout << x << ' ';
    std::cout << '\n';
    for (auto &x : l2)
        std::cout << x << ' ';
    std::cout << '\n';

    List<int> l3 = std::move(l);
    for (auto &x : l)
        std::cout << x << ' ';
    std::cout << '\n';
    for (auto &x : l3)
        std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}