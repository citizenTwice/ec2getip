/*

C++ string util library - coded by Luigi Galli GIT@THLG.nl

MIT License

Copyright (c) 2020 Luigi Galli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 

*/
#pragma once
#include <string>
#include <algorithm>
#include <vector>
#include <regex>
#include <array>

namespace citizentwice_str { 

  std::string to_lower(const char *s) {
    std::string _s{s};
    std::transform(_s.begin(), _s.end(), _s.begin(), ::tolower); 
    return _s;
  }

  std::string to_lower(const std::string& str) {
    return to_lower(str.c_str());
  }

  std::string to_upper(const char *s) {
    std::string _s{s};
    std::transform(_s.begin(), _s.end(), _s.begin(), ::toupper);
    return _s;
  }

  std::string to_upper(const std::string& str) {
    return to_upper(str.c_str());
  }

  std::vector<std::string> split(const std::string& s1, const std::string& s2 ) {
    std::vector<std::string> retval{};
    auto p = s1.find(s2);
    if (p == std::string::npos || s2.length() == 0 || s1.length() == 0) {
      retval.push_back(s1);
      return retval;
    }
    auto ip = s1.begin();
    while (p != -1 && ip < s1.end()) {
      retval.push_back(std::string{ip, s1.begin() + p});
      ip = s1.begin() + (p + s2.length());
      p = s1.find(s2, p+s2.length());
      if (p == std::string::npos ) break;
    }
    retval.push_back(std::string{ip, s1.end()});
    return retval;
  }
  
  std::string substring_before(const std::string& s, const std::string& lookfor) {
    if (lookfor.length() == 0) return s;
    auto p = s.find(lookfor);
    if (p == std::string::npos || s.length() == 0) return {};
    return std::string{s.begin(), s.begin() + p};
  }

  std::string substring_before(const char *str, const char* lookfor) {
    if (str == nullptr || lookfor == nullptr) return {};
    return substring_before(std::string{str}, std::string{lookfor});
  }

  std::string substring_after(const std::string& s, const std::string& lookfor) {
    if (lookfor.length() == 0) return s;
    auto p = s.find(lookfor);
    if (p == std::string::npos || s.length() == 0) return {};
    return std::string{s.begin() + p + lookfor.length(), s.end()};
  }

  std::string substring_after(const char *str, const char* lookfor) {
    if (str == nullptr || lookfor == nullptr) return {};
    return substring_after(std::string{str}, std::string{lookfor});
  }

  std::string ltrim(const std::string& s) {
    std::array<char,4> w{' ', 9, 10, 13};
    std::string delims{w.begin(),w.end()};
    auto p = s.find_first_not_of(delims);
    if (p == std::string::npos) {
      if (s.find_first_of(delims) != std::string::npos) {return {};}
      else {return s;}
    }
    return std::string{s.begin() + p, s.end()};
  }

  std::string rtrim(const std::string& s) {
    std::array<char,4> w{' ', 9, 10, 13};
    std::string delims{w.begin(),w.end()};
    auto p = s.find_last_not_of(delims);
    if (p == std::string::npos) {
      if (s.find_first_of(delims) != std::string::npos) {return {};}
      else {return s;}
    }
    return std::string{s.begin(), s.begin() + p + 1};
  }

  std::string trim(const std::string& s) {
    return ltrim(rtrim(s));
  }

  std::string left(const std::string& s, size_t chars) {
    if (chars >= s.length()) return s;
    return std::string{s.begin(), s.begin() + chars};
  }

  std::string left(const char *s, size_t chars) {
    if (s == nullptr) {return {}; };
    return left(std::string{s}, chars);
  }

  std::string right(const std::string& s, size_t chars) {
    if (chars >= s.length()) return s;
    return std::string{s.end() - chars, s.end()};
  }
  
  std::string right(const char *s, size_t chars) {
    if (s == nullptr) {return {}; };
    return right(std::string{s}, chars);
  }

  std::string replace_all(const std::string& str, const std::string& lookfor, const char* replwith) {
    std::string retval{};
    auto p = str.find(lookfor);
    if (p == std::string::npos || str.length() == 0 || lookfor.length() == 0) return str;
    auto ip = str.begin();
    while (p != -1 && ip < str.end()) {
      retval += (std::string{ip, str.begin() + p} + replwith);
      ip = str.begin() + (p + lookfor.length());
      p = str.find(lookfor, p+lookfor.length());
      if (p == std::string::npos ) break;
    }
    retval += std::string{ip, str.end()};
    return retval;
  }

  std::string replace_alli(const std::string& str, const std::string& lookfor, const char* replwith) {
    std::string retval{};
    std::string ustr{to_upper(str.c_str())}, ulookfor{to_upper(lookfor.c_str())};
    auto p = ustr.find(ulookfor);
    if (p == std::string::npos || str.length() == 0 || lookfor.length() == 0) return str;
    auto ip = str.begin();
    while (p != -1 && ip < str.end()) {
      retval += (std::string{ip, str.begin() + p} + replwith);
      ip = str.begin() + (p + lookfor.length());
      p = ustr.find(ulookfor, p+ulookfor.length());
      if (p == std::string::npos ) break;
    }
    retval += std::string{ip, str.end()};
    return retval;
  }

  std::string join(const std::vector<std::string>& v, const std::string& s2 = {} ) {
    std::string retval{};
    if (v.empty()) return retval;
    for (auto i = v.begin(); i != (v.end() - 1) ; ++i) {
      retval += (*i + s2);
    }
    retval += *(v.end() - 1);
    return retval;
  }
   
  std::string re_get_match(const std::string& re, const std::string& txt)  {
    if (re.empty() || txt.empty()) return {};
    std::regex _regex(re);
    std::smatch _match;
    if (std::regex_match(txt, _match, _regex)) {
      if (_match.size() >= 2) {
        return (_match[1]).str();
      }
    }
    return {};
  }

  std::string re_get_match(const char* re, const char* txt) { 
    if (re == nullptr || txt == nullptr) return {};
    return re_get_match(std::string{re}, std::string{txt});
  }

  bool starts_with(const char* str, const char* prefix) {
    if (str == nullptr || prefix == nullptr) { return false; }
    while (*str && *prefix && (*str == *prefix)) {
      str++;
      prefix++;
    }
    return (*prefix == 0);
  }

  bool ends_with(const char* str, const char* suffix) {
    if (str == nullptr || suffix == nullptr) { return false; }
    auto bstr = str + strlen(str);
    auto bsuf = suffix + strlen(suffix);
    while ((bstr != str) && (bsuf != suffix) && (*bsuf == *bstr)) {
      bstr--;
      bsuf--;
    }
    return (bsuf == suffix) && (*bstr == *bsuf);
  }

  bool re_does_match(const std::string& re, const std::string& txt)  {
    if (re.empty() || txt.empty()) return {};
    std::regex _regex(re);
    std::smatch _match;
    return std::regex_match(txt, _match, _regex);
  }

  bool re_does_match(const char* re, const char* txt)  {
    if (re == nullptr || txt == nullptr) return {};
    return re_does_match(std::string{re}, std::string{txt});
  }

};

