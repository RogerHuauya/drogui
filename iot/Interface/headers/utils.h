#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <cstdlib>
#include <sstream>
#include <zlib.h>
#include <stdexcept>

#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)     "\033[1;37m"#n"\033[0m"

std::string compress_string(const std::string& str,int);
std::string decompress_string(const std::string& str);
#endif