#pragma once
#include <unordered_set>

template <class T, class Comp, class Alloc, class Predicate>
void discard_if(std::unordered_set<T, Comp, Alloc>& c, Predicate pred) {
    for (auto it = c.begin() , end = c.end() ; it != end;) {
        if (pred(*it))
        {   
            it = c.erase(it);
        }
        else{
            ++it;
        }
    }
}
