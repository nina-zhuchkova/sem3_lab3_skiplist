#include <SkipList.h>
#include <iostream>

int main() {
    SkipList<int> l;
    l.begin();
    SkipList<int> l2(std::move(l));
    std::cout << l2.size();
    auto lb = l.upper_bound(1);
    return 0;
}