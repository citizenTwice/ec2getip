/*
 
 AWS Credentials loader
 
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

#include <fstream>
#include <iostream>
#include <array>
#include "citizentwice_str.h"

namespace str = citizentwice_str;

struct creds_t {
  bool        err = false;
  std::string err_msg = {};
  std::string access_key_id = {};
  std::string secret_access_key = {};

  static creds_t from_file(const char* filename) {
    creds_t retval;
    std::ifstream inis(filename, std::ios::in);
    if (!inis.is_open()) {
      retval.err = true;
      retval.err_msg = std::string{"Error opening "} + filename;      
      return retval;
    }
    bool section_found = false;
    while(!inis.eof()) {
      std::array<char,4096> buf;
      inis.getline(buf.data(), std::streamsize(buf.max_size()));
      if (inis.bad()) {
        retval.err = true;
        retval.err_msg = "I/O error";
        return retval;
      }
      std::string line{buf.data()};
      line = str::trim(line);
      if (str::left(line, 1) == ";") { continue; }
      if (str::re_does_match("\\[CREDENTIALS\\]", line)) {                                
        section_found = true; 
        continue; 
      }
      if (section_found) {
        std::string t;
        if (!(t = str::re_get_match("ACCESS_KEY_ID\\s*=\\s*(.*)",line)).empty()) {
          retval.access_key_id = t;
        } else if (!(t = str::re_get_match("SECRET_ACCESS_KEY\\s*=\\s*(.*)",line)).empty()) {
          retval.secret_access_key = t;
        }        
      }
    }
    if (retval.secret_access_key.empty() || retval.access_key_id.empty()) {
      retval.err = true;
      retval.err_msg = std::string{"Missing credentials in file "} + filename;
    }
    ///if (retval.)
    return retval; 
  }
  std::string to_string() {
    std::string s = {};
    s += "(\n";
    s += " error status:        " + std::to_string(err) + " : <" + err_msg  + ">\n";
    s += " access_key_id:       " + access_key_id + "\n";
    s += " secret_access_key:   " + ( secret_access_key.empty() ? 
      ("\n") :
      ("..." + str::right(secret_access_key, 2) + "\n"));
    s += ")\n";
    return s;
  }
};
