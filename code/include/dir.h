#ifndef __DIR_H
#define __DIR_H

#include <dirent.h>

struct dir_filelist {
    struct dirent **entries;
    size_t length;
};

int dir_find(const char *dir, const char *pattern, struct dir_filelist *result);
void dir_free(struct dir_filelist *result);

#endif
