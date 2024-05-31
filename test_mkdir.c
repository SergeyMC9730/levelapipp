#define _GNU_SOURCE

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

// https://stackoverflow.com/questions/2336242/recursive-mkdir-system-call-on-unix
int mkpath(char *dir, mode_t mode)
{
    struct stat sb;

    if (!dir) {
        errno = EINVAL;
        return 1;
    }

    if (!stat(dir, &sb))
        return 0;

    mkpath(dirname(strdupa(dir)), mode);

    return mkdir(dir, mode);
}

int main() {
    const char *path = "test/1/2/3/4";

    printf("[LevelAPI TEST] Creating folder with path: %s\n", path);

    int res = mkpath((char *)path, 0777);

    printf("[LevelAPI TEST] mkpath returned: %d\n", res);

    return 0;
}