#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void thresh(int x) {
    printf("## Offending processes:\n");
    DIR *dirp = opendir("/proc");
    struct dirent *dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] >= '0' && dp->d_name[0] <= '9') {
            char path[256];
            snprintf(path, sizeof(path), "/proc/%s", dp->d_name);
            struct stat sb;
            if (lstat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                snprintf(path, sizeof(path), "/proc/%s/fd", dp->d_name);
                DIR *fd_dir = opendir(path);
                if (fd_dir != NULL) {
                    int count = 0;
                    while ((readdir(fd_dir)) != NULL) count++;
                    if (count > x) printf("%s (%d), ", dp->d_name, count);
                    closedir(fd_dir);
                }
            }
        }
    }
    closedir(dirp);
}

int main(){
    thresh(33);
    return 0;
}