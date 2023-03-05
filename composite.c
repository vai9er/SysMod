#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    char link[1024];
    struct stat sb;

    printf("     PID     FD      Filename                Inode\n");
    printf("=================================================\n");

    dirp = opendir("/proc");
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] >= '0' && dp->d_name[0] <= '9') {
            sprintf(path, "/proc/%s/fd", dp->d_name);
            DIR *dirp2 = opendir(path);
            if (dirp2) {
                struct dirent *dp2;
                while ((dp2 = readdir(dirp2)) != NULL) {
                    if (dp2->d_name[0] >= '0' && dp2->d_name[0] <= '9') {
                        sprintf(path, "/proc/%s/fd/%s", dp->d_name, dp2->d_name);
                        memset(link, 0, sizeof(link));
                        readlink(path, link, sizeof(link)-1);
                        lstat(path,&sb);
                        printf("%10s %6s %22s %10ld\n", dp->d_name,
                               dp2->d_name,
                               link,
                               sb.st_ino);
                    }
                }
                closedir(dirp2);
            }
        }
    }
    closedir(dirp);

    return 0;
}