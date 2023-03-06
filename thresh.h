#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void thresh(int x) {
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    int pid;
    int max_fd;

    printf("## Offending processes:\n");

    dirp = opendir("/proc");
    while ((dp = readdir(dirp)) != NULL) {
        if (sscanf(dp->d_name, "%d", &pid) == 1) {
            sprintf(path, "/proc/%d/fd", pid);
            DIR *dirp2 = opendir(path);
            if (dirp2 != NULL) {
                max_fd = 0;
                struct dirent *dp2;
                while ((dp2 = readdir(dirp2)) != NULL) {
                    int fd;
                    if (sscanf(dp2->d_name, "%d", &fd) == 1 && fd > max_fd)
                        max_fd = fd;
                }
                closedir(dirp2);
                if (max_fd > x)
                    printf("%d (%d), ", pid, max_fd);
            }
        }
    }
    closedir(dirp);
}

// int main(){
//     thresh(6);
//     return 0;
// }


