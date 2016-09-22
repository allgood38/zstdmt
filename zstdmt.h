
/**
 * Copyright © 2016 Tino Reichardt
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * You can contact the author at:
 * - zstdmt source repository: https://github.com/mcmilk/zstdmt
 */

/* ***************************************
 * Defines
 ****************************************/

#ifndef ZSTDMT_H
#define ZSTDMT_H

#if defined (__cplusplus)
extern "C" {
#endif

#define ZSTDMT_THREAD_MAX 128

/* **************************************
 * Structures
 ****************************************/

typedef struct {
	void *buf;		/* ptr to data */
	int size;		/* length of buf */
} ZSTDMT_Buffer;

/**
 * reading and writing functions
 * - you can use stdio functions or plain read/write
 * - just write some wrapper on your own
 * - a sample is given in 7-Zip ZS
 */
typedef int (fn_read) (void *args, ZSTDMT_Buffer * in);
typedef int (fn_write) (void *args, ZSTDMT_Buffer * out);

typedef struct {
	fn_read *fn_read;
	void *arg_read;
	fn_write *fn_write;
	void *arg_write;
} ZSTDMT_RdWr_t;

/* **************************************
 * Compression
 ****************************************/

typedef struct ZSTDMT_CCtx_s ZSTDMT_CCtx;

/**
 * 1) allocate new cctx
 * - return cctx or zero on error
 *
 * @level   - 1 .. 22
 * @threads - 1 .. ZSTDMT_THREAD_MAX
 * @inputsize - if zero, becomes some optimal value for the level
 *            - if nonzero, the given value is taken
 */
ZSTDMT_CCtx *ZSTDMT_createCCtx(int threads, int level, int inputsize);

/**
 * 2) threaded compression
 * - return -1 on error
 */
int ZSTDMT_CompressCCtx(ZSTDMT_CCtx * ctx, ZSTDMT_RdWr_t * rdwr);

/**
 * 3) get some statistic
 */
size_t ZSTDMT_GetFramesCCtx(ZSTDMT_CCtx * ctx);
size_t ZSTDMT_GetInsizeCCtx(ZSTDMT_CCtx * ctx);
size_t ZSTDMT_GetOutsizeCCtx(ZSTDMT_CCtx * ctx);

/**
 * 4) free cctx
 * - no special return value
 */
void ZSTDMT_freeCCtx(ZSTDMT_CCtx * ctx);

/* **************************************
 * Decompression - TODO, but it's easy...
 ****************************************/

typedef struct ZSTDMT_DCtx_s ZSTDMT_DCtx;

/**
 * 1) allocate new cctx
 * - return cctx or zero on error
 *
 * @level   - 1 .. 22
 * @threads - 1 .. ZSTDMT_THREAD_MAX
 * @srclen  - the max size of src for ZSTDMT_CompressCCtx()
 * @dstlen  - the min size of dst
 */
ZSTDMT_DCtx *ZSTDMT_createDCtx(int threads);

/**
 * 2) threaded compression
 * - return -1 on error
 * - return zero on success
 * - will do all the compression, is calling the read/write wrapper...
 * - the wrapper should also do some progress, not only read/write
 */
int ZSTDMT_DecompressDCtx(ZSTDMT_DCtx * ctx, ZSTDMT_RdWr_t * rdwr);

/**
 * 3) get some statistic, fail only when ctx is invalid
 */
size_t ZSTDMT_GetFramesDCtx(ZSTDMT_DCtx * ctx);
size_t ZSTDMT_GetInsizeDCtx(ZSTDMT_DCtx * ctx);
size_t ZSTDMT_GetOutsizeDCtx(ZSTDMT_DCtx * ctx);

/**
 * 4) free cctx
 * - no special return value
 */
void ZSTDMT_freeDCtx(ZSTDMT_DCtx * ctx);

#if defined (__cplusplus)
}
#endif
#endif				/* ZSTDMT_H */