#pragma once
#include <vector>
#include <iostream>

class VoisinsIterator {
public:
    VoisinsIterator(int r, int c, bool fin) : r(r), c(c), index(fin ? 6 : 0) {
        if (r % 2 == 0) {
            // Cas r pair
            offsets = { {-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0} };
        }
        else {
            // Cas r impair
            offsets = { {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1} };
        }
    }

    std::pair<int, int> operator*() const {
        return { r + offsets[index].first, c + offsets[index].second };
    }

    VoisinsIterator& operator++() {
        ++index;
        return *this;
    }

    bool operator!=(const VoisinsIterator& other) const {
        return index != other.index;
    }

private:
    int r, c;
    int index;
    std::vector<std::pair<int, int>> offsets;
};
