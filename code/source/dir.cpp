#include <dir.h>
#include <stdlib.h>
#include <stdio.h>

int dir_find(const char *dir, const char *pattern, struct dir_filelist *result)
{
    // TODO: filter by glob pattern
    int res = scandir(dir, &result->entries, NULL, alphasort);
    if (res < 0) {
        perror("scandir");
        return 0;
    }
    result->length = res;
    return 1;
}

void dir_free(struct dir_filelist *result) {
    for (size_t i = 0; i < result->length; ++i)
        free(result->entries[i]);
    free(result->entries);
}
