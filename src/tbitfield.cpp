#include "tbitfield.h"
#include <iostream>

TBitField::TBitField(int len) : BitLen(len) {
    if (len <= 0) {
        throw std::invalid_argument("The bit field length must be positive");
    }
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) {
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField() {
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("A bit outside the bit field");
    }
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("A bit outside the bit field");
    }
    return 1 << (n % (sizeof(TELEM) * 8));
}

int TBitField::GetLength(void) const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) {
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const {
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
}

// Проверка на сравнение
int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
    return !(*this == bf);
}

TBitField& TBitField::operator=(const TBitField& bf) {
    if (this != &bf) {
        BitLen = bf.BitLen;
        if (MemLen != bf.MemLen) {
            delete[] pMem;
            pMem = nullptr;
            MemLen = bf.MemLen;
            pMem = new TELEM[MemLen];
        }
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

TBitField TBitField::operator|(const TBitField& bf) {
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);

    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            result.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            result.pMem[i] = bf.pMem[i];
        }
    }

    return result;
}

TBitField TBitField::operator&(const TBitField& bf) {
    int minLen = (BitLen < bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(minLen);

    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator~(void) {
    TBitField result(BitLen);

    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }

    int extraBits = BitLen % (sizeof(TELEM) * 8);
    if (extraBits != 0) {
        TELEM mask = (1 << extraBits) - 1;
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}

istream& operator>>(istream& istr, TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        char bit;
        istr >> bit;

        if (bit == '1') {
            bf.SetBit(i);
        }
        else if (bit == '0') {
            bf.ClrBit(i);
        }
        else {
            istr.putback(bit);
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << (bf.GetBit(i) ? '1' : '0');
        if ((i + 1) % 8 == 0 && i != bf.BitLen - 1) {
            ostr << ' ';
        }
    }
    return ostr;
}