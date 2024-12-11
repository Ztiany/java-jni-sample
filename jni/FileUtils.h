#ifndef CFORJNI_FILEUTILS_H
#define CFORJNI_FILEUTILS_H

#include <stdbool.h>

bool splitFile(const char* path, const char* path_pattern, int file_num);

bool mergeFile(const char* path_pattern, int file_num, const char* merge_path);

#endif
