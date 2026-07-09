#pragma once

#include <deque>

template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1, const std::deque<T>& half2, const Comp& comparator) {
    std::deque<T> result;
    auto it1 = half1.begin();
    auto it2 = half2.begin();
    
    while (it1 != half1.end() && it2 != half2.end()) {
        if (comparator(*it1, *it2)) {
            result.push_back(*it1);
            ++it1;
        } else {
            result.push_back(*it2);
            ++it2;
        }
    }
    
    while (it1 != half1.end()) {
        result.push_back(*it1);
        ++it1;
    }
    
    while (it2 != half2.end()) {
        result.push_back(*it2);
        ++it2;
    }
    
    return result;
}

template<class T, class Comp>
std::deque<T> MergeSort(const std::deque<T>& src, const Comp& comparator) {
    if (src.size() <= 1) {
        return src;
    }
    
    size_t mid = src.size() / 2;
    std::deque<T> left(src.begin(), src.begin() + mid);
    std::deque<T> right(src.begin() + mid, src.end());
    
    left = MergeSort(left, comparator);
    right = MergeSort(right, comparator);
    
    return Merge(left, right, comparator);
}