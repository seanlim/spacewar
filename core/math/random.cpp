#include "random.h"

std::random_device rd;
std::mt19937 rng(rd());

int randInt(int l_limit, int u_limit)
{
  std::uniform_int_distribution<int> d(l_limit, u_limit);
  return d(rng);
}