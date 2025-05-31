/*
 * xcore/fs/utils.h
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

/**
 * @file
 * Helper functions for file system interface.
 */

#ifndef XCORE_FS_UTILS_H_
#define XCORE_FS_UTILS_H_
/*----------------------------------------------------------------------------*/
#include <xcore/fs/fs.h>
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

/* TODO Doxygen */
bool fsExtractBaseName(char *buffer, const char *path);
const char *fsExtractName(const char *path);
const char *fsFollowNextPart(struct FsHandle *handle, struct FsNode **node,
    const char *path, bool leaf);
FsCapacity fsFindUsedSpace(struct FsHandle *handle, struct FsNode *node);
struct FsNode *fsFollowPath(struct FsHandle *handle, const char *path,
    bool leaf);
const char *fsGetChunk(char *dst, const char *src);
void fsJoinPaths(char *buffer, const char *directory, const char *path);
struct FsNode *fsOpenBaseNode(struct FsHandle *handle, const char *path);
struct FsNode *fsOpenNode(struct FsHandle *handle, const char *path);
bool fsStripName(char *buffer);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_FS_UTILS_H_ */
