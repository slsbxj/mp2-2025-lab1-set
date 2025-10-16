#include "tset.h"
#include <iostream>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {
    if (mp <= 0) {
        throw std::invalid_argument("Мощность множества должна быть положительной");
    }
}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower(void) const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Элемент за пределами универсума");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Элемент за пределами универсума");
    }
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Элемент за пределами универсума");
    }
    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet& s) const {
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const {
    return !(*this == s);
}

TSet& TSet::operator=(const TSet& s) {
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

TSet TSet::operator+(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Элемент за пределами универсума");
    }
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Элемент за пределами универсума");
    }
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator+(const TSet& s) {
    int newMaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet result(newMaxPower);

    if (MaxPower == s.MaxPower) {
        result.BitField = BitField | s.BitField;
    }
    else {
        TBitField temp1 = BitField;
        TBitField temp2 = s.BitField;

        if (MaxPower < newMaxPower) {
            TBitField extended(newMaxPower);
            for (int i = 0; i < MaxPower; i++) {
                if (BitField.GetBit(i)) {
                    extended.SetBit(i);
                }
            }
            temp1 = extended;
        }

        if (s.MaxPower < newMaxPower) {
            TBitField extended(newMaxPower);
            for (int i = 0; i < s.MaxPower; i++) {
                if (s.BitField.GetBit(i)) {
                    extended.SetBit(i);
                }
            }
            temp2 = extended;
        }

        result.BitField = temp1 | temp2;
    }

    return result;
}

TSet TSet::operator*(const TSet& s) {
    int newMaxPower = (MaxPower < s.MaxPower) ? MaxPower : s.MaxPower;
    TSet result(newMaxPower);

    if (MaxPower == s.MaxPower) {
        result.BitField = BitField & s.BitField;
    }
    else {
        TBitField temp1 = BitField;
        TBitField temp2 = s.BitField;

        if (MaxPower > newMaxPower) {
            TBitField trimmed(newMaxPower);
            for (int i = 0; i < newMaxPower; i++) {
                if (BitField.GetBit(i)) {
                    trimmed.SetBit(i);
                }
            }
            temp1 = trimmed;
        }

        if (s.MaxPower > newMaxPower) {
            TBitField trimmed(newMaxPower);
            for (int i = 0; i < newMaxPower; i++) {
                if (s.BitField.GetBit(i)) {
                    trimmed.SetBit(i);
                }
            }
            temp2 = trimmed;
        }

        result.BitField = temp1 & temp2;
    }

    return result;
}

TSet TSet::operator~(void) {
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

istream& operator>>(istream& istr, TSet& s) {
    int elem;
    while (istr >> elem) {
        if (elem >= 0 && elem < s.MaxPower) {
            s.InsElem(elem);
        }
        else {
            istr.putback(elem);
            break;
        }

        if (istr.peek() == '\n' || istr.peek() == EOF) {
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) {
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i)) {
            if (!first) {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }
    }
    ostr << " }";
    return ostr;
}