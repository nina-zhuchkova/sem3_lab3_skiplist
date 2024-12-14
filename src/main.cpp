#include <SkipList.h>
#include <iostream>
#include <memory>

int main() {
    SkipList<int> l;
    l.end();
    SkipList<int> l2(std::move(l));
    std::cout << l2.size();
    auto lb = l.upper_bound(1);
    return 0;
}