// ����, ���, ���� "������ ����������������-2", �++, ���
//
// tbitfield.cpp - Copyright (c) ������� �.�. 07.05.2001
//   ������������ ��� Microsoft Visual Studio 2008 �������� �.�. (19.04.2015)
//
// ������� ����

#include "tbitfield.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8) //������� � ����

TBitField::TBitField(int len)
{
    if (len < 0) throw std::invalid_argument("Bit length must be >= 0");
    BitLen = len;
    if (BitLen == 0) {
        MemLen = 0;
        pMem = nullptr;
    }
    else {
        MemLen = (BitLen + BITS_IN_ONE_MEM - 1) / BITS_IN_ONE_MEM;
        pMem = new TELEM[MemLen];
        std::fill(pMem, pMem + MemLen, 0);
        /*for (int i = 0; i < MemLen; i++) {
          pMem[i] = 0;
          }
        */
    }
}

TBitField::TBitField(const TBitField& bf) // ����������� �����������
{
    if (&bf == this) return; //��������������� 
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    else pMem = nullptr;
}

TBitField::~TBitField()
{
    BitLen = 0;
    MemLen = 0;
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // ������ ��� ��� ���� n
{
    return (n / BITS_IN_ONE_MEM);
}

TELEM TBitField::GetMemMask(const int n) const // ������� ����� ��� ���� n
{

    return (1 << (n % BITS_IN_ONE_MEM));
}

// ������ � ����� �������� ����

int TBitField::GetLength(void) const // �������� ����� (�-�� �����)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // ���������� ���
{
    if ((n < 0) || (n >= BitLen))
        throw 2;
    int i = GetMemIndex(n);
    TELEM Mask = GetMemMask(n);
    pMem[i] |= Mask;
}

void TBitField::ClrBit(const int n) // �������� ���
{
    if ((n < 0) || (n >= BitLen)) throw 2;
    int i = GetMemIndex(n);
    TELEM Mask = GetMemMask(n);
    pMem[i] &= ~Mask; //~Mask - �������� ����� 
}

int TBitField::GetBit(const int n) const // �������� �������� ����
{
    if ((n < 0) || (n >= BitLen)) throw 2;
    int i = GetMemIndex(n);
    TELEM Mask = GetMemMask(n);
    return (pMem[i] & Mask) ? 1 : 0;
}

// ������� ��������

TBitField& TBitField::operator=(const TBitField& bf) // ������������
{
    if (&bf == this) return *this;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (pMem != nullptr) {
        delete[] pMem;
        pMem = nullptr;
    }
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const
{
    // 1. �������� �����
    if (BitLen != bf.BitLen)
        return 0; // false

    // 2. ��������� ���� ������ ��������� �������
    for (int i = 0; i < MemLen - 1; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0; // false
    }

    // 3. ��������� ���������� ��������
    if (MemLen > 0)
    {
        int k = BitLen % BITS_IN_ONE_MEM; // ���������� �������� ����� � ��������� ��������
        if (k == 0) // ������� ��������� ��������
        {
            if (pMem[MemLen - 1] != bf.pMem[MemLen - 1])
                return 0; // false
        }
        else // �������� ����������� �������
        {
            TELEM Mask = (1 << k) - 1; // ����� �������� �����
            if ((pMem[MemLen - 1] & Mask) != (bf.pMem[MemLen - 1] & Mask))
                return 0; // false
        }
    }

    // 4. ��� �������
    return 1; // true
}

int TBitField::operator!=(const TBitField& bf) const // ���������
{
    return (int)(!(*this == bf));
}

TBitField TBitField::operator|(const TBitField& bf)
{
    int newLen = std::max(this->BitLen, bf.BitLen);
    TBitField result(newLen);

    // ��������� ��� ���� ���������
    for (int i = 0; i < result.MemLen; i++)
    {
        TELEM a = (i < this->MemLen) ? this->pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a | b;
    }

    // �������� ������ ���� � ��������� ��������
    int k = newLen % BITS_IN_ONE_MEM;
    if (k != 0)
    {
        TELEM Mask = (1 << k) - 1;
        result.pMem[result.MemLen - 1] &= Mask;
    }

    return result;
}


TBitField TBitField::operator&(const TBitField& bf) // �������� "�"
{
    int newLen = std::max(this->BitLen, bf.BitLen);
    TBitField result(newLen);

    // ��������� � ���� ���������
    for (int i = 0; i < result.MemLen; i++)
    {
        TELEM a = (i < this->MemLen) ? this->pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a & b;
    }

    // �������� ������ ���� � ��������� ��������, ���� ����� �� ������ TELEM
    int k = newLen % BITS_IN_ONE_MEM;
    if (k != 0)
    {
        TELEM Mask = (1 << k) - 1;
        result.pMem[result.MemLen - 1] &= Mask;
    }

    return result;
}

TBitField TBitField::operator~(void) // ���������
{

    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~(this->pMem[i]);
    }
    if (BitLen % BITS_IN_ONE_MEM != 0) {
        TELEM Mask = (1 << (BitLen % BITS_IN_ONE_MEM)) - 1;
        result.pMem[MemLen - 1] &= Mask;
    }
    return result;
}

// ����/�����

istream& operator>>(istream& istr, TBitField& bf)
{
    char c;
    for (int i = 0; i < bf.GetLength(); i++)
    {
        istr >> c;                     // ������ ���� ������
        if (c == '1')
            bf.SetBit(i);              // ������ ���
        else if (c == '0')
            bf.ClrBit(i);              // ������� ���
        else
            throw std::invalid_argument("Invalid character in bitfield input");
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // �����
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}