#define FUSE_USE_VERSION 34

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include "util.h"

#define MNT_PATH "evil"

extern const char *evil_path;
extern int fuse_pipes[2];

int evil_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi);

int evil_getattr(const char *path, struct stat *stbuf,
             struct fuse_file_info *fi);

int evil_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
             off_t offset, struct fuse_file_info *fi,
             enum fuse_readdir_flags flags);