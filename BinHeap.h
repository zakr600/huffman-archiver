//
// Created by Захар on 06.10.2021.
//

#ifndef ARCHIVER_BINHEAP_H
#define ARCHIVER_BINHEAP_H

#include <vector>

template<class T>
class BinHeap {
private:
    std::vector<T> a;

    size_t get_parent(size_t ind) const {
        if (ind == 0) {
            return 0;
        } else {
            return (ind - 1) / 2;
        }
    }

    size_t get_left_child(size_t ind) const {
        return 2 * ind + 1;
    }

    size_t get_right_child(size_t ind) const {
        return 2 * ind + 2;
    }
public:
    void push_back(const T& el) {
        a.push_back(el);
        int ind = a.size() - 1;
        while (ind > 0 && a[ind] < a[get_parent(ind)]) {
            swap(a[ind], a[get_parent(ind)]);
            ind = get_parent(ind);
        }
    }
    T extract_min() {
        T el = a[0];
        swap(a[0], a.back());
        a.pop_back();
        int ind = 0;
        while (ind < a.size()) {
            int left_son_pos = get_left_child(ind);
            int right_son_pos = get_right_child(ind);
            bool left_son_exists = (left_son_pos < a.size());
            bool right_son_exists = (right_son_pos < a.size());
            if (left_son_exists && a[left_son_pos] <= a[ind] && (!right_son_exists || a[left_son_pos] <= a[right_son_pos])) {
                swap(a[ind], a[left_son_pos]);
                ind = left_son_pos;
            } else if (right_son_exists && a[right_son_pos] <= a[ind] && (!left_son_exists || a[right_son_pos] <= a[left_son_pos])) {
                swap(a[ind], a[right_son_pos]);
                ind = right_son_pos;
            } else {
                break;
            }
        }
        return el;
    }
    size_t size() const {
        return a.size();
    }


};


#endif //ARCHIVER_BINHEAP_H
