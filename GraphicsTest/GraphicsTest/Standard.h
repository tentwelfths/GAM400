#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <stdlib.h>
#include <string.h>
#include <unordered_map>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
  s.erase(0, s.find_first_not_of(t));
  return s;
}

// trim from both ends of string (left & right)
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
  return ltrim(rtrim(s, t), t);
}