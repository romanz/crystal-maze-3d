#ifndef __DIR_H
#define __DIR_H

struct ffblk_t {
    const char *ff_name;
};
typedef struct ffblk_t ffblk;

extern int findfirst(const char *pattern, ffblk *ff, int flag);
extern int findnext(ffblk *ff);

#endif
