#pragma once

#include <vector>

template <typename T> class Array : public std::vector<T>
{
public:
  // Constructors
  Array<T>() : std::vector<T>() {}
  Array<T>(size_t size) : std::vector<T>(size) {}
};