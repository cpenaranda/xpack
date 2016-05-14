/*
 * prog_util.h - utility functions for programs
 *
 * Copyright 2016 Eric Biggers
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PROGRAMS_PROG_UTIL_H
#define PROGRAMS_PROG_UTIL_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "libxpack.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_defs.h"

#ifdef __GNUC__
# define _printf(str_idx, args_idx)	\
		__attribute__((format(printf, str_idx, args_idx)))
#else
# define _printf(str_idx, args_idx)
#endif

#ifdef _WIN32

/*
 * Definitions for Windows builds.  Mainly, 'tchar' is defined to be the 2-byte
 * 'wchar_t' type instead of 'char'.  This is the only "easy" way I know of to
 * get full Unicode support on Windows...
 */

#include <wchar.h>
extern int wmain(int argc, wchar_t **argv);
#  define	tmain		wmain
#  define	tchar		wchar_t
#  define	_T(text)	L##text
#  define	T(text)		_T(text)
#  define	TS		"ls"
#  define	TC		"lc"
#  define	tmemcpy		wmemcpy
#  define	topen		_wopen
#  define	tstrchr		wcschr
#  define	tstrcmp		wcscmp
#  define	tstrcpy		wcscpy
#  define	tstrlen		wcslen
#  define	tstrrchr	wcsrchr
#  define	tstrtoul	wcstoul
#  define	tstrxcmp	wcsicmp
#  define	tunlink		_wunlink
#  define	tutimbuf	_utimbuf
#  define	tutime		_wutime
#  ifdef _MSC_VER
     /* Standard file descriptors?  What are those? */
#    define	STDIN_FILENO	0
#    define	STDOUT_FILENO	1
#    define	STDERR_FILENO	2

     /* Fix the broken stat-related definitions. */
#    define	fstat		_fstat
#    define	stat		_stat
#    define	S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#    define	S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)
#  endif
#  define	O_NOFOLLOW	0

#else /* _WIN32 */

/* Standard definitions for everyone else */

#  define	tmain		main
#  define	tchar		char
#  define	T(text)		text
#  define	TS		"s"
#  define	TC		"c"
#  define	tmemcpy		memcpy
#  define	topen		open
#  define	tstrchr		strchr
#  define	tstrcmp		strcmp
#  define	tstrcpy		strcpy
#  define	tstrlen		strlen
#  define	tstrrchr	strrchr
#  define	tstrtoul	strtoul
#  define	tstrxcmp	strcmp
#  define	tunlink		unlink
#  define	tutimbuf	utimbuf
#  define	tutime		utime
#  define	O_BINARY	0

#endif /* !_WIN32 */

extern const tchar *program_invocation_name;

extern void _printf(1, 2) msg(const char *fmt, ...);
extern void _printf(1, 2) msg_errno(const char *fmt, ...);

extern void *xmalloc(size_t size);

extern u64 current_time(void);

extern const tchar *get_filename(const tchar *path);

struct file_stream {
	int fd;
	tchar *name;
	bool is_standard_stream;
};

extern int xopen_for_read(const tchar *path, struct file_stream *strm);
extern int xopen_for_write(const tchar *path, bool force,
			   struct file_stream *strm);

extern ssize_t xread(struct file_stream *strm, void *buf, size_t count);
extern int skip_bytes(struct file_stream *strm, size_t count);
extern int full_write(struct file_stream *strm, const void *buf, size_t count);

extern int xclose(struct file_stream *strm);

extern u32 parse_chunk_size(const tchar *arg);
extern int parse_compression_level(const tchar *arg);

extern struct xpack_compressor *alloc_compressor(u32 chunk_size, int level);
extern struct xpack_decompressor *alloc_decompressor(void);

/* tgetopt.c */

extern tchar *toptarg;
extern int toptind, topterr, toptopt;

extern int tgetopt(int argc, tchar *argv[], const tchar *optstring);

#endif /* PROGRAMS_PROG_UTIL_H */
