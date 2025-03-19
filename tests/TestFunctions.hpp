#pragma once

#include <iostream>

template <typename T>
void Expect(const T expected, const T& actual)
{
    if (expected == actual)
    {
        return;
    }
    else
    {
        exit(1);
    }
}

void ExpectTrue(const bool actual)
{
    if (!actual)
    {
        exit(1);
    }
}

void ExpectFalse(const bool actual)
{
    if (actual)
    {
        exit(1);
    }
}