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

bool fsExtractBaseName(char *, const char *);
const char *fsExtractName(const char *);
const char *fsFollowNextPart(struct FsHandle *, struct FsNode **, const char *,
    bool);
FsCapacity fsFindUsedSpace(struct FsHandle *, struct FsNode *);
struct FsNode *fsFollowPath(struct FsHandle *, const char *, bool);
const char *fsGetChunk(char *, const char *);
void fsJoinPaths(char *, const char *, const char *);
struct FsNode *fsOpenBaseNode(struct FsHandle *, const char *);
struct FsNode *fsOpenNode(struct FsHandle *, const char *);
bool fsStripName(char *);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_FS_UTILS_H_ */
