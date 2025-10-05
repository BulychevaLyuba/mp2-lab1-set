// ����, ���, ���� "������ ����������������-2", �++, ���
//
// tset.cpp - Copyright (c) ������� �.�. 04.10.2001
//   ������������ ��� Microsoft Visual Studio 2008 �������� �.�. (19.04.2015)
//
// ��������� - ���������� ����� ������� ����

#include "tset.h"
#include <stdexcept>

TSet::TSet(int mp) : BitField(mp)
{
    if (mp <= 0) throw std::invalid_argument("the power must be greater than 0");
    MaxPower = mp;
}

// ����������� �����������
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// ����������� �������������� ����
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // �������� ����. �-�� ��-���
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element out of range");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // ��������� �������� ���������
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // ���������� �������� ���������
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element out of range");
    BitField.ClrBit(Elem);
}

// ���������-������������� ��������

TSet& TSet::operator=(const TSet& s) // ������������
{
    if (&s == this) return;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // ���������
{
    if (MaxPower != s.MaxPower) return false;
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // ���������
{
    return !(*this == s) ? 1 : 0;
}

TSet TSet::operator+(const TSet& s) // �����������
{
    int Power = max(this->MaxPower, s.MaxPower);
    TSet Result(Power);
    Result.BitField = this->BitField;
    Result.BitField = Result.BitField | s.BitField;
    return Result;
}

TSet TSet::operator+(const int Elem) // ����������� � ���������
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element out of range");
    TSet Result(MaxPower);
    Result.BitField = this->BitField;
    Result.BitField.SetBit(Elem);
    return Result;
}

TSet TSet::operator-(const int Elem) // �������� � ���������
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element out of range");
    TSet Result(*this);
    Result.BitField.ClrBit(Elem);
    return Result;
}

TSet TSet::operator*(const TSet& s) // �����������
{
    int Power = min(this->MaxPower, s.MaxPower);
    TSet Result(Power);
    Result.BitField = this->BitField & s.BitField;
    return Result;
}

TSet TSet::operator~(void) // ����������
{
    TSet Result(MaxPower);
    Result.BitField = ~this->BitField;
    return Result;
}

// ���������� �����/������

// ���� ���������
istream& operator>>(istream& istr, TSet& s)
{
    char c;
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        istr >> c;                       // ������ ���� ������
        if (c == '1') s.InsElem(i);      // ������ ���
        else if (c == '0') s.DelElem(i); // ������� ���
        else throw std::invalid_argument("Invalid character in set input");
    }
    return istr;
}

// ����� ���������
ostream& operator<<(ostream& ostr, const TSet& s)
{
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        ostr << s.BitField.GetBit(i);   // ������� 0 ��� 1
    }
    return ostr;
}