#pragma once
#include "VoisinIterator.h"
class Voisin {
public:
    Voisin(int r, int c) : r(r), c(c) {}

    VoisinsIterator begin() const {
        return VoisinsIterator(r, c, false);
    }

    VoisinsIterator end() const {
        return VoisinsIterator(r, c, true);
    }

private:
    int r, c;
};
