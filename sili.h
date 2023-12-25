/*
sili.h - a cross-platform standard library for modern C99 programming
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.

	- If you want to disable certain features, you can do:
		- #define SI_NO_ALLOCATOR
		- #define SI_NO_PAIR
		- #define SI_NO_OPTIONAL
		- #define SI_NO_ARRAY
		- #define SI_NO_STRING
		- #define SI_NO_UNICODE
		- #define SI_NO_CHAR
		- #define SI_NO_HASHTABLE
		- #define SI_NO_IO
		- #define SI_NO_THREAD
		- #define SI_NO_BIT
		- #define SI_NO_CPU
		- #define SI_NO_PRINT
		- #define SI_NO_DLL
		- #define SI_NO_MATH
		- #define SI_NO_BENCHMARK
	before the SI_IMPLEMENTATION macro, as well as before any other include of
	`sili.h`.

	- NOTE: Disabling certain features may or may not create 'undefined'
	compiler errors at certain spots. Use it at your own risk.

===========================================================================
DOCUMENTATION
	- All functions, constant variables and macros contain a comment with a description
	of what they do above them, as well what they return (if anything). Macros
	specifically use a consistent format because of their lack of typing.
	That being:
		/ argumentName - type | otherArgumentName - KEYWORD | ...VALUES - TYPE*
		description of the macro. /
		#define smth(argumentName, otherArgumentName, .../ VALUES/)

	- More often than not a macro's argument will not be a specific type and instead
	some kind of 'text'. Such arguments are noted if their type denotation is
	A FULLY CAPITALIZED KEYWORD. A general list of the keywords, what they mean
	and examples what to input:
		- TYPE - argument is just the type name (siString, usize, rawptr).
		- TYPE* - same as TYPE except it's a pointer (siString*, usize*, rawptr*).
		- INT - argument can be any integer (50, -250LL, ISIZE_MAX).
		- UINT - argument can be any UNSIGNED integer (50, 250LL, USIZE_MAX).
		- EXPRESSION - argument is just some kind of value (60, "hello", SI_RGB(255, 255, 255)).
		- NAME - argument has to be regular text with _no_ enquotes (test, var, len).
		- ANYTHING - argument can be literally anything.

===========================================================================
MACROS
	- For any of the macros to work, you must _always_ define it before including
	the library. Example:
	```c
		#define SI_IMPLEMENTATION
		#define SI_RELEASE_MODE
		#include "sili.h"
	```

	- SI_NO_ASSERTIONS_IN_HEADER - SI_ASSERT() and its other variations do not
	check for the assertion, essentially doing nothing, meaning any given expression
	will evaluate to nothing and not crash the app. Slightly increases the performance
	of the app, but should only be enabled for release builds, as any error in
	the code will then not get caught and will most likely crash the app somewhere
	else further down.

	- SI_NO_ALLOC_DEBUG_INFO - by default si_malloc() and similar functions take
	__FILE__ and __LINE__ arguments for debugging. For performance reasons, you
	can disable this.

	- SI_RELEASE_MODE - same as defining both SI_NO_ASSERTIONS_IN_HEADER and SI_NO_ALLOC_DEBUG_INFO.

	- SI_NO_ERROR_STRUCT - by default siErrorInfo is a struct that contains an
	i32 error code, time in UTC+0 and a cstring of the last function that edited
	the structure. Defining this macro makes siErrorInfo only contain an error code.

	- SI_NO_INLINE_ASM - disables any usage of inline ASM in sili.h (when possible).

	- SI_ASM_USE_ATT_SYNTAX - makes 'si_asm' understand AT&T syntax, as by default
	the Intel syntax is used.

	- SI_NO_WINDOWS_H - disables the inclusion of windows.h inside sili.h. Might
	cause 'undefined' compiler errors.

===========================================================================
CREDITS
	- Ginger Bill's 'gb.h' (https://github.com//gingerBill/gb) - inspired me to
	make the Sili Toolchain, as well as certain features were taken from the
	library and implemented here.

LICENSE:
	- This software is licensed under the zlib license (see the LICENSE in the
	bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as
	expected.
	- This also means that functions may not be documented.

*/

#ifndef SI_INCLUDE_SI_H
#define SI_INCLUDE_SI_H

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
	#define SI_EXTERN extern "C"
#else
	#define SI_EXTERN extern
#endif

#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && !defined(_WIN32))
	#define SI_SYSTEM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
	#define SI_SYSTEM_APPLE 1
	#include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR == 1
		#define SI_PLATFORM_IOS 1
	#elif TARGET_OS_IPHONE == 1
		#define SI_PLATFORM_IOS 1
	#elif TARGET_OS_MAC
		#define SI_SYSTEM_OSX 1
	#endif
#elif defined(__unix__)
	#define SI_SYSTEM_UNIX 1

	#if defined(__ANDROID__)
		#define SI_SYSTEM_ANDROID 1
	#elif defined(__EMSCRIPTEN__)
		#define SI_SYSTEM_WASM 1
	#elif defined(__linux__)
		#define SI_SYSTEM_LINUX 1
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define SI_SYSTEM_FREEBSD 1
	#endif
#elif
#endif

#if defined(_MSC_VER)
	#define SI_COMPILER_MSVC 1
#elif defined(__clang__)
	#define SI_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#define SI_COMPILER_GCC 1
#else
	#warning "Unknown compiler"
#endif

#if defined(__GNUC__)
	#define SI_GNUC_COMPLIANT 1
#endif


#if defined(__cplus__)
	#define SI_LANGUAGE_CPLUS 1
#elif defined(__OBJC__)
	#define SI_LANGUAGE_OBJ_C 1
	#if defined(__cplusplus)
		#define SI_LANGUAGE_CPP 1
		#define SI_LANGUAGE_OBJ_CPP 1
	#endif
#elif defined(__cplusplus)
	#define SI_LANGUAGE_CPP 1
#else
	#define SI_LANGUAGE_C 1
#endif


#if defined(__STDC__)
	#if !defined(SI_LANGUAGE_CPP)
		#if defined(__STDC_VERSION__)
			#define SI_STANDARD_VERSION __STDC_VERSION__
		#else
			#define SI_STANDARD_VERSION 198900L
		#endif

		#define SI_STANDARD_C89 198900L
		#define SI_STANDARD_C94 199409L
		#define SI_STANDARD_C99 199901l
		#define SI_STANDARD_C11 201112L
		#define SI_STANDARD_C17 201710L
		#define SI_STANDARD_C23 202300L
	#else
		#define SI_STANDARD_VERSION __cplusplus

		#define SI_STANDARD_CPP98 199711L
		#define SI_STANDARD_CPP11 201103L
		#define SI_STANDARD_CPP14 201402L
		#define SI_STANDARD_CPP17 201703L
		#define SI_STANDARD_CPP20 202002L
		#define SI_STANDARD_CPP23 202311L
	#endif
#endif

#if defined(__STRICT_ANSI__)
	#define SI_STANDARD_ANSI 1
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__)
	#define SI_CPU_X86 1
	#define SI_CACHE_LINE_SIZE 64

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__) || defined(_ARCH_PPC)
	#define SI_CPU_PPC 1
	#define SI_CACHE_LINE_SIZE 128

#elif defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_ARM) || defined(_M_ARM) || defined(__arm) || defined(__aarch64__)
	#define SI_CPU_ARM 1
	#define SI_CPU_ARM64 __aarch64__
	#define SI_CACHE_LINE_SIZE 64

#elif defined(__mips__) || defined(__mips) || defined(__MIPSEL__) || defined(__mips_isa_rev)
	#define SI_CPU_MIPS 1
	#define SI_CACHE_LINE_SIZE 64

#elif defined(__sparc__) || defined(__sparc)
	#define SI_CPU_SPARC 1
	#define SI_CACHE_LINE_SIZE 64

#elif defined(__riscv)
	#define SI_CPU_RISC_V 1
	#define SI_CACHE_LINE_SIZE __riscv_xlen

#elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
	#define SI_CPU_ALPHA 1
	#define SI_CACHE_LINE_SIZE 64 /* NOTE(EimaMei): Sorta correct, sorta not really. Beware. */

#else
	#warning "Unknown CPU Type. Proceeding with caution."
	#define SI_CACHE_LINE_SIZE 0
#endif

#if 1
	#define SI_STATIC_ASSERT2(condition, msg)  typedef char static_assertion_##msg[(!!(condition)) * 2 - 1]
	#define SI_STATIC_ASSERT1(condition, line) SI_STATIC_ASSERT2(condition, line)
#endif
#define SI_STATIC_ASSERT(cond) SI_STATIC_ASSERT1(cond, __LINE__)


#if defined(SI_SYSTEM_WINDOWS)
	#if !defined(_CRT_SECURE_NO_WARNINGS)
		#define _CRT_SECURE_NO_WARNINGS
	#endif
#endif

#if defined(SI_SYSTEM_UNIX) || defined(SI_SYSTEM_APPLE)
	#if !defined(_GNU_SOURCE)
		#define _GNU_SOURCE
	#endif
	#if !defined(_LARGEFILE64_SOURCE)
		#define _LARGEFILE64_SOURCE
	#endif
	#if !defined(__USE_POSIX199309)
		#define __USE_POSIX199309
	#endif
	#if !defined(__USE_XOPEN_EXTENDED)
		#define __USE_XOPEN_EXTENDED
	#endif
#endif

#if !defined(SI_SYSTEM_WINDOWS)
	#include <time.h>
	#include <stdlib.h>
	#include <memory.h>
	#include <stdarg.h>
	#include <stddef.h>
	#include <errno.h>
	#include <pthread.h>
	#include <unistd.h>
	#include <dirent.h>

	#include <sys/stat.h>
	#include <sys/fcntl.h>
	#include <sys/time.h>

	#include <stdio.h>
	#include <stdint.h>
#else
	#if !defined(SI_NO_WINDOWS_H)
		#define NOMINMAX            1
		#define WIN32_LEAN_AND_MEAN 1
		#define WIN32_MEAN_AND_LEAN 1
		#define VC_EXTRALEAN        1
		#include <windows.h>
		#include <aclapi.h>
		#undef NOMINMAX
		#undef WIN32_LEAN_AND_MEAN
		#undef WIN32_MEAN_AND_LEAN
		#undef VC_EXTRALEAN
	#endif

	#include <malloc.h>
	#include <intrin.h>
	#include <time.h>
#endif

#if defined(SI_SYSTEM_OSX)
	#include <sys/socket.h>
#endif

#if defined(SI_SYSTEM_UNIX)
	#include <sys/sendfile.h>
#endif


#if UINTPTR_MAX == UINT64_MAX
	#define SI_ARCH_64_BIT 1
#else
	#define SI_ARCH_32_BIT 1
#endif

#define SI_HOST_IS_BIG_ENDIAN      (!*(u8*)&(u16){1})
#define SI_HOST_IS_LITTLE_ENDIAN   (!SI_HOST_IS_BIG_ENDIAN)

#if defined(SI_RELEASE_MODE)
	#undef SI_NO_ASSERTIONS_IN_HEADER
	#under SI_NO_ALLOC_DEBUG_INFO

	#define SI_NO_ASSERTIONS_IN_HEADER
	#define SI_NO_ALLOC_DEBUG_INFO
#endif

#if defined(SI_SYSTEM_WINDOWS)
	#if _MSC_VER < 1300
		typedef unsigned char     u8;
		typedef   signed char     i8;
		typedef unsigned short   u16;
		typedef   signed short   i16;
		typedef unsigned int     u32;
		typedef   signed int     i32;
	#else
		typedef unsigned __int8   u8;
		typedef   signed __int8   i8;
		typedef unsigned __int16 u16;
		typedef   signed __int16 i16;
		typedef unsigned __int32 u32;
		typedef   signed __int32 i32;
	#endif

	typedef unsigned __int64 u64;
	typedef   signed __int64 i64;

	#define INT8_MIN     ((i8)_I8_MIN)
	#define INT8_MAX     _I8_MAX
	#define INT16_MIN    ((i16)_I16_MIN)
	#define INT16_MAX    _I16_MAX
	#define INT32_MIN    ((i32)_I32_MIN)
	#define INT32_MAX    _I32_MAX
	#define INT64_MIN    ((i64)_I64_MIN)
	#define INT64_MAX    _I64_MAX
	#define UINT8_MAX    _UI8_MAX
	#define UINT16_MAX   _UI16_MAX
	#define UINT32_MAX   _UI32_MAX
	#define UINT64_MAX   _UI64_MAX

#else
	typedef uint8_t   u8;
	typedef  int8_t   i8;
	typedef uint16_t u16;
	typedef  int16_t i16;
	typedef uint32_t u32;
	typedef  int32_t i32;
	typedef uint64_t u64;
	typedef  int64_t i64;
#endif

#if !defined(usize)
	typedef size_t      usize;
	typedef ptrdiff_t   isize;
#endif

#if !defined(uintptr)
	typedef usize  uintptr;
	typedef isize  intptr;
#endif

typedef u8 siByte;

#if defined(SI_ARCH_64_BIT)
	#define USIZE_MAX UINT64_MAX
	#define USIZE_MIN UINT64_MIN
	#define ISIZE_MAX INT64_MAX
	#define ISIZE_MIN INT64_MIN
#else
	#define USIZE_MAX UINT32_MAX
	#define USIZE_MIN UINT64_MIN
	#define ISIZE_MAX UINT32_MAX
	#define ISIZE_MIN UINT32_MIN
#endif


SI_STATIC_ASSERT(sizeof(u8) == sizeof(i8));
SI_STATIC_ASSERT(sizeof(u16) == sizeof(i16));
SI_STATIC_ASSERT(sizeof(u32) == sizeof(i32));
SI_STATIC_ASSERT(sizeof(u64) == sizeof(i64));
SI_STATIC_ASSERT(sizeof(usize) == sizeof(isize));

SI_STATIC_ASSERT(sizeof(u8)  == 1);
SI_STATIC_ASSERT(sizeof(u16) == 2);
SI_STATIC_ASSERT(sizeof(u32) == 4);
SI_STATIC_ASSERT(sizeof(u64) == 8);
SI_STATIC_ASSERT(sizeof(usize) == sizeof(size_t));


typedef float  f32;
typedef double f64;

SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);

#define SI_F32_MIN (1.17549435e-38f)
#define SI_F32_MAX (3.40282347e+38f)

#define SI_F64_MIN (2.2250738585072014e-308)
#define SI_F64_MAX (1.7976931348623157e+308)


#if !defined(b8)
	typedef u8   b8; /* NOTE(EimaMei): Don't use this as the primary boolean type. */
	typedef u16 b16;
	typedef u32 b32; /* NOTE(EimaMei): Use this as the default! */
	typedef u64 b64;
#endif

#if !defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION <= SI_STANDARD_C17
	#ifndef true
		#define true 1
	#endif
	#ifndef false
		#define false 0
	#endif
#endif

SI_STATIC_ASSERT(sizeof(b8)  == 1);
SI_STATIC_ASSERT(sizeof(b16) == 2);
SI_STATIC_ASSERT(sizeof(b32) == 4);
SI_STATIC_ASSERT(sizeof(b64) == 8);

SI_STATIC_ASSERT(true == 1);
SI_STATIC_ASSERT(false == 0);


#if !defined(SI_LANGUAGE_CPP)
	#if defined(_MSC_VER) && _MSC_VER <= 1800
		#define inline __inline
	#elif !defined(__STDC_VERSION__)
		#define inline __inline__
	#else
		#define inline
	#endif
#endif

#ifdef intern
	#undef intern
#endif
#define intern static

#if defined(SI_COMPILER_MSVC)
	/* asm - ASSEMBLY
	 * Inserts inline assembly into the program using MSVC assembly syntax. */
	#define si_asm(... /* asm */) __asm  { __VA_ARGS__ }
#else
	#if defined(SI_CPU_X86) && defined(SI_GNUC_COMPLIANT)
		/* asm - cstring | ...IOR - ASM INPUT, OUTPUT OR REGISTERS
		 * Inserts inline assembly into the program using GNU C assembly syntax. */
		#define si_asm(asm, .../* IOR */) \
			__asm__ __volatile__( \
				".intel_syntax noprefix" SI_ASM_NL \
				asm SI_ASM_NL \
				".att_syntax" \
				__VA_ARGS__ \
				)

		#define SI_ASM_INPUT(...) : __VA_ARGS__
		#define SI_ASM_OUTPUT(...) : __VA_ARGS__
		#define SI_ASM_REGISTERS(...) : __VA_ARGS__
		#define SI_ASM_NL "\n"
	#else
		/* asm - cstring | ...IOR - ASM INPUT, OUTPUT OR REGISTERS
		 * Inserts inline assembly into the program using GNU C assembly syntax. */
		#define si_asm(asm, .../ * IOR */) __asm__ __volatile__(asm __VA_ARGS__)
	#endif
#endif



#if (defined(SI_LANGUAGE_CPP) && (SI_STANDARD_VERSION >= SI_STANDARD_CPP17)) || (!defined (SI_LANGUAGE_CPP) && SI_STANDARD_VERSION > SI_STANDARD_C17)
	/* Specifies a fallthrough for the compiler. */
	#define fallthrough [[fallthrough]]
#elif defined(SI_GNUC_COMPLIANT)
	/* Specifies a fallthrough for the compiler. */
	#define fallthrough __attribute__((fallthrough))
#else
	/* Specifies a fallthrough for the compiler. */
	#define fallthrough do {} while (0)
#endif


#if !(defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION >= SI_STANDARD_C99)
	#if defined(SI_GNUC_COMPLIANT)  && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
		#define restrict __restrict
	#elif defined(_MSC_VER) && _MSC_VER >= 1400
		#define restrict __restrict
	#else
		#define restrict
	#endif
#endif


#if !defined(rawptr)
	/* A pointer type pointing to raw data. */
	typedef void* rawptr;
#endif

#if !defined(cstring)
	/* A const C-string type. */
	typedef const char* cstring;
#endif

	/* A struct containing information about an error that happened during an operation. */
typedef struct {
	/* Error code from the operation. */
	i32 code;

#if !defined(SI_NO_ERROR_STRUCT)
	/* The function where the error occurred. */
	cstring function;
	/* The time when the error happened (in UTC+0). */
	i64 time;
#endif
} siErrorInfo;

/*
	========================
	| Constants            |
	========================
*/

/* Output is successful. */
#define SI_OKAY  (0)
/* Output is unsuccessful. */
#define SI_ERROR (-1)

/* x - INT.
 * Converts kilobytes into bytes. */
#define SI_KILO(x) ((usize)(x) * 1024)
/* x - INT.
 * Converts megabytes into bytes. */
#define SI_MEGA(x) (SI_KILO(x) * 1024)
/* x - INT.
 * Converts gigabytes into bytes. */
#define SI_GIGA(x) (SI_MEGA(x) * 1024)
/* x - INT.
 * Converts terabytes into bytes. */
#define SI_TERA(x) (SI_GIGA(x) * 1024)

/* x - UINT
 * Shortcut for (1 << x). */
#define SI_BIT(x) ((usize)1 << si_cast(usize, x))

#if !defined(nil)
	/* A nothing value, equivalent to NULL. */
	#define nil ((void*)0)
#endif

SI_STATIC_ASSERT(sizeof(nil) == sizeof(void*));

/*
	========================
	| Builtin functions    |
	========================
*/
#if !defined(__has_builtin)
	#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_unreachable)
	#define SI_BUILTIN_UNREACHABLE() __builtin_unreachable()
#else
	#define SI_BUILTSI_BUILTIN_UNREACHABLE() SI_PANIC()
#endif


/*
	========================
	| Declaration macros   |
	========================
*/

/* ...KEYWORDS - C KEYWORDS
 * Syntactic sugar to differentiate the keywords from the return type. */
#define F_TRAITS(.../* KEYWORDS */) __VA_ARGS__

/* type - TYPE | name - NAME
 * Defines the enum with the given integer type. */
#define SI_ENUM(type, name) type name; enum
/* varName - NAME | multipleArgs - (TYPE1, TYPE2...)
 * Macro to define a function pointer. */
#define SI_FUNC_PTR(varName, multipleArgs) (*varName)multipleArgs
/* x - ANYTHING
 * Silences the unused warnings for a given variable. */
#define SI_UNUSED(x) (void)(x)
/* label - NAME
 * Used for adding additional context to a code block. Does not affect the actual
 * code, it's just documentation. */
#define SI_LABEL(label)
/* label - NAME
 * Creates a jump label while also fixing an annoying C error, where trying to
 * create variables in the label scope is illegal. */
#define SI_GOTO_LABEL(label) label: (void)((int)0);

/*
	========================
	| Macro related        |
	========================
*/

/* a - ANYTHING | b - ANYTHING
 * Combines 'a' and 'b' into 'ab' for macro shenanigans. */
#define SI_CAT(a, b) a##b


/*
	========================
	| Casting              |
	========================
*/

/* type - TYPE | value - EXPRESSION
 * Casts a value to the type provided. */
#define si_cast(type, value) ((type)((value)))
/* type - TYPE | value - EXPRESSION
 * Bit cast converts the value with the same size types. */
#define si_transmute(type, value) ((union { typeof(value) in; type out; }){value}.out)
/* text - cstring
 * Converts a cstring into an u8. */
#define SI_CSTR_U8(text)  (*si_cast(u8*, text))
/* text - cstring
 * Converts a cstring into an u16. */
#define SI_CSTR_U16(text) (*si_cast(u16*, text))
/* text - cstring
 * Converts a cstring into an u32. */
#define SI_CSTR_U32(text) (*si_cast(u32*, text))
/* text - cstring
 * Converts a cstring into an u64. */
#define SI_CSTR_U64(text) (*si_cast(u64*, text))
/* text - cstring | bytes - usize
 * Converts a cstring into an u64 containing 'bytes' amount of data from the string.
 * Maximum byte count is 8. */
#define SI_CSTR_UBYTE(text, bytes) si_u64FromPtr(text, bytes)


/*
	========================
	| Unary operators      |
	========================
*/
#if !defined(typeof) && !(defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION > SI_STANDARD_C17)
	/* ...VALUE - TYPE/EXPRESSION
	* Gets the value's type and expresses it as just a regular type. */
	#define typeof(.../* VALUE */)  __typeof__(__VA_ARGS__)
#endif

#if !defined(countof)
	/* value - ARRAY
	* Gets the static length of the given value (must be an array). */
	#define countof(value) (sizeof(value) / sizeof((value)[0]))
#endif

#if !defined(si_offsetof)
	/* type - STRUCT TYPE | element - TYPE's member
	* Get's the offset of the provided member. Doesn't work as a compile-time macro. */
	#define si_offsetof(type, element) ((isize)&(((type*)nil)->element))
#endif

#if (!defined(SI_LANGUAGE_CPP) && !defined(alignof)) || (defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION < SI_STANDARD_CPP11)
	/* type - TYPE
	* Gets the alignment of a type. */
	#define alignof(type) offsetof(struct { char c; type member; }, member)
#endif


/*
	========================
	| General macros       |
	========================
*/

/* bytes - usize
 * Allocates the memory into the stack. */
#define si_salloc(bytes) si_cast(rawptr, (siByte[bytes]){0})
/* type - TYPE/EXPRESSION
 * Allocates 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocItem(type) (type*)si_salloc(sizeof(type))
/* type - TYPE/EXPRESSION | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocArray(type, count) (type*)si_salloc(sizeof(type) * count)
/* allocator - siAllocator* | type - TYPE
 * Allocates 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_mallocItem(allocator, type) (type*)si_malloc(allocator, sizeof(type))
/* allocator - siAllocator* | type - TYPE | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_mallocArray(allocator, type, count) (type*)si_malloc(allocator, sizeof(type) * (count))
/* allocator - siAllocator* | pointer - REGULAR POINTER
 * Allocates 'sizeof(variable)' bytes to the allocator and copies the value of
 * 'variable' to the allocated memory. Variable _must_ be a pointer. */
#define si_mallocCopy(allocator, pointer) \
	memcpy( \
		si_mallocItem(allocator, typeof(*(pointer))),  \
		pointer , \
		sizeof(typeof(*(pointer))) \
	)
/* type - TYPE | ...VALUES - <EXPRESSION>, [EXPRESSION] ...
 * Declares a static buffer. */
#define si_buf(type, .../* VALUES */) (type[]){__VA_ARGS__}
/* a - VARIABLE | b - VARIABLE
 * Swaps the value of 'a' with 'b'; 'b' with 'a'. */
#define si_swap(a, b) do { typeof((a)) tmp = (a); (a) = (b); (b) = tmp; } while (0)
/* Pauses the thread and waits for the user to press enter in the terminal. */
#define si_pause() do { si_print("Press any key to continue..."); getchar(); } while(0)
/* x - EXPRESSION
 * Checks if value is nil/zero. */
#define si_isNil(x) ((x) == 0)
#if 1
	#define for_range_2(countVar, struct_name, start, end) \
		siPointS struct_name = {(isize)start, (isize)end}; \
		for (isize countVar = struct_name.x; countVar < struct_name.y; countVar += 1)
	#define for_range_1(countVar, line, start, end) \
		for_range_2(countVar, SI_CAT(__si_impl_range_, line), start, end)
#endif
/* countvar - NAME | start - INT | end - INT
 * A loop that loops from 'start' to 'end' and increments 'countVar' each cycle. */
#define for_range(countVar, start, end) \
	for_range_1(countVar, __LINE__, start, end)


/*
	========================
	| Endian swap macros   |
	========================
*/

/* x - u16
 * Swaps the endianess of a 16-bit number. */
#define si_swap16(x) \
	((u16)((((x) >> 8) & 0xFF) | (((x) & 0xFF) << 8)))
/* x - u32
 * Swaps the endianess of a 32-bit number. */
#define si_swap32(x)	\
	((((x) & 0xFF000000U) >> 24) | (((x) & 0x00FF0000U) >> 8) \
	| (((x) & 0x0000FF00U) << 8) | (((x) & 0x000000FFU) << 24))
/* x - u64
 * Swaps the endianess of a 64-bit number. */
#define si_swap64(x) \
   ((((x) &  (u64)0xFF00000000000000) >> 56) \
   | (((x) & (u64)0x00FF000000000000) >> 40) \
   | (((x) & (u64)0x0000FF0000000000) >> 24) \
   | (((x) & (u64)0x000000FF00000000) >> 8) \
   | (((x) & (u64)0x00000000FF000000) << 8)	\
   | (((x) & (u64)0x0000000000FF0000) << 24) \
   | (((x) & (u64)0x000000000000FF00) << 40) \
   | (((x) & (u64)0x00000000000000FF) << 56))
/* x - u16
 * Changes the endianess of the number to big-endian. Does nothing if the host is big-endian. */
#define si_swap16be(x) ((SI_HOST_IS_LITTLE_ENDIAN) ? si_swap16(x) : x)
/* x - u32
 * Changes the endianess of the number to big-endian. Does nothing if the host is big-endian. */
#define si_swap32be(x) ((SI_HOST_IS_LITTLE_ENDIAN) ? si_swap32(x) : x)
/* x - u64
 * Changes the endianess of the number to big-endian. Does nothing if the host is big-endian. */
#define si_swap64be(x) ((SI_HOST_IS_LITTLE_ENDIAN) ? si_swap64(x) : x)

/* x - u16
 * Changes the endianess of the number to little-endian. Does nothing if the host is little-endian. */
#define si_swap16le(x) ((SI_HOST_IS_BIG_ENDIAN) ? si_swap16(x) : x)
/* x - u32
 * Changes the endianess of the number to little-endian. Does nothing if the host is little-endian. */
#define si_swap32le(x) ((SI_HOST_IS_BIG_ENDIAN) ? si_swap32(x) : x)
/* x - u64
 * Changes the endianess of the number to little-endian. Does nothing if the host is little-endian. */
#define si_swap64le(x) ((SI_HOST_IS_BIG_ENDIAN) ? si_swap64(x) : x)

/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| Mics/General         |
	========================
*/

/*
	========================
	| SI_ASSERT            |
	========================
*/
usize si_assertEx(b32 condition, cstring conditionStr, cstring file, i32 line,
		cstring func, cstring message, ...);

#if !defined(SI_NO_ASSERTIONS_IN_HEADER)
/* condition - EXPRESSION
 * Crashes the app if the condition is not met. */
#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
/* condition - EXPRESSION | message - cstring
 * Crashes the app with a message if the condition is not met. */
#define SI_ASSERT_MSG(condition, message) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, "")
/* condition - EXPRESSION | message - cstring | ...FMT - VARIADIC ARGUMENTS
 * Crashes the app with a formatted message if the condition is not met. */
#define SI_ASSERT_FMT(condition, message, ...) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__)
/* ptr - rawptr
 * Crashes the app if a pointer is NULL. */
#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")
/* path - cstring
 * Crashes the app if the provided path doesn't exist. */
#define SI_ASSERT_PATH_EXISTS(path) SI_ASSERT_FMT(si_pathExists(path), "Path '%s' doesn't exist.", path)
#else
#define SI_ASSERT(condition) do {} while(0)
#define SI_ASSERT_MSG(condition, message) do {} while(0)
#define SI_ASSERT_FMT(condition, message, ...) do {} while(0)
#define SI_ASSERT_NOT_NULL(ptr) do {} while(0)
#define SI_ASSERT_PATH_EXISTS(path) do {} while(0)
#endif /* SI_NO_ASSERTIONS_IN_HEADER */

/* Crashes the app immediately. */
#define SI_PANIC() si_assertEx(false, "SI_PANIC()", __FILE__, __LINE__, __func__, nil)
/* message - cstring
 * Crashes the app immediately with a message. */
#define SI_PANIC_MSG(message) si_assertEx(false, "SI_PANIC()", __FILE__, __LINE__, __func__, message, "")
/* condition - EXPRESSION | ACTION - ANYTHING
 * Checks if the condition is true. If it is, execute 'action'. */
#define SI_STOPIF(condition, .../* ACTION */) if (condition) { __VA_ARGS__; } do {} while(0)


/*
	========================
	| siAny                |
	========================
*/

typedef struct {
	/* Pointer to the data. */
	rawptr ptr;
	/* Size of the data */
	usize typeSize;
} siAny;

/* ...VALUE - EXPRESSION
 * Creates a 'siAny' object. Said object's pointer is just "&(<VALUE>)". */
#define si_anyMake(.../* VALUE */) \
	(siAny){ \
		(rawptr)&((struct { typeof(__VA_ARGS__) in; }){__VA_ARGS__}.in), \
		sizeof(typeof(__VA_ARGS__)) \
	}

/*
	========================
	| siGeneric/siFatPtr   |
	========================
*/

/* A generic fat pointer header, which stores a length. Usually good enough for
 * most things. */
typedef struct {
	usize len;
} siGenericHeader;

/* x - generic's original pointer
 * A macro to access the generic header. */
#define SI_GENERIC_HEADER(x) ((siGenericHeader*)x - 1)
/* type - TYPE
 * Denotes that this is a generic fat pointer. */
#define siGenericFat(type) type*


/* headerType - STRUCT TYPE | ..CONTENT - pointer to data that'll be copied over
 * Creates a fat pointer from the given header type AND content in the stack.
 * IMPORTANT NOTE(EimaMei): You're responsible for filling out the header, not sili.
*/
#define si_fatPtrMake(headerType, .../*CONTENT*/)  \
	memcpy( \
		si_fatPtrMakeReserve(headerType, typeof(__VA_ARGS__), 1), \
		&(typeof(__VA_ARGS__)){__VA_ARGS__}, \
		sizeof(typeof(__VA_ARGS__)) \
	)

/* headerType - STRUCT HEADER | type - TYPE | count - usize
 * Reserves an array of fat pointers in the stack. */
#define si_fatPtrMakeReserve(headerType, type, count)  \
	((siByte*)si_salloc((sizeof(type) * count) + sizeof(headerType)) + sizeof(headerType))


/*
	========================
	| siFunction           |
	========================
*/
/* Contains a function pointer for siThread. */
typedef struct {
   rawptr SI_FUNC_PTR(ptr, (rawptr));
} siFunction;

/* func - NAME
 * A hasty macro solution for inputting a function into a thread. */
#define siFunc(func) (siFunction){(rawptr (*)(rawptr))func}

/*
	========================
	| Other                |
	========================
*/

/* A struct denoting the version. */
typedef struct { i32 major, minor; } siVersion;

/* An XY point structure. Both are 32-bit integers. */
typedef struct { i32 x, y; } siPoint;
/* x - i32 | y - i32
 * Macro to define an XY i32 point. */
#define SI_POINT(x, y) ((siPoint){x, y})

/* An XY point structure. Both are isize integers. */
typedef struct { isize x, y; } siPointS;
/* x - isize | y - isize
 * Macro to define an XY isize point. */
#define SI_POINT_S(x, y) ((siPointS){x, y})

/* An RGBA structure. */
typedef struct { u8 r, g, b, a; } siColor;
/* r, g, b, a - u8
 * Macro to define an RGBA color. */
#define SI_RGBA(r, g, b, a) ((siColor){r, g, b, a})
/* r, g, b - u8
 * Macro to define an RGB color. */
#define SI_RGB(r, g, b) SI_RGBA(r, g, b, 255)

/* Width and height i32 structure. */
typedef struct { i32 width, height; } siArea;
/* width - i32 | height - i32
 * Macro to define a signed area. */
#define SI_AREA(width, height) ((siArea){width, height})

/* Poistion and area structure. */
typedef struct { i32 x, y, width, height; } siRect;
/* x - i32 | y - i32 | width - i32 | height - i32
 * Macro to define a rectangle from 4 32-bit integers. */
#define SI_RECT(x, y, width, height) ((siRect){(f32)x, (f32)y, (f32)width, (f32)height})
/* x - i32 | y - i32 | area - siArea
 * Macro to define a rectangle from XY and area. */
#define SI_RECT_A(x, y, area) SI_RECT(x, y, (area).width, (area).height)
/* point - siPoint | width - i32 | height - i32
 * Macro to define a rectangle from point and width & height. */
#define SI_RECT_P(point, width, height) SI_RECT((point).x, (point).y, width, height)
/* point - siPoint | area - siArea
 * Macro to define a rectangle from position and area. */
#define SI_RECT_PA(point, area) SI_RECT((point).x, (point).y, (area).width, (area).height)

/* 2D vector structure. */
typedef struct { f32 x, y; } siVec2;
/* x - f32 | y - f32
 * Macro to define a 2D vector from two f32. */
#define SI_VEC2(x, y) ((siVec2){x, y})
/* 3D vector structure. */
typedef struct { f32 x, y, z; } siVec3;
/* x - f32 | y - f32 | z - f32
 * Macro to define a 3D vector from three f32. */
#define SI_VEC3(x, y, z) ((siVec3){x, y, z})
/* 4D vector structure. */
typedef struct { f32 x, y, z, w; } siVec4;
/* x - f32 | y - f32 | z - f32 | w - f32
 * Macro to define a 4D vector from three f32. */
#define SI_VEC4(x, y, z, w) ((siVec4){x, y, z, w})
/* a - siVec2 | b - siVec2
 * Macro to define a 4D vector from two vec2s. */
#define SI_VEC4_VEC2(a, b) SI_VEC4((a).x, (a).y, (b).x, (b).y)

typedef SI_ENUM(u32, siDirection) {
	SI_DIRECTION_LEFT,
	SI_DIRECTION_RIGHT
};

/* Moves memory by 'moveBy' amount of bytes depending on the direction. */
void si_ptrMoveBy(rawptr src, usize srcLen, isize moveBy, siDirection direction);

/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| SI_DEFINE            |
	========================
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*/

#if !defined(SI_NO_ALLOCATOR)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siAllocator          |
	========================
*/

/* A structure containing the starting pointer of the data, maximum length and
 * the current index of the data. */
typedef struct {
	siByte* ptr;
	usize maxLen;
	usize offset;
} siAllocator;

/* Sets memory alignment. By default it's 4 bytes. */
#define SI_DEFAULT_MEMORY_ALIGNMENT (4)
/* Aligns 'n' by SI_DEFAULT_MEMORY_ALIGNMENT. */
usize si_alignCeil(usize n);
/* Aligns 'n' by 'alignment'. */
usize si_alignCeilEx(usize n, u32 alignment);

/* Creates an allocator from the heap with N amounts of bytes. */
siAllocator* si_allocatorMake(usize bytes);
/* bytes - usize
 * Creates an allocator from the stack with N amounts of bytes. */
#define si_allocatorMakeStack(bytes) \
	si_allocatorMakeStackEx(bytes, si_salloc(bytes + sizeof(siAllocator)))
siAllocator* si_allocatorMakeStackEx(usize bytes, rawptr res);
/*  Creates a temporary allocator. */
siAllocator si_allocatorMakeTmp(rawptr pointer, usize maxLen);

/* Returns the amount of bytes that are still available from the allocator. */
usize si_allocatorAvailable(siAllocator* alloc);
/* Returns the current pointer offset of the allocator. */
rawptr si_allocatorCurPtr(siAllocator* alloc);

/* Resizes the amount of available bytes from the allocator. */
void si_allocatorResize(siAllocator* alloc, usize newSize);
/* Resets the allocator to start from the beginning. */
void si_allocatorReset(siAllocator* alloc);
/* Resets the allocator to the given offset. */
void si_allocatorResetFrom(siAllocator* alloc, usize offset);
/* Pushes a byte into the allocator. */
void si_allocatorPush(siAllocator* alloc, siByte byte);
/* Frees the allocator from memory. All allocations from the allocator are also
 * invalidated with this. */
void si_allocatorFree(siAllocator* alloc);

#if defined(SI_NO_ALLOC_DEBUG_INFO)
/* Allocates 'bytes' amount of memory from the allocator. */
rawptr si_malloc(siAllocator* alloc, usize bytes);
/* Allocates 'bytes' multipled by 'num' amount of memory from the allocator. */
rawptr si_calloc(siAllocator* alloc, usize num, usize bytes);
/* Equivalent to doing 'si_malloc' and then 'memcpy'. */
rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize);
/* Syntactic sugar, but also to keep compatibility with non-allocator mode. */
void si_free(siAllocator* alloc, rawptr ptr);

#else
/* alloc - siAllocator* | bytes - usize
 * Allocates 'bytes' amount of memory from the allocator. */
#define si_malloc(alloc, bytes) si_mallocEx(alloc, bytes, __FILE__, __LINE__)
/* alloc - siAllocator* | num - usize | bytes - usize
 * Allocates 'bytes' multipled by 'num' amount of memory from the allocator. */
#define si_calloc(alloc, num, bytes) si_callocEx(alloc, num, bytes, __FILE__, __LINE__)
/* alloc - siAllocator* | ptr - rawptr | oldSize - usize | newSize - usize
 * Equivalent to doing 'si_malloc' and then 'memcpy'. */
#define si_realloc(alloc, ptr, oldSize, newSize) si_reallocEx(alloc, ptr, oldSize, newSize, __FILE__, __LINE__)
/* Syntactic sugar, but also to keep compatibility with non-allocator mode. */
void si_free(siAllocator* alloc, rawptr ptr);

#if 1
	rawptr si_mallocEx(siAllocator* alloc, usize bytes, cstring filename, i32 line);
	rawptr si_callocEx(siAllocator* alloc, usize num, usize bytes, cstring filename, i32 line);
	rawptr si_reallocEx(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize,
			cstring filename, i32 line);
#endif
#endif /* SI_NO_ALLOC_DEBUG_INFO */


#endif /* SI_NO_ALLOCATOR */

#if !defined(SI_NO_PAIR)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siPair               |
	========================
*/
/* type1 - TYPE | type2 - TYPE
 * A pair structure. */
#define siPair(type1, type2) struct { type1 first; type2 second; }
/* firstValue - EXPRESSION | secondValue - EXPRESSION
 * Creates a pair structure variable. */
#define si_pairMake(firstValue, secondValue) {firstValue, secondValue}
/* pair - siPair
 * Makes a copy of the given pair. */
#define si_pairCopy(pair) si_pairMake(pair.first, pair.second)

#endif /* SI_NO_PAIR */

#if !defined(SI_NO_OPTIONAL)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siOptional           |
	========================
*/

/* type - TYPE
 * Specifies the return of a 'siOptional(typpe)'. */
#define siOptionalRet(type) rawptr
/* type - TYPE
 * Denotes a 'siOptional' type. */
#define siOptional(type) struct { b32 hasValue; type value; }*
/* A fully empty 'siOptional' variable. */
#define SI_OPTIONAL_NULL(alloc) si_mallocCopy(alloc, &(struct { b32 hasValue; }){false})

/* allocator - siAllocator* | ...VALUE - EXPRESSION
 * Creates a 'siOptional' from the given value, writes it into the allocator and
 * returns a pointer to the written data.. */
#define si_optionalMake(allocator, .../* VALUE */) \
	si_optionalMakeEx( \
		si_mallocItem(allocator, struct { b32 hasValue; typeof(__VA_ARGS__) value; }), \
		si_anyMake(__VA_ARGS__), \
		si_offsetof(struct { b32 hasValue; typeof(__VA_ARGS__) value; }, value) \
	)
/* optionalVar -  siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. HOWVER if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(optionalVar, defaultValue) \
	((optionalVar)->hasValue ? (optionalVar)->value : (typeof((optionalVar)->value))(defaultValue))

/* optionalVar - siOptional(TYPE)
 * Resets the optional variable. */
#define si_optionalReset(optionalVar) \
	do { \
		memset(&(optionalVar)->value, 0, sizeof(typeof((optionalVar)->value))); \
		(optionalVar)->hasValue = false; \
	} while(0)

#endif /* SI_NO_OPTIONAL */

#if !defined(SI_NO_ARRAY)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siArray             |
	========================
*/

/* The header for 'siArray'. */
typedef struct {
	/* The allocator used for creating the array. Used for reallocations, frees, etc. */
	siAllocator* allocator;
	/* The current amount of items. */
	usize len;
	/* The maximum possible count of maximum items. */
	usize capacity;
	/* Size for each item. */
	u32 typeSize;
} siArrayHeader;

/* type - TYPE
 * Denotes a 'siArray' variable. */
#define siArray(type) type*
/* array - siArray(TYPE)
 * Gets the siArray's header. */
#define SI_ARRAY_HEADER(array) ((siArrayHeader*)array - 1)

#if !defined(for_each)
	#if defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION > SI_STANDARD_C89
		/* variableName - NAME | array - siArray(TYPE)
		 * Iterates through a siArray. */
		#define for_each(variableName, array) \
			for (\
				typeof(array) variableName = array; \
				variableName != (typeof(array))si_arrayGetPtr(array, si_arrayLen(array)); \
				variableName += 1 \
			)
	#else
		/* variableName - NAME | array - siArray(TYPE)
		 * Iterates through a siArray. */
		#define for_each(variableName, array) \
			typeof(array) variableName; \
			for ( \
				variableName = array; \
				variableName != (typeof(array))si_arrayGetPtr(array, si_arrayLen(array)); \
				variableName += 1 \
			)
	#endif
#endif

/* array - siArray(TYPE) | index - usize
 * Gets a specific item's pointer and return it as a 'siByte*' for pointer arithmetic. */
#define si_arrayGetPtr(array, index) \
	(si_cast(siByte*, array) + si_arrayTypeSize(array) * (index))

/* allocator - siAllocator* | ...BUFFER - STATIC BUFFER
 * Creates a siArray from the provided static buffer. */
#define si_arrayMake(allocator, ...) \
	si_arrayMakeList(allocator, __VA_ARGS__, sizeof(typeof(*(__VA_ARGS__))), countof((__VA_ARGS__)))
/* Creates a siArray from the provided list. */
rawptr si_arrayMakeList(siAllocator* alloc, rawptr list, usize sizeofItem, usize count);
/* Reserves a 'count' amount of items in the array. */
rawptr si_arrayMakeReserve(siAllocator* alloc, usize sizeofItem, usize count);
/* Creates a siArray from another siArray. */
rawptr si_arrayCopy(siAllocator* alloc, siArray(void) array);

/* Gets the current length of the siArray. */
usize si_arrayLen(rawptr array);
/* Gets the item capacity of the siArray. */
usize si_arrayCapacity(rawptr array);
/* Gets the siArray's type size. */
usize si_arrayTypeSize(rawptr array);
/* Gets the array's allocator. */
siAllocator* si_arrayAllocator(rawptr array);
/* Returns the amount of bytes that the array takes up in memory whilst not
 * counting the header. Equivalent to arrayCapacity * arrayTypeSize. */
usize si_arrayTotalSize(rawptr array);
/* Returns true if 'array' is nil or the length is zero. */
b32 si_arrayEmpty(rawptr array);

/* Returns a pointer of the item. If the index is out of the bounds, the return is nil. */
rawptr si_arrayAt(rawptr array, usize index);
/* Returns a pointer of the front item. */
rawptr si_arrayFront(rawptr array);
/* Returns a pointer of the back item. */
rawptr si_arrayBack(rawptr array);

/* arrayPtr - siArray(TYPE)* | ...VALUE - EXPRESSION
 * Appends the given value to the array. If the array's capacity is reached, the
 * array is reallocated by (currentCapacity * 2). */
#define si_arrayAppend(arrayPtr, .../* VALUE */) si_intern_arrayAppend(arrayPtr, si_anyMake(__VA_ARGS__))
/* arrayPtr - siArray(TYPE)* | ...VALUE - EXPRESSION
 * Wrapper macro for si_arrayAppend. */
#define si_arrayPush(arrayPtr, .../* VALUE */) si_arrayAppend(arrayPtr, __VA_ARGS__)
/* Erases the last item in the array. */
void si_arrayPop(rawptr arrayPtr);
/* arrayPtr - siArray(TYPE)* | newItem - EXPRESSION | index - usize
 * Inserts 'newItem' at the provided index. */
#define si_arrayInsert(arrayPtr, newItem, index) si_intern_arrayInsert(arrayPtr, si_anyMake(newItem), index)
/* Erases a certain index. */
void si_arrayErase(rawptr arrayPtr, usize index);
/* Erases a certain index PLUS the amount specified afterwards get erased too. */
void si_arrayEraseCount(rawptr arrayPtr, usize index, usize count);
/* arrayPtr - siArray(TYPE)* | ...VALUE - EXPRESSION
 * Removes every item in the array that equals to the value. */
#define si_arrayRemoveItem(arrayPtr, .../* VALUE */) si_intern_arrayRemoveItem(arrayPtr, si_anyMake(__VA_ARGS__))
/* arrayPtr - siArray(TYPE)* | index - usize | count - usize | ...VALUE - EXPRESSION
 * 'array[index]...array[index + count]' get filled with 'VALUE'. */
#define si_arrayFill(arrayPtr, index, count, .../* VALUE */ ) si_intern_arrayFill(arrayPtr, index, count, si_anyMake(__VA_ARGS__))

/* array - siArray(TYPE) | ...VALUE - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from index 0. Returns
 * -1 if nothing was found. */
#define si_arrayFind(array, ...) si_intern_arrayFind(array, 0, si_arrayLen(array), si_anyMake(__VA_ARGS__))
/* array - siArray(TYPE) | start - usize | end - usize | ...VALUE - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from 'start' to 'end'.
 * Returns -1 if nothing was found. */
#define si_arrayFindEx(array, start, end, ...) si_intern_arrayFind(array, start, end, si_anyMake(__VA_ARGS__))
/* array - siArray(TYPE) | ...VALUE - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from the last index.
 * Returns -1 if nothing was found. */
#define si_arrayRFind(array, ...) si_intern_arrayRFind(array, si_arrayLen(array) - 1, 0, si_anyMake(__VA_ARGS__))
/* array - siArray(TYPE) | start - usize | end - usize | ...VALUE - EXPRESSION
 * Attempts to find the last mention of 'VALUE', starting from 'start' to 'end'.
 * Returns -1 if nothing was found. */
#define si_arrayRFindEx(array, start, end, ...) si_intern_arrayRFind(array, start, end, si_anyMake(__VA_ARGS__))
/* array - siArray(TYPE) | oldValue - EXPRESSION | ...newValue - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from the last index.
 * Returns -1 if nothing was found. */
#define si_arrayReplace(arrayPtr, oldValue, .../* newValue */) si_intern_arrayReplace(array, si_anyMake(oldValue), si_anyMake(__VA_ARGS__))
/* Reverses the contents of the array. */
void si_arrayReverse(rawptr array);
/* Creates a 'siString' from a 'siArray'. */
char* si_arrayToSistring(siArray(char*) array, cstring separator);

/* Sums the contents of the array as an unsigned number. */
u64 si_arraySumU64(siArray(void) array);
/* Sums the contents of the array as a signed number. */
i64 si_arraySumI64(siArray(void) array);
/* Sums the contents of the array as a float number. */
f64 si_arraySumF64(siArray(void) array);

/* Clears the contents of the array to zero. */
void si_arrayClear(rawptr array);
/* Checks if two arrays equal. */
b32 si_arrayEqual(rawptr lha, rawptr rha);

/* Frees the array from memory. Should not really be used if an allocator system is used.*/
void si_arrayFree(rawptr array);
/* Shrinks the array to its bare minimums in memory. Should REALLY not be used
 * if an allocator system is used.*/
void si_arrayShrinkToFit(rawptr arrayPtr);

#if 1 /* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice really. */
isize si_intern_arrayFind(rawptr array, usize start, usize end, siAny value);
isize si_intern_arrayRFind(rawptr array, usize start, usize end, siAny value);
rawptr si_intern_arrayAppend(rawptr arrayPtr, siAny value);
void si_intern_arrayInsert(rawptr arrayPtr, siAny value, usize index);
void si_intern_arrayReplace(rawptr arrayPtr, siAny oldValue, siAny newValue);
void si_intern_arrayRemoveItem(rawptr arrayPtr, siAny item);
void si_intern_arrayFill(rawptr arrayPtr, usize index, usize count, siAny item);
#endif

#endif /* SI_NO_ARRAY */

#if !defined(SI_NO_STRING)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siString             |
	========================
*/

/* siString is just a regular UTF-8 char pointer. */
typedef char* siString;

/* The string header. */
typedef struct {
	/* Allocator used by the string. */
	siAllocator* allocator;
	/* Current length of the string. */
	usize len;
	/* Total memory capacity of the string. */
	usize capacity;
} siStringHeader;


/* Gets the header of the string. */
#define SI_STRING_HEADER(str) ((siStringHeader*)str - 1)

/* Creates a siString from a null-terminated C-string. */
siString si_stringMake(siAllocator* alloc, cstring str);
/* Creates a siString from a C-string with a specified length. */
siString si_stringMakeLen(siAllocator* alloc, cstring str, usize len);
/* Creates a siString from a C-string with a specified length and capacity. */
siString si_stringMakeEx(siAllocator* alloc, cstring str, usize len, usize capacity);
/* Creates a siString with a reserved capacity. */
siString si_stringMakeReverse(siAllocator* alloc, usize capacity);
/* Create a siString from a formatted null-terminated C-string. */
siString si_stringMakeFmt(siAllocator* alloc, cstring str, ...);

/*  Creates a copy of an existing siString. */
siString si_stringCopy(siAllocator* alloc, siString from);

/* Gets the length of the siString. */
usize si_stringLen(siString str);
/* Gets the capacity of a siString. */
usize si_stringCapacity(siString str);
/* Gets the type size of a siString. */
usize si_stringTypeSize(siString str);
/* Gets the allocator of the specific siString. */
siAllocator* si_stringAllocator(siString str);
/* Returns true if the string is nil or length is 0. */
b32 si_stringEmpty(siString str);

/* Gets the character at a specified index in the siString. If the index is out of
 * bounds, the return is '/0'. */
char si_stringAt(siString str, usize index);
/* Gets the first character of the siString. */
char si_stringFront(siString str);
/* Gets the last character of the siString. */
char si_stringBack(siString str);
/* Gets the substring of the siString. and writes it into the allocator. */
char* si_stringSub(siAllocator* alloc, siString str, usize begin, usize len);

/* Finds the first occurrence of a substring in the siString. Returns -1 if nothing
 * was found. */
isize si_stringFind(siString str, cstring cstr);
/* Finds the first occurrence of a substring within a specific range of a siString.
 * Returns -1 if nothing was found. */
isize si_stringFindEx(siString str, usize start, usize end, cstring cstr, usize cstrLen);
/* Finds the first occurrence of a substring in siString. If 'stopAt' is met in
 * the string, the search ends. Returns -1 if nothing was found. */
isize si_stringFindStopAt(siString str, cstring cstr, char stopAt);
/* Finds the first occurrence of a substring within a specific range of a siString.
 * If 'stopAt' is met in the string, the search ends. Returns -1 if nothing was found. */
isize si_stringFindStopAtEx(siString str, usize start, usize end, cstring cstr,
	usize cstrLen, char stopAt);
/* Finds the last occurrence of a substring in the siString. Returns -1 if nothing
 * was found. */
isize si_stringRFind(siString str, cstring cstr);
/* Finds the last occurrence of a substring within a specific range of the siString.
 * Returns -1 if nothing was found. */
isize si_stringRFindEx(siString str, usize start, usize end, cstring cstr, usize cstrLen);
/* Finds the last occurrence of a substring in siString. If 'stopAt' is met in
 * the string, the search ends. Returns -1 if nothing was found. */
isize si_stringRFindStopAt(siString str, cstring cstr, char stopAt);
/* Finds the last occurrence of a substring within a specific range of a siString.
 * If 'stopAt' is met in the string, the search ends. Returns -1 if nothing was found. */
isize si_stringRFindStopAtEx(siString str, usize start, usize end, cstring cstr,
	usize cstrLen, char stopAt);

/* Joins a siString with a null-terminated C-string using a separator. */
void si_stringJoin(siString* str, cstring cstr, cstring separator);
/* Sets the siString to a null-terminated C-string. */
void si_stringSet(siString* str, cstring cstr);
/* Replaces all occurences of first null-terminated C-string, `oldStr`,
 * in the siString to the second null-terminated C-string, `newStr`. */
void si_stringReplace(siString* str, cstring oldStr, cstring newStr);
/* Replaces all occurences of first C-string with specified length, `oldStr`,
 * in the siString to the second C-string with specified length, `newStr`. */
void si_stringReplaceEx(siString* str, cstring oldValue, usize oldLen, cstring newValue,
		usize newLen);
/* Cuts all occurences of the null-terminated C-string in the siString. */
void si_stringTrim(siString str, cstring cutSet);
/* Cuts all occurences of the C-string with specified length in the siString. */
void si_stringTrimLen(siString str, cstring cutSet, usize cutLen);
/* Enquotes siString (String 'test' becomes "test"). */
void si_stringEnquote(siString* str);

/* Appends a null-terminated C-string to the siString. */
void si_stringAppend(siString* str, cstring other);
/* Appends a C-string with specified length to the siString. */
void si_stringAppendLen(siString* str, cstring other, usize otherLen);
/* Pushes a character into the siString. */
void si_stringPush(siString* str, char other);
/* Pops the last character of the siString.  **/
void si_stringPop(siString str);

/* Inserts a null-terimnated C-string to the siString at the specified index. */
void si_stringInsert(siString* str, cstring cstr, usize index);
/* Inserts a C-string with specified length to the siString at the specified index. */
void si_stringInsertLen(siString* str, cstring cstr, usize cstrLen, usize index);
/* Inserts a C-string with specified length to the siString at the specified index,
 * as erases the index itself if 'eraseIndex' is set to true. */
void si_stringInsertEx(siString* str, cstring cstr, usize cstrLen, usize index, b32 eraseIndex);
/* Erases a substring from the specified index. */
void si_stringErase(siString str, usize index, usize eraseLen);
/* Removes every occurrence of the provided null-terminated C-string. */
void si_stringRemoveCstr(siString str, cstring cstr);

/* Removes any leading or trailing spaces and newlines in the string. */
void si_stringStrip(siString str);
/* Reverses the entirety of the siString. */
void si_stringReverse(siString str);
/* Reverses a front section of the string. */
void si_stringReverseLen(siString str, usize len);

/* Returns a siArray of substrings that were split based on the delimiter. */
siArray(siString) si_stringSplit(siAllocator* alloc, siString str, cstring delimiter);
/* Clears the string by setting the first letter and length to 0. */
void si_stringClear(siString str);

/* Frees the string from memory. Does nothing when the allocator system is used. */
void si_stringFree(siString str);
/* Shrinks the string to the point where the capacity is equal to the length of
 * the string. Should _not_ be used with an allocator system, it'll consume
 * even more memory than reduce it. */
void si_stringShrinkToFit(siString* str);
/* Internal function. Allocates more space to the string. */
void si_stringMakeSpaceFor(siString* str, usize addLen);
#endif /* SI_NO_STRING */

#if !defined(SI_NO_UNICODE)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	============================
	| UTF-8, UTF-16 and UTF-32 |
	============================
*/


/* Denotes that the string is UTF-8 encoded. */
typedef cstring siUtf8String;
/* Denotes that the string is UTF-16 encoded. */
typedef u16* siUtf16String;


/* Denotes that the UTF-8 character is invalid. */
#define SI_UTF8_INVALID (-1)


typedef struct {
	/* The codepoint (UTF-8 encoded). */
	char codepoint[4];
	/* Length of 'codepoint' in bytes. */
	u32 len;
} siUtf8Char;

typedef struct {
	/* The codepoint (UTF-32 encoded). */
	i32 codepoint;
	/* Length of the character in UTF-8. */
	u32 len;
} siUtf32Char;


/* Decodes the given UTF-8 character into UTF-32 and returns a 'siUtf32Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf32Char){SI_UTF8_INVALID, 0}. */
siUtf32Char si_utf8Decode(const char character[4]);
/* Encodes the specified UTF-32 character into UTF-8 and returns a 'siUtf8Char'
 * structure. This function does not check if the UTF-32 is invalid. */
siUtf8Char si_utf8Encode(i32 codepoint);

/* Returns the amount of characters in an UTF-8 string. For example, an input
 * string of "リトアニア" returns 15 from 'si_cstrLen', while 'si_utf8StrLen'
 * returns 5. */
usize si_utf8StrLen(siUtf8String str);
/* Returns a UTF-32 codepoint and length from the specified index. For example,
 * 'si_utf8StrAt("リトアニア", 2)' will return (siUtf32Char){0x30A2, 3}. */
siUtf32Char si_utf8StrAt(siUtf8String str, usize charIndex);

/* Encodes a NULL-terminated UTF-8 string into UTF-16. */
siUtf16String si_utf8ToUtf16Str(siAllocator* alloc, siUtf8String str, usize* strLenOut);
/* Encodes a NULL-terminated UTF-16 string into UTF-8. */
siUtf8String si_utf16ToUtf8Str(siAllocator* alloc, siUtf16String str, usize* strLenOut);


#endif /* SI_NO_UNICODE */

#if !defined(SI_NO_CHAR)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siChar               |
	========================
*/

/* Returns the lowercased version of the given character. */
char si_charLower(char c);
/* Returns the uppercased version of the given character. */
char si_charUpper(char c);

/* Returns true  if the given character is a space. */
b32 si_charIsSpace(char c);
/* Returns true if the given character is from '0' to '9'. */
b32 si_charIsDigit(char c);
/* Returns true if the given character is a hex digit (between '0'..'9'; 'a'...'f'; 'A'...'F'). */
b32 si_charIsHexDigit(char c);
/* Returns true if the given character is in the English alphabet. */
b32 si_charIsAlpha(char c);
/* Returns true if the given character is in the English alphabet OR is a number. */
b32 si_charIsAlphanumeric(char c);
/* Converts '0'...'9' to an actual integer ('3' -> 3). */
i32 si_charDigitToInt(char c);
/* Converts a hex digit into an actual integer ('F' -> 15). */
i32 si_charHexDigitToInt(char c);
#endif /* SI_NO_CHAR */

#if !defined(SI_NO_STRING)
/*
	========================
	|  cstring             |
	========================
*/

#define SI_NUM_MAX_DIGITS 20

/* Writes a NULL-terminated C-string into the allocator. */
char* si_cstrMake(siAllocator* alloc, cstring cstr);
/* Writes a C-string with specified length into the allocator. */
char* si_cstrMakeLen(siAllocator* alloc, cstring cstr, usize len);
/* Writes a formatted NULL-terminated C-string into the allocator. */
char* si_cstrMakeFmt(siAllocator* alloc, cstring cstr, ...);

/* Returns the length of a NULL-terminated C-string. */
usize si_cstrLen(cstring str);

/* Sets every character in C-string to uppercased. */
void si_cstrUpper(char* str);
/* Sets every character in C-string to lowercased. */
void si_cstrLower(char* str);
/* Sets every word's first letter to uppercase, while the rest lowercase. */
void si_cstrTitle(char* str);
/* Uppercases the first NULL-terminated C-string's character, then the rest are lowercased. */
void si_cstrCapitalize(char* str);

/* Returns true if both NULL-terminated C-strings are equal in memory. */
b32 si_cstrEqual(cstring str1, cstring str2);
/* Returns true if both C-strings with specified lengths are equal in memory. */
b32 si_cstrEqualLen(cstring str1, usize str1Len, cstring str2, usize str2Len);

/* Returns an unsigned 64-bit integer form of the NULL-terminated C-string.
 * The string must be a number. */
u64 si_cstrToU64(cstring str);
/* Returns an unsigned 64-bit integer form of the C-string with specified length.
 * The string must be a number. */
u64 si_cstrToU64Len(cstring str, usize len);
/* Returns a signed 64-bit integer form of the NULL-terminated C-string.
 * The string must be a number. */
i64 si_cstrToI64(cstring str);
/* Returns an signed 64-bit integer form of the C-string with specified length.
 * The string must be a number. */
u64 si_cstrToI64Len(cstring str, usize len);
/* TODO(EimaMei): f64 si_cstr_to_f64(cstring str); */

/* Creates a string from the given unsigned number. By default the function assumes
 * the number is base-10. */
char* si_u64ToCstr(siAllocator* alloc, u64 num);
/* Creates a string from the specified unsigned number and its base. Length of
 * the string is written into 'outLen', unless 'outLen' is nil. */
char* si_u64ToCstrEx(siAllocator* alloc, u64 num, i32 base, usize* outLen);
/* Creates a string from the specified signed number. By default the function assumes
 * the number is base-10. */
char* si_i64ToCstr(siAllocator* alloc, i64 num);
/* Creates a string from the specified signed number and its base. Length of
 * the string is written into 'outLen', unless 'outLen' is nil. */
char* si_i64ToCstrEx(siAllocator* alloc, i64 num, i32 base, usize* outLen);

/* Creates a string from the specified float. By default the function assumes that
 * the float is base-10, and that the afterPoint count is set to 6 decimals. */
char* si_f64ToCstr(siAllocator* alloc, f64 num);
/* Creates a string from the specified float, its base and afterPoint decimal count.
 * Length of the string is written into 'outLen', unless 'outLen' is nil. */
char* si_f64ToCstrEx(siAllocator* alloc, f64 num, i32 base, u32 afterPoint,
		usize* outLen);

#endif /* SI_NO_STRING */

#if !defined(SI_NO_HASHTABLE)

/* type - TYPE
 * Denotes that this is a 'siHashTable' variable. */
#define siHt(type) siHashTable*

typedef struct {
	/* Key name for the value. */
	cstring key;
	/* Pointer to the value. */
	rawptr value;
} siHashEntry;

typedef struct {
	/* Allocator used to store the hash entries. */
	siAllocator* alloc;
	/* Array of the hash entries. */
	siArray(siHashEntry) entries;
} siHashTable;

/* Creates a hash table using the given names and data. */
siHashTable si_hashtableMake(siAllocator* alloc, cstring* keyArray, rawptr dataArray,
		usize sizeofElement, usize len);
/* Reserves a 'capacity' amount of items for the hash table. */
siHashTable si_hashtableMakeReserve(siAllocator* alloc, usize capacity);
/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
rawptr si_hashtableGet(siHashTable ht, cstring key);
/* ht - siHashTable* | key - cstring | type - TYPE
 * Returns the key entry's value as the specified type. If the item does NOT exist,
 * this will crash. */
#define si_hashtableGetItem(ht, key, type) (*(type*)si_hashtableGet(ht, key))
/* ht - siHashTable* | key - cstring | value - EXPRESSION
 * Adds a 'key' entry to the hash table. */
#define si_hashtableSet(ht, key, value) si_hashtableSetEx(ht, key, si_anyMake(value))
/* Adds a 'key' entry to the hash table, with the value being siAny. */
siHashEntry* si_hashtableSetEx(siHashTable* ht, cstring key, siAny value);

/* Frees the hash table. */
void si_hashtableFree(siHashTable ht);
#endif /* SI_NO_HASHTABLE */

#if !defined(SI_NO_IO)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	|  Input and Output    |
	========================
*/

typedef SI_ENUM(i32, siFileMode) {
	SI_FILE_READ = SI_BIT(0),
	SI_FILE_WRITE = SI_BIT(1),
	SI_FILE_APPEND = SI_BIT(2),
	SI_FILE_PLUS = SI_BIT(3),

	SI_FILE_ALL = SI_FILE_READ | SI_FILE_WRITE | SI_FILE_APPEND | SI_FILE_PLUS
};

typedef struct {
	/* OS-specific handle of the file. */
	#if defined(SI_SYSTEM_WINDOWS)
		rawptr handle;
	#else
		i64 handle;
	#endif
	/* Size of the read file. */
	usize size;
	/* Filename of the file. */
	cstring filename;
	/* Last time it was written at. */
	u64 lastWriteTime;
} siFile;

typedef SI_ENUM(u32, siFilePermissions) {
	SI_FS_PERM_NONE = 0, /* No permission bits are set. */

	SI_FS_PERM_OWNER_READ = 0400, /* File owner has read permission. */
	SI_FS_PERM_OWNER_WRITE = 0200, /* File owner has write permission. */
	SI_FS_PERM_OWNER_EXEC = 0100, /* File owner has execute/search permission. */
	SI_FS_PERM_OWNER_ALL = 0700, /* File owner has read, write, and execute/search permissions, equivalent to
									 'SI_FS_PERM_OWNER_READ | SI_FS_PERM_OWNER_WRITE | SI_FS_PERM_OWNER_EXEC'. */
	SI_FS_PERM_GROUP_READ = 040, /* The file's user group has read permission. */
	SI_FS_PERM_GROUP_WRITE = 020, /* The file's user group has write permission. */
	SI_FS_PERM_GROUP_EXEC = 010, /* The file's user group has execute/search permission. */
	SI_FS_PERM_GROUP_ALL = 070, /* The file's user group has read, write, and execute/search permissions, equivalent to
									 'SI_FS_PERM_GROUP_READ | SI_FS_PERM_GROUP_WRITE | SI_FS_PERM_GROUP_EXEC'. */
	SI_FS_PERM_OTHERS_READ = 04, /* Other users have read permission. */
	SI_FS_PERM_OTHERS_WRITE = 02, /* Other users have write permission. */
	SI_FS_PERM_OTHERS_EXEC = 01, /* Other users have execute/search  permission. */
	SI_FS_PERM_OTHERS_ALL = 07, /* Other users have read, write, and execute/search permissions, equivalent to
									 'SI_FS_PERM_OTHERS_READ | SI_FS_PERM_OTHERS_WRITE | SI_FS_PERM_OTHERS_EXEC'. */
	SI_FS_PERM_ALL = 0777, /* All users have read, write, and execute/search permissions. Equivalent to
								'SI_FS_PERM_OWNER_ALL | SI_FS_PERM_GROUP_ALL | SI_FS_PERM_OTHERS_ALL'. */
	/* UNIX ONLY */
	SI_FS_PERM_SET_UID = 04000, /* Set user ID to file owner user ID on execution. */
	SI_FS_PERM_SET_GID = 02000, /* Set group ID to file's user group ID on execution. */
	SI_FS_PERM_STICKY_BIT = 01000, /* Implementation-defined meaning, but POSIX XSI specifies that when set on a directory,
								   only file owners may delete files even if the directory is writeable to others (used with /tmp). */
	SI_FS_PERM_MASK = 07777 /* 	All valid permission bits, equivalent to 'SI_FS_PERM_ALL | SI_FS_PERM_SET_UID |
									SI_FS_PERM_SET_GID | SI_FS_PERM_STICKY_BIT'. */
};

typedef SI_ENUM(i32, siFileMoveMethod) {
	SI_FILE_MOVE_BEGIN = 0, /* Sets the pointer from the beginning of the file. */
	SI_FILE_MOVE_CURRENT = 1, /* Sets the pointer from the current offset. */
	SI_FILE_MOVE_END = 2 /* Sets the pointer from the EOF offset. */
};

typedef SI_ENUM(i32, siStdFileType) {
	SI_STDTYPE_INPUT,
	SI_STDTYPE_OUTPUT,
	SI_STDTYPE_ERROR,
	SI_STDTYPE_COUNT
};

#define SI_STDIN  (si_fileGetStdFile(SI_STDTYPE_INPUT))
#define SI_STDOUT (si_fileGetStdFile(SI_STDTYPE_OUTPUT))
#define SI_STDERR (si_fileGetStdFile(SI_STDTYPE_ERROR))

typedef SI_ENUM(i32, siFileSystemError) {
	SI_FS_ERROR_NONE = 0, /* No error has been encountered. */
	SI_FS_ERROR_INVALID, /* Provided value is invalid. */
	SI_FS_ERROR_INVALID_FILENAME, /* Invalid filename. */
	SI_FS_ERROR_EXISTS, /* File/directory already exists. */
	SI_FS_ERROR_DOESNT_EXIST, /* File/directory doesn't exist. */
	SI_FS_ERROR_PERMISSION, /* User doesn't have permission to read/write. */
	SI_FS_ERROR_TRUNCATE_FAILURE /* Failed to truncate the file. */
};

#if defined(SI_SYSTEM_WINDOWS)
	static const char SI_PATH_SEPARATOR = '\\';
	static const char SI_PATH_SEPARATOR_REVERSE = '/';
#else
	static const char SI_PATH_SEPARATOR = '/';
	static const char SI_PATH_SEPARATOR_REVERSE = '\\';
#endif

/* Last file system error. On non-release mode, the user gets notified when an
 * error happens. */
extern siErrorInfo SI_FS_ERROR;
/* Returns the string representation of the error code. */
cstring si_pathFsErrorName(siFileSystemError err);
/* Returns the description of the error. */
cstring si_pathFsErrorStr(siFileSystemError err);

/* The amount of bytes that were read. */
extern i32 SI_FS_READ_BYTES;

/*
	========================
	|  siPath              |
	========================
*/

/* Returns a boolean indicating if the given NULL-terminated C-string path exists. */
b32 si_pathExists(cstring path);
/* Copies the _file_ 'existingPath' to 'newPath'. Returns the size of the
 * copied file. */
isize si_pathCopy(cstring existingPath, cstring newPath);
/* Copies every file and folder from 'pathSrc' to 'pathDst'. Returns the amount
 * of items that were copied. */
u32 si_pathItemsCopy(cstring pathSrc, cstring pathDst);
/* Moves the file system object from 'existingPath' to 'newPath'. Returns true
 * if the path was moved successfully. */
b32 si_pathMove(cstring existingPath, cstring newPath);
/* Renames 'oldName' path to 'newName. Equivalent to si_pathMove. */
b32 si_pathRename(cstring oldName, cstring newName);
/* Creates a new folder based on the specified path, with the permission being
 * set to SI_FS_PERM_ALL. Returns true if the folder was created. */
b32 si_pathCreateFolder(cstring path);
/* Creates a new folder based on the specified path and permissions. Returns true
 * if the folder was created. */
b32 si_pathCreateFolderEx(cstring path, siFilePermissions perms);
/* Removes the file system object specified at 'path'. Returns true if the removal
 * was successful. */
b32 si_pathRemove(cstring path);
/* Creates a hard link of the specified file. */
b32 si_pathCreateHardLink(cstring existingPath, cstring linkPath);
/* Creates a soft link of the specified file. */
b32 si_pathCreateSoftLink(cstring existingPath, cstring linkPath);
/* Modifies the permissions of the given path. Returns true if the edit succeeded. */
b32 si_pathEditPermissions(cstring path, siFilePermissions newPerms);

/* Returns a pointer that points to the start of the base name. */
cstring si_pathBaseName(cstring path);
cstring si_pathBaseNameLen(cstring path, usize len);
/* Returns a pointer that points to the start of the file extension, if any. */
cstring si_pathExtension(cstring path);
/* Returns a pointer that points to the start of the directory WITHOUT the root
 * directory. Eg. 'res/images/img.png' -> 'images/img.png', 'C:\folder* -> 'folder' */
cstring si_pathUnrooted(cstring path);
/* Returnss a siString of the full filename of the given path and and writes it
 * into the allocator. */
siString si_pathGetFullName(siAllocator* alloc, cstring path);
/* Returns the last time the path was edited. */
u64 si_pathLastWriteTime(cstring path);
/* Returns the system's temporary path. */
cstring si_pathGetTmp(void);
/* Returns the permissions of the given path. */
siFilePermissions si_pathPermissions(cstring path);

/* Returns true if the path is absolute. */
b32 si_pathIsAbsolute(cstring path);
/* Returns true if the path is relative. */
b32 si_pathIsRelative(cstring path);

/*
	========================
	|  siFile              |
	========================
*/

/* Gets the pointer to the specified standard file (stdin, stdout or stderr). */
siFile* si_fileGetStdFile(siStdFileType type);

/* Creates a file if the path doesn't exist. Mode is SI_FILE_WRITE. */
siFile si_fileCreate(cstring path);
/* Opens the provided file. Mode is SI_FILE_READ. */
siFile si_fileOpen(cstring path);
/* The file is opened under the user specified mode. */
siFile si_fileOpenMode(cstring path, siFileMode mode);

/* Returns the size of the file. */
usize si_fileSize(siFile file);
/* Updates the size of the file in the structure. */
void si_fileSizeUpdate(siFile* file);
/* Returns 'true' if the file has been changed since the last write time. */
b32 si_fileHasChanged(siFile file);

/* Allocates 'len' amount of bytes from the allocator, reads that amount from the
 * file's current offset, then returns the buffer. The file stream offset is also
 * increased by 'len' amount. 'SI_FS_READ_BYTES' gets modified by how many bytes
 * were read. */
rawptr si_fileRead(siAllocator* alloc, siFile file, usize len);
rawptr si_fileReadBuf(siFile file, usize len, rawptr outBuffer);
/* Allocates 'bufferLen' amount of bytes from the allocator, reads 'si_min(len, bufferLen)'
 * amount of bytes from the file's current offset, then returns the buffer. The
 * file stream offset is also increased by the amount read. 'SI_FS_READ_BYTES'
 * gets modified by how many bytes were read. */
rawptr si_fileReadEx(siAllocator* alloc, siFile file, usize len, usize bufferLen);
rawptr si_fileReadBufEx(siFile file, usize len, rawptr outBuffer, usize bufferLen);
/* Allocates 'len' amount of bytes from the allocator, reads that amount from the
 * specified offset, then returns the buffer. The file stream offset also changes
 * to ('offset' + 'len'). 'SI_FS_READ_BYTES' gets modified by how many bytes
 * were read. */
rawptr si_fileReadAt(siAllocator* alloc, siFile file, isize offset, usize len);
rawptr si_fileReadAtBuf(siFile file, isize offset, usize len, rawptr outBuffer);
/* Allocates 'bufferLen' amount of bytes from the allocator, reads 'si_min(len, bufferLen)'
 * amount of bytes from the specified offset, then returns the buffer. The file
 * stream offset also changes to 'offset' + 'si_mint(len, bufferLen)'. 'SI_FS_READ_BYTES'
 * gets modified by how many bytes were read. */
rawptr si_fileReadAtEx(siAllocator* alloc, siFile file, isize offset, usize len,
		usize bufferLen);
rawptr si_fileReadAtBufEx(siFile file, isize offset, usize len, rawptr outBuffer,
		usize bufferLen);
/* Allocates 'file.size' amount of bytes from the allocator, reads the entire file's
 * contents, then returns the buffer. The file stream offset does not get changed.
 * 'SI_FS_READ_BYTES' gets modified by how many bytes were read. */
rawptr si_fileReadContents(siAllocator* alloc, siFile file);
rawptr si_fileReadContentsBuf(siFile file, rawptr outBuffer);
/* Allocates 'bufferLen' amount of bytes from the allocator, reads 'si_min(file.size, bufferLen)',
 * amount of bytes, then returns the buffer. The file stream offset does not get
 * changed. 'SI_FS_READ_BYTES' gets modified by how many bytes were read. */
rawptr si_fileReadContentsEx(siAllocator* alloc, siFile file, usize bufferLen);
rawptr si_fileReadContentsBufEx(siFile file, rawptr outBuffer, usize bufferLen);
/* Reads the contents of the file, then splits every line and writes them all
 * into a 'siArray(siString)'. */
siArray(siString) si_fileReadlines(siAllocator* alloc, siFile file);

/* Writes a NULL-terminated C-string into the file from the current stream offset,
 * then returns the amount of bytes that were successfully written. The file stream
 * offset also increments by that amount. */
usize si_fileWrite(siFile* file, cstring content);
/* Writes a C-string with a specified length into the file from the current stream
 * offset, then returns the amount of bytes that were successfully written. The
 * file stream offset also increments by that amount. */
usize si_fileWriteLen(siFile* file, rawptr content, usize len);
/* Writes a NULL-terminated C-string into the file from the specified offset,
 * then returns the amount of bytes that were successfully written. The file stream
 * offset also changes to 'offset + <writtenBytes>'. */
usize si_fileWriteAt(siFile* file, cstring content, isize offset);
/* Writes a C-string with a specified length into the file from the specified
 * offset, then returns the amount of bytes that were successfully written. The
 * file stream offset also changes to 'offset + <writtenBytes>'. */
usize si_fileWriteAtLen(siFile* file, rawptr content, usize contentLen, isize offset);
/* Writes a C-string into the file at the specified line. Writes the amount of
 * characters that were written into the file. */
usize si_fileWriteAtLine(siFile* file, cstring content, usize index);

/* Returns the current offset of the file stream. */
usize si_fileTell(siFile file);
/* Seeks the file stream offset to the specified offset using the given method.
 * Returns the amount of bytes that were read. */
usize si_fileSeek(siFile file, usize offset, siFileMoveMethod method);
/* Seeks to the front of the file. Returns 'true' if the operation went through. */
b32 si_fileSeekFront(siFile file);
/* Seeks to the back of the file. Returns 'true' if the operation went through. */
b32 si_fileSeekBack(siFile file);

/* Truncates the file to the specified size. Returns 'true' if the operation went
 * through. */
b32 si_fileTruncate(siFile* file, usize size);

/* Closes the file. */
void si_fileClose(siFile file);

/*
	========================
	|  siDirectory         |
	========================
*/
typedef SI_ENUM(i32, siIOType) {
	SI_IO_TYPE_ANY = 1,

	/* === Cross-platform === */
	SI_IO_TYPE_FILE,
	SI_IO_TYPE_DIR,
	SI_IO_TYPE_LINK,

	SI_IO_TYPE_BITS = SI_IO_TYPE_FILE | SI_IO_TYPE_DIR | SI_IO_TYPE_LINK,

	/* === THE REST ARE UNIX ONLY. === */
	SI_IO_TYPE_SOCKET,
	SI_IO_TYPE_DEVICE,
	SI_IO_TYPE_BLOCK,
	SI_IO_TYPE_FIFO,

	SI_IO_TYPE_BITS_ALL = SI_IO_TYPE_BITS | SI_IO_TYPE_SOCKET | SI_IO_TYPE_DEVICE | SI_IO_TYPE_BLOCK | SI_IO_TYPE_FIFO
};

typedef struct {
	/* A string containing the path. */
	char path[256];
	/* Actual length of the path. */
	usize len;
	/* An integer denoting if the object is a file, directory or link. */
	siIOType type;
} siDirectoryEntry;

typedef struct {
	/* OS Specific handle. */
	rawptr handle;
	/* Pointer to the base path. */
	char* path;
	/* The path's length. */
	usize pathLen;
} siDirectory;

/* Creates a directory context. Writes a copy of 'path' into the allocator. */
siDirectory si_dirOpen(siAllocator* alloc, cstring path);
/* Polls an entry (file, directory, link, etc) from the directory and edits the
 * contents of 'entry' with the new data. Returns 'true' if a new entry was found,
 * 'false' if there are no more entries in the directory. */
b32 si_dirPollEntry(siDirectory dir, siDirectoryEntry* entry);
/* Polls an entry (file, directory, link, etc) from the directory and edits the
 * contents of 'entry' with the new data. Setting 'fullPath' to true makes 'entry->path'
 * contain both the entry's filename AND the directory 'dir.path'. Returns 'true'
 * if a new entry was found, 'false' if there are no more entries in the directory. */
b32 si_dirPollEntryEx(siDirectory dir, siDirectoryEntry* entry, b32 fullPath);
/* Closes the directory context. */
void si_dirClose(siDirectory dir);

#endif

#if !defined(SI_NO_THREAD)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siThread             |
	========================
*/


typedef struct {
	#if defined(SI_SYSTEM_WINDOWS)
		/* Win32 thread ID. */
		HANDLE id;
	#else
		/* POSIX thread ID. */
		pthread_t id;
	#endif
	/* A boolean indicating if the thread is still running. */
	volatile b32 isRunning;
	/* The rawptr return of the function. */
	rawptr returnValue;

	/* Pointer to the function. */
	siFunction func;
	/* The functions' argument. */
	rawptr arg;
} siThread;

/* TODO(EimaMei): Add mutexes and other threading stuff. */

/* function - NAME | arg - rawptr
 * Returns a siThread from the given function name and argument. */
#define si_threadCreate(function, arg) si_threadCreateEx(siFunc(function), arg)
/* Returns a siThread from the given name and argument. */
siThread si_threadCreateEx(siFunction function, rawptr arg);

/* Starts the thread. */
void si_threadStart(siThread* t);
/* Starts the thread with a specified stack size. */
void si_threadStartStack(siThread* t, usize stackSize);
/* Starts the thread while hanging the parent thread until the specified thread
 * is finished. */
void si_threadJoin(siThread* t);
/* thread - siThread | type - TYPE
 * Casts 'thread.returnValue' correctly to match the given type instead of being rawptr. */
#define si_threadGetReturn(thread, type) (*(type*)(&thread.returnValue))

/* Stops the thread mid-execution. */
void si_threadCancel(siThread* t); /* NOTE(EimaMei): Only works on Unix. */
/* Destroys the thread. */
void si_threadDestroy(siThread* t);

/* Sets the priority of the thread. */
void si_threadPrioritySet(siThread t, i32 priority);

#endif

#if !defined(SI_TIME_UNDEFINE)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siTime               |
	========================
*/

typedef SI_ENUM(usize, siMonth) {
	siJanuary = 1,
	siFebruary,
	siMarch,
	siApril,
	siMay,
	siJune,
	siJuly,
	siAugust,
	siSeptember,
	siOctober,
	siNovember,
	siDecember
};

/* NOTE(EimaMei): This uses the ISO-8601 standard. */
typedef SI_ENUM(usize, siWeek) {
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday
};

/* An unsigned 64-bit integer type, notifying that it's used for time stamps. */
typedef u64 siTimeStamp;
/* A signed 64-bit integer type, notifying that it's used for UTC-0 time. */
typedef i64 siUtcTime;
/* A signed 64-bit integer type, notifying that it's used for local time. */
typedef i64 siLocalTime;

/* Executes the 'RDTSC' asm instruction and returns the value in nano seconds.
 * NOTE: Only natively works for x86-64, i386, ARM64 and PPC CPUs. On other CPUs
 * the function relies on OS functions like 'gettimeofday'. */
siTimeStamp si_RDTSC(void);
/* Returns the number of seconds since 1970-01-01 UTC */
siUtcTime si_timeNowUTC(void);
/* Returns the number of seconds since 1970-01-01 local time.*/
siLocalTime si_timeNowLocal(void);

/* Starts the timestamp. */
siTimeStamp si_timeStampStart(void);
/* Prints the time since the start. */
#define si_timeStampPrintSince(time) si_timeStampPrintSinceEx(time, __FILE__, __LINE__)

/* Makes the CPU sleep for a certain amount of miliseconds. */
void si_sleep(usize miliseconds);


#if 1
	void si_timeStampPrintSinceEx(siTimeStamp t, cstring filename, i32 line);
#endif

#endif

#if !defined(SI_NO_BIT)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siBit                |
	========================
*/

typedef SI_ENUM(usize, siBitType) {
	/* 0b00000000. */
	SI_BIT_ZERO,
	/* 0b00000001. */
	SI_BIT_ONE
};

/* num - INT
 * Gets the most significant bit of the number. */
#define SI_BIT_MSB(num) ((num & SI_BIT(sizeof(typeof(num)) * 8 - 1)) != 0)
/* num - INT
 * Gets the least significant bit of the number. */
#define SI_BIT_LSB(num) ((num & SI_BIT(0)) != 0)

/* bits - INT
 * Shortcut for (bits * 8). */
#define SI_BYTE_TO_BIT(bits) ((bits) * (usize)8)

/* num - INT
 * Gets the upper half of the number */
#define SI_NUM_HIGH_BITS(num) ( \
	(num & ((typeof(num))0xFF << SI_BYTE_TO_BIT(sizeof(typeof(num)) - 1))) \
	>> \
	SI_BYTE_TO_BIT(sizeof(typeof(num)) - 1))

/* num - INT
 * Gets the lower half of the number */
#define SI_NUM_LOW_BITS(num) (num & ((typeof(num))0xFF))

/* value - EXPRESSION | flag - UINT
 * Returns 1 or 0 depending if the bit flag is set in the value. */
#define SI_NUM_BIT_GET(value, flag) (((value) & (flag)) != 0)
/* variable - VARIABLE | flag - UINT | condition - EXPRESSION
 * If 'condition' evaluates to true, the provided 'variable's' 'flag' is set.
 * If it evaluates to false, then the 'flag' is set to 0. */
#define SI_NUM_BIT_SET(variable, flag, condition) \
	(condition) ? ((variable) |= (flag)) : ((variable) &= ~(flag))


/* ptr - rawptr | sizeofNum - usize
 * Extracts 'sizeofNum' amount of bytes from a given pointer. Maximum is 8 bytes. */
#define si_u64FromPtr(ptr, sizeofNum) \
	(*si_cast(u64*, ptr) & ((sizeofNum < 8) ? (SI_BIT((sizeofNum) * 8) - 1) : UINT64_MAX) )

/* x - INT
 * Returns true if x is negative. */
#define si_numIsNeg(x) (x < 0)

/* Returns how many 1 bits are in an 8-bit number. */
usize si_numCountBitsU8(u8 num);
/* Returns how many 1 bits are in a 32-bit number. */
usize si_numCountBitsU32(u32 num);
/* Returns how many 1 bits are in a 64-bit number. */
usize si_numCountBitsU64(u64 num);

#if SI_STANDARD_VERSION >= SI_STANDARD_C11
	#define si_numCountBits(num) \
		_Generic((num), \
			u8  : si_numCountBitsU8(num), \
			u16 : si_numCountBitsU32(num), \
			u32 : si_numCountBitsU32(num), \
			u64 : si_numCountBitsU64(num), \
			default: si_numCountBitsU64(num) \
		)(X)
#endif

/* num - UINT | bitType - siBitType
 * Returns the amount of leading bits of the specified bit type (0 or 1). */
#define si_numLeadingBit(num, bitType) si_numLeadingBitEx(num, sizeof(typeof(num)) * 8, bitType)
/* num - UINT | bitType - siBitType
 * Returns the amount of trailing bits of the specified bit type (0 or 1). */
#define si_numTrailingBit(num, bitType) si_numTrailingBitEx(num, sizeof(typeof(num)) * 8, bitType)
/* num - UINT | bits - usize
 * Rotates the bits of the number left by 'bits' amount. */
#define si_numRotateLeft(num, bits) si_numRotateLeftEx(num, sizeof(typeof(num)) * 8, bits)
/* num - UINT | bits - usize
 * Rotates the bits of the number right by 'bits' amount. */
#define si_numRotateRight(num, bits) siNumRotateRightEx(num, sizeof(typeof(num)) * 8, bits)
/* num - UINT
 * Reverses the bits of the number. */
#define si_numReverseBits(num) siNumReverseBitsEx(num, sizeof(typeof(num)) * 8 )
/* allocator - siAllocator* | num - UINT
 * Creates a 'siArray(u8)' from the specified number and writes it into the allocator. */
#define si_numToBytes(allocator, num) si_numToBytesEx(allocator, num, sizeof(typeof(num)))
/* Makes a number from the specified siArray(u8). */
u64 si_bytesToNumSiArr(siArray(siByte) arr);
/* Makes a number from a C-array. */
u64 si_bytesToNumArr(siByte* array, usize len);

/* Raises an unsigned base 10 by the power of the exponent, and returns the result
 * as a 64-bit unsigned int (meaning the exponent can only be from 0 to 19, otherwise
 * the app will crash). */
u64 si_pow10(u32 exponent);
/* Raises a f64 10 by the power of the exponent. The exponent must be in-between
 * -18 to 18, otherwise the app will crash.*/
f64 si_pow10F64(i32 exponent);

/* Returns the length of a base-10 unsigned number. */
usize si_numLen(u64 num);
/* Returns the length of a specified base unsigned number. */
usize si_numLenEx(u64 num, u32 base);
/* Returns the length of a base-10 signed number. */
usize si_numLenI64(i64 num);
/* Returns the length of a specified base signed number. */
usize si_numLenI64Ex(i64 num, u32 base);
/* Rounds up the number to the nearest multiple and returns it. */
u64 si_numRoundNearestMultiple(u64 num, usize multiple);


#if 1 /* NOTE(EimaMei): The actual header definition for the macros. No reason to use these in practice. */
usize si_numLeadingBitEx(u64 num, usize sizeof_num, siBitType bit);
usize si_numTrailingBitEx(u64 num, usize number_sizeof, siBitType bit);

u64 si_numRotateLeftEx(u64 num, usize num_sizeof, usize bits);
u64 siNumRotateRightEx(u64 num, usize num_sizeof, usize n);
u64 siNumReverseBitsEx(u64 num, usize num_sizeof);

siArray(u8) si_numToBytesEx(siAllocator* alloc, u64 num, usize num_sizeof);
#endif

#endif

#if !defined(SI_NO_CPU)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siCpu                |
	========================
*/
/* The frequency of the CPU in GHz (3, 4.2, etc). Set to 0 by default. When
 * 'si_cpuClockSpeed' is ran, the result gets cached to this variable. */
extern f32 SI_CPU_FREQ_GHZ;

/* Returns the CPU's clock speed in mHz. The res of the function gets cached
 * to 'SI_CPU_FREQ_GHZ'. */
f32 si_cpuClockSpeed(void);

#endif /* SI_NO_CPU */

#if !defined(SI_NO_PRINT)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| si_print             |
	========================
*/

/* Writes a NULL-terminated C-string into SI_STDOUT. Returns the amount of
 * written bytes. */
isize si_print(cstring str);
/* Writes a NULL-terminated formatted C-string into SI_STDOUT. Returns the amount
 * of written bytes. */
isize si_printf(cstring fmt, ...);
/* Writes a NULL-terminated formatted C-string from specified va_list into SI_STDOUT.
 * Returns the amount of written bytes. */
isize si_printfVa(cstring fmt, va_list va);

/* Writes a NULL-terminated C-string into a specified file. Returns the amount of
 * written bytes. */
isize si_fprint(siFile* file, cstring str);
/* Writes a NULL-terminated formatted C-string into a specified file. Returns the
 * amount of written bytes. */
isize si_fprintf(siFile* file, cstring fmt, ...);
/* Writes a NULL-terminated formatted C-string from specified 'va_list' into the
 * specified file. Returns the amount of written bytes. */
isize si_fprintfVa(siFile* file, cstring fmt, va_list va);

/* Writes a NULL-terminated formatted C-string into the 'buffer'. Returns the
 * amount of written bytes. */
isize si_sprintf(char* buffer, cstring fmt, ...);
/* Writes a NULL-terminated formatted C-string into the 'buffer' with a specified
 * buffer capacity. Returns the amount of written bytes. */
isize si_snprintf(char* buffer, usize outCapacity, cstring fmt, ...);
/* Writes a NULL-terminated formatted C-string from specified va_list into the
 * 'buffer'. Returns the amount of written bytes. */
isize si_sprintfVa(char* buffer, cstring fmt, va_list va);
/* Writes a NULL-terminated formatted C-string from specified va_list into the
 * 'buffer' with a specified capacity. Returns the amount of written bytes. */
isize si_snprintfVa(char* buffer, usize outCapacity, cstring fmt, va_list va);

#endif /* !defined(SI_NO_PRINT) */

#if !defined(SI_NO_DLL)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siDll                |
	========================
*/

/* A type denotating that the value is a DLL handle. */
typedef rawptr siDllHandle;
/* A type denotating that the value is a DLL process/function. */
typedef void (*siDllProc)(void);

/* Loads the specified path as a DLL and returns the handle. */
siDllHandle si_dllLoad(cstring path);
/* Unloads the specified DLL. */
void si_dllUnload(siDllHandle dll);
/* Returns the pointer to the specified processor name of the DLL. */
siDllProc si_dllProcAddress(siDllHandle dll, cstring name);

#endif /* !defined(SI_NO_DLL) */


#if !defined(SI_NO_MATH)
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siMath               |
	========================
*/

/* Returns the smaller of its arguments: either a or b.*/
i64 si_min(i64 a, i64 b);
f64 si_fmin(f64 a, f64 b);
f32 si_fminf(f32 a, f32 b);
/* Returns the smaller of its arguments: either a, b or c.*/
i64 si_min3(i64 a, i64 b, i64 c);
f64 si_fmin3(f64 a, f64 b, f64 c);
f32 si_fminf3(f32 a, f32 b, f32 c);

/* Returns the larger of its arguments: either a or b.*/
i64 si_max(i64 a, i64 b);
f64 si_fmax(f64 a, f64 b);
f32 si_fmaxf(f32 a, f32 b);
/* Returns the larger of its arguments: either a, b or c.*/
i64 si_max3(i64 a, i64 b, i64 c);
f64 si_fmax3(f64 a, f64 b, f64 c);
f32 si_fmaxf3(f32 a, f32 b, f32 c);

/* Returns the absolute value of 'x'. */
i64 si_abs(i64 x);
f64 si_fabs(f64 x);
f32 si_fabsf(f32 x);

/* Returns a boolean that indicates if 'x' is between 'lower' and 'upper'. */
b32 si_between(i64 x, i64 lower, i64 upper);
b32 si_betweenu(u64 x, u64 lower, u64 upper);
b32 si_fbetween(f64 x, f64 lower, f64 upper);
b32 si_fbetweenf(f32 x, f32 lower, f32 upper);
#define si_betweenPtr(x, lower, upper) si_betweenu((u64)(x), (u64)(lower), (u64)(upper))

#endif


#if !defined(SI_NO_BENCHMARK)

/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' times and prints how long it took to finish. */
#define si_benchmarkRunsPerLoop(timesToLoop, function) \
	do { \
		usize index; \
		clock_t timeSince = clock(); \
		for (index = 0; index < (usize)timesToLoop; index++) { \
			function; \
		} \
		timeSince = clock() - timeSince; \
		f64 timeTaken = ((f64)timeSince) / 1000.0; /* NOTE(EimaMei): This will take the time in ms, not seconds. */ \
		si_printf(SI_PERFORMANCE_MSG, #function, timeTaken, (f64)timesToLoop / 1000000.0); \
	} while(0)
/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' for 'ms' miliseconds and prints how many times
 * the function got executed.  */
#define si_benchmarkExecutesPerMs(ms, function) \
	do { \
		u64 counter = 0; \
		u32 miliseconds = (u32)ms; \
		siThread thread = si_threadCreate(si_intern_benchmarkThread, &miliseconds); \
		si_threadStart(&thread); \
		while (thread.isRunning) { \
			function; \
			counter += 1; \
		} \
		si_printf(SI_PERFORMANCE_MSG, #function, (f64)miliseconds, (f64)counter / 1000000.0); \
	} while(0)

/* multiplesOf10 - usize | function - NAME
 * Runs the function through multiples of 10 until it reaches 'multiplesOf10'.
 * Then it prints the benchmarking data for each run, as well as the combined
 * average of all of the runs. */
#define si_benchmarkLoopsAvg(multiplesOf10, function) \
	si_benchmarkLoopsAvgRange(1, multiplesOf10, function)
/* start - usize | end - usize | function - NAME
 * Runs the function through multiples of 10 from 'start' until it reaches 'end'.
 * Then it prints the benchmarking data for each run, as well as the combined
 * average of all of the runs. */
#define si_benchmarkLoopsAvgRange(start, end, function) \
	do { \
		siAllocator* alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siArray(u64) cycles = si_arrayMakeReserve(alloc, sizeof(u64), 20); \
		si_benchmarkLoop(function, cycles, start, end); \
		\
		si_benchmarkLoopsAvgPrint(alloc, #function, start, end, cycles, si_cpuClockSpeed()); \
	} while(0)
/* multiplesOf10 - usize | function1 - NAME | function2 - NAME
 * Runs both functions through multiples of 10 until it reaches 'multiplesOf10'.
 * Then it prints the benchmarking data for each functions' runs, as well as the
 * combined average of all of the runs. All of this data then gets compared between
 * function1 vs function2 (ON UNIX: green text - faster, red text - slower). */
#define si_benchmarkLoopsAvgCmp(multiplesOf10, function1, function2) \
	si_benchmarkLoopsAvgCmpRange(1, multiplesOf10, function1, function2)
/* start - usize | end - usize | function1 - NAME | function2 - NAME
 * Runs both functions through multiples of 10 from 'start' until it reaches 'end'.
 * Then it prints the benchmarking data for each functions' runs, as well as the
 * combined average of all of the runs. All of this data then gets compared between
 * function1 vs function2 (ON UNIX: green text - faster, red text - slower). */
#define si_benchmarkLoopsAvgCmpRange(start, end, function1, function2) \
	do { \
		siAllocator* alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siArray(u64) cycles[2] = { \
			(u64*)si_arrayMakeReserve(alloc, sizeof(u64), 20), \
			(u64*)si_arrayMakeReserve(alloc, sizeof(u64), 20)  \
		};\
		si_benchmarkLoop(function1, cycles[0], start, end); \
		si_benchmarkLoop(function2, cycles[1], start, end); \
		\
		cstring funcnames[2] = {#function1, #function2}; \
		si_benchmarkLoopsAvgCmpPrint(alloc, funcnames, start, end, cycles, si_cpuClockSpeed()); \
	} while(0)

typedef struct { siTimeStamp first; cstring second; } siBenchmarkLimit;

#define SI_PERFORMANCE_MSG \
	"====== BENCHMARK DATA ======\n" \
	"General:\n" \
		"\tFunction - '%s'\n" \
		"\tDuration - '%f' ms\n" \
		"\tExecution count - '%f' million\n"

#define si_benchmarkLoop(function, cycles, start, end) \
	do { \
		usize arrayIndex = 0; \
		u64 medianIndex; \
		for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) { \
			u64 index; \
			u64 begin = si_RDTSC(); \
			for (index = 0; index < medianIndex; index++) { \
				(void)function; \
			} \
			begin = si_RDTSC() - begin; \
			cycles[arrayIndex] = begin; \
			arrayIndex += 1; \
		} \
		SI_ARRAY_HEADER(cycles)->len = arrayIndex; \
	} while (0)

extern const siBenchmarkLimit siBenchLimit[];

const siBenchmarkLimit* si_benchmarkLimitLoop(siTimeStamp time);
void si_intern_benchmarkThread(u32* arg);

#endif

/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| SI_IMPLEMENTATION    |
	========================
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*/

#if defined(SI_IMPLEMENTATION)
	#define SI_IMPLEMENTATION_GENERAL
	#define SI_IMPLEMENTATION_ALLOCATOR
	#define SI_IMPLEMENTATION_PAIR
	#define SI_IMPLEMENTATION_OPTIONAL
	#define SI_IMPLEMENTATION_ARRAY
	#define SI_IMPLEMENTATION_STRING
	#define SI_IMPLEMENTATION_UNICODE
	#define SI_IMPLEMENTATION_CHAR
	#define SI_IMPLEMENTATION_HASHTABLE
	#define SI_IMPLEMENTATION_IO
	#define SI_IMPLEMENTATION_THREAD
	#define SI_IMPLEMENTATION_BIT
	#define SI_IMPLEMENTATION_CPU
	#define SI_IMPLEMENTATION_PRINT
	#define SI_IMPLEMENTATION_DLL
	#define SI_IMPLEMENTATION_MATH
	#define SI_IMPLEMENTATION_BENCHMARK
#endif


#if defined(SI_IMPLEMENTATION_ALLOCATOR)

F_TRAITS(inline)
usize si_alignCeil(usize n) {
	SI_STOPIF(n % SI_DEFAULT_MEMORY_ALIGNMENT == 0, return n);
	n |= SI_DEFAULT_MEMORY_ALIGNMENT - 1;
	n += 1;
	return n;
}
F_TRAITS(inline)
usize si_alignCeilEx(usize n, u32 alignment) {
	SI_STOPIF(n % alignment == 0, return n);
	n |= alignment - 1;
	n += 1;
	return n;
}

F_TRAITS(inline)
siAllocator* si_allocatorMake(usize bytes) {
#if !defined(SI_NO_ALLOCATOR)
	rawptr ptr = malloc(sizeof(siAllocator) + bytes);
	SI_ASSERT_NOT_NULL(ptr);

	siAllocator* res = (siAllocator*)ptr;
	res->ptr = (siByte*)ptr + sizeof(siAllocator);
	res->offset = 0;
	res->maxLen = bytes;

	return res;
#else
	SI_UNUSED(bytes);
	return nil;
#endif
}
F_TRAITS(inline)
siAllocator* si_allocatorMakeStackEx(usize bytes, rawptr ptr) {
#if !defined(SI_NO_ALLOCATOR)
	siAllocator* res = (siAllocator*)ptr;
	res->ptr = (siByte*)ptr + sizeof(siAllocator);
	res->offset = 0;
	res->maxLen = bytes;

	return res;
#else
	SI_UNUSED(bytes); SI_UNUSED(ptr);
	return nil;
#endif
}
F_TRAITS(inline)
siAllocator si_allocatorMakeTmp(rawptr pointer, usize maxLen) {
	siAllocator alloc;
	alloc.ptr = (siByte*)pointer;
	alloc.maxLen = maxLen;
	alloc.offset = 0;

	return alloc;
}
F_TRAITS(inline)
void si_allocatorFree(siAllocator* alloc) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc->ptr);
	free(alloc->ptr - sizeof(siAllocator));
#else
	SI_UNUSED(alloc);
#endif
}

F_TRAITS(inline)
void si_allocatorReset(siAllocator* alloc) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	alloc->offset = 0;
#else
	SI_UNUSED(alloc);
#endif
}

F_TRAITS(inline)
void si_allocatorResetFrom(siAllocator* alloc, usize offset) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_MSG(offset < alloc->maxLen, "Provided offset is too large.");
	alloc->offset = offset;
#else
	SI_UNUSED(alloc); SI_UNUSED(offset);
#endif
}
F_TRAITS(inline)
usize si_allocatorAvailable(siAllocator* alloc) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	return alloc->maxLen - alloc->offset;
#else
	return 0;
#endif
}
F_TRAITS(inline)
rawptr si_allocatorCurPtr(siAllocator* alloc) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	return si_cast(rawptr, alloc->ptr + alloc->offset);
#else
	return 0;
#endif
}
F_TRAITS(inline)
void si_allocatorPush(siAllocator* alloc, siByte byte) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_FMT(alloc->offset + 1 < alloc->maxLen,
		"Exceeded the available memory for an allocation",
		" (Tried writing '%zd' bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
		alloc->maxLen, alloc->offset);

	alloc->ptr[alloc->offset] = byte;
	alloc->offset += 1;
#else
	SI_UNUSED(alloc); SI_UNUSED(byte);
#endif
}
#if !defined(SI_NO_ALLOC_DEBUG_INFO)
F_TRAITS(inline)
rawptr si_mallocEx(siAllocator* alloc, usize bytes, cstring filename, i32 line) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);

	siByte* res = alloc->ptr + alloc->offset;
	alloc->offset += bytes;

	if (alloc->offset > alloc->maxLen) {
		si_fprintf(SI_STDERR,
			"%s:%i: si_malloc: Exceeded the available memory for an allocation"
			" (Tried writing '%zd' bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
			  filename, line, bytes, alloc->maxLen, alloc->offset
		);
		exit(1);
	}

	return res;
#else
	return malloc(bytes);
	SI_UNUSED(alloc); SI_UNUSED(bytes); SI_UNUSED(filename); SI_UNUSED(line);
#endif
}

F_TRAITS(inline)
rawptr si_callocEx(siAllocator* alloc, usize num, usize bytes,
		cstring filename, i32 line) {
#if !defined(SI_NO_ALLOCATOR)
	rawptr res = si_mallocEx(alloc, num * bytes, filename, line);
	return memset(res, 0, num * bytes);
#else
	return calloc(bytes, num);
	SI_UNUSED(alloc); SI_UNUSED(num); SI_UNUSED(bytes) SI_UNUSED(filename); SI_UNUSED(line);
#endif
}
rawptr si_reallocEx(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize,
		cstring filename, i32 line) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	SI_STOPIF(ptr == nil, return si_malloc(alloc, newSize));

	SI_ASSERT_FMT(
		si_betweenPtr(ptr, alloc->ptr, alloc->ptr + alloc->maxLen),
		"%s:%i: Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
		filename, line, ptr, alloc->ptr, alloc->ptr + alloc->maxLen
	);
	rawptr out = si_malloc(alloc, newSize);
	memcpy(out, ptr, oldSize);
	return out;
#else
	return realloc(ptr, newSize);
	SI_UNUSED(alloc); SI_UNUSED(ptr); SI_UNUSED(oldSize); SI_UNUSED(newSize) SI_UNUSED(filename); SI_UNUSED(line);
#endif
}

#else
F_TRAITS(inline)
rawptr si_malloc(siAllocator* alloc, usize bytes) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);

	siByte* res = alloc->ptr + alloc->offset;
	alloc->offset += bytes;

	SI_ASSERT_FMT(
		alloc->offset < alloc->maxLen,
		"Exceeded the available memory for an allocation ((Tried writing '%zd'"
		"bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
		bytes, alloc->maxLen, alloc->offset
	);

	return res;
#else
	return malloc(bytes);
	SI_UNUSED(alloc); SI_UNUSED(bytes); SI_UNUSED(filename); SI_UNUSED(line);
#endif
}

F_TRAITS(inline)
rawptr si_calloc(siAllocator* alloc, usize num, usize bytes) {
#if !defined(SI_NO_ALLOCATOR)
	rawptr res = si_malloc(alloc, num * bytes);
	return memset(res, 0, num * bytes);
#else
	return calloc(bytes, num);
	SI_UNUSED(alloc); SI_UNUSED(num); SI_UNUSED(bytes) SI_UNUSED(filename); SI_UNUSED(line);
#endif
}
rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	SI_STOPIF(ptr == nil, return si_malloc(alloc, newSize));

	SI_ASSERT_FMT(
		si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->maxLen),
		"Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
		ptr, alloc->ptr, alloc->ptr + alloc->maxLen
	);
	rawptr out = si_malloc(alloc, newSize);
	memcpy(out, ptr, oldSize);
	return out;
#else
	return realloc(ptr, newSize);
	SI_UNUSED(alloc); SI_UNUSED(ptr); SI_UNUSED(oldSize); SI_UNUSED(newSize) SI_UNUSED(filename); SI_UNUSED(line);
#endif
}
#endif /* SI_NO_ALLOC_DEBUG_INFO */

F_TRAITS(inline)
void si_free(siAllocator* alloc, rawptr ptr) {
#if !defined(SI_NO_ALLOCATOR)
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(ptr);
	SI_ASSERT_FMT(
		si_betweenPtr(ptr, alloc->ptr, alloc->ptr + alloc->maxLen),
		"Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
		ptr, alloc->ptr, alloc->ptr + alloc->maxLen
	);
#else
	free(ptr);
	SI_UNUSED(alloc); SI_UNUSED(ptr); SI_UNUSED(filename); SI_UNUSED(line);
#endif
}


#endif /* SI_IMPLEMENTATION_ALLOCATOR */


#if defined(SI_IMPLEMENTATION_GENERAL)

usize si_assertEx(b32 condition, cstring conditionStr, cstring file, i32 line, cstring func, cstring message, ...) {
	SI_STOPIF(condition, return 0);
	si_fprintf(
		SI_STDERR,
		"Assertion \"%s\" at \"%s:%d\": %s%s",
		conditionStr, file, line, func, (message != nil ? ": " : "\n")
	);

	if (message != nil) {
		va_list va;
		va_start(va, message);
		si_fprintfVa(SI_STDERR, message, va);
		si_fprint(SI_STDERR, "\n");
		va_end(va);
	}

	abort();
	return 1;
}

F_TRAITS(inline)
siUtcTime si_timeNowUTC(void) {
	time_t rawtime;
	time(&rawtime);

	return rawtime;
}
siLocalTime si_timeNowLocal(void) {
#if defined(SI_SYSTEM_WINDOWS)
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	return (i64)ft.dwHighDateTime | ((i64)ft.dwLowDateTime << 32);
#else
	struct timeval tv;
	i32 res = gettimeofday(&tv, nil);
	SI_STOPIF(res == 1, return 0);

	return tv.tv_sec;
#endif
}

F_TRAITS(inline)
void si_sleep(usize miliseconds) {
#if defined(SI_SYSTEM_WINDOWS)
	Sleep(miliseconds);
#else
	struct timespec ts = {
		(time_t)miliseconds / 1000,
		si_cast(time_t, miliseconds % 1000) * 1000000
	};
	nanosleep(&ts, &ts);
#endif
}

F_TRAITS(inline)
siTimeStamp si_RDTSC(void) {
	/* NOTE(EimaMei): Shoutout to gb.h for the i386 and PPC code! (Link: https://github.com/gingerBill/gb/blob/master/gb.h#L8682C1-L8715C7). */
#if !defined(SI_NO_INLINE_ASM)
	#if defined(SI_COMPILER_MSVC)
		return __rdtsc();
	#elif defined(SI_CPU_X86) && !defined(SI_ARCH_64_BIT)
		u64 res;
		si_asm (".byte 0x0f, 0x31", : "=A" (res));
		return res;
	#elif defined(SI_CPU_X86) && defined(SI_ARCH_64_BIT)
		u64 res;
		si_asm(
			"rdtsc"           SI_ASM_NL
			"shl rdx, 0x20"   SI_ASM_NL
			"or rax, rdx",
			SI_ASM_OUTPUT("=a"(res))
		);
		return res;
	#elif defined(SI_CPU_PPC)
		u32 high, low, tmp;
		si_asm (
			"0:"            SI_ASM_NL
			"mftbu %0"      SI_ASM_NL
			"mftb %1"       SI_ASM_NL
			"mftbu %2"      SI_ASM_NL
			"cmpw %2, %0"   SI_ASM_NL
			"bne 0b",
			SI_ASM_OUTPUT("=r"(high), "=r"(low), "=r"(tmp))
		);
		return ((u64)high << 32) | low;
	#elif defined(SI_CPU_ARM64)
		u64 res;
		si_asm ("mrs %0, cntvct_el0", SI_ASM_OUTPUT("=r"(res)));
		return res;
	#elif defined(SI_SYSTEM_WINDOWS)
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return count.QuadPart;
	#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000 + tv.tv_usec;
	#endif
#else
	#if defined(SI_SYSTEM_WINDOWS)
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return count.QuadPart;
	#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000 + tv.tv_usec;
	#endif
#endif /* SI_NO_INLINE_ASM */
}
F_TRAITS(inline)
siTimeStamp si_timeStampStart(void) {
	return si_RDTSC();
}
void si_timeStampPrintSinceEx(siTimeStamp ts, cstring filename, i32 line) {
	u64 end = si_RDTSC();
	u64 diff = (end - ts) / (si_cpuClockSpeed());

	const siBenchmarkLimit* time = si_benchmarkLimitLoop(diff);
	si_printf(
		"si_timeStampPrintSince: %s:%i: TIME: %.2f %s\n",
		filename, line, (f32)diff / time->first, time->second
	);
}

F_TRAITS(inline)
void si_ptrMoveBy(rawptr src, usize srcLen, isize moveBy, siDirection direction) {
	isize mul = (direction == SI_DIRECTION_RIGHT ? 1 : -1); /* NOTE(EimaMei): Branchless way of moving. */
	memcpy((siByte*)src + (moveBy * mul), src, srcLen);
}

#endif /* SI_IMPLEMENTATION_GENERAL */


#if defined(SI_IMPLEMENTATION_PAIR) && !defined(SI_NO_PAIR)
F_TRAITS(inline)
rawptr si_intern_pairMake(siAllocator* alloc, siAny first, siAny second) {
	siByte* res = (siByte*)si_malloc(alloc, first.typeSize + second.typeSize);
	memcpy(res, first.ptr, first.typeSize);
	memcpy(res + first.typeSize, second.ptr, second.typeSize);

	return (rawptr)res;
}
#endif /* SI_IMPLEMENTATION_PAIR */

#if defined(SI_IMPLEMENTATION_OPTIONAL) && !defined(SI_NO_OPTIONAL)
F_TRAITS(inline)
rawptr si_optionalMakeEx(rawptr res, siAny any, usize offset) {
	siByte* ptr = (siByte*)res;
	*(b32*)ptr = true;
	memcpy(ptr + offset, any.ptr, any.typeSize);
	return res;
}
#endif /* SI_IMPLEMENTATION_OPTIONAL */

#if defined(SI_IMPLEMENTATION_ARRAY) && !defined(SI_NO_ARRAY)
F_TRAITS(inline)
rawptr si_arrayMakeList(siAllocator* alloc, rawptr list, usize sizeofItem, usize count)  {
	rawptr array = si_arrayMakeReserve(alloc, sizeofItem, count);
	memcpy(array, list, sizeofItem * count);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;

	return array;
}
F_TRAITS(inline)
rawptr si_arrayCopy(siAllocator* alloc, rawptr array) {
	siArrayHeader* header = SI_ARRAY_HEADER(array);
	return si_arrayMakeList(alloc, array, header->typeSize, header->len);
}
rawptr si_arrayMakeReserve(siAllocator* alloc, usize sizeofItem, usize count) {
	SI_STOPIF(count == 0, count = 1);

	rawptr ptr = si_malloc(alloc, sizeof(siArrayHeader) + (sizeofItem * count));
	rawptr array = (siByte*)ptr + sizeof(siArrayHeader);

	siArrayHeader* header = (siArrayHeader*)ptr;
	header->len = 0;
	header->capacity = count;
	header->typeSize = sizeofItem;
	header->allocator = alloc;

	return array;
}

F_TRAITS(inline)
usize si_arrayLen(rawptr array) {
	return SI_ARRAY_HEADER(array)->len;
}
F_TRAITS(inline)
usize si_arrayCapacity(rawptr array) {
	return SI_ARRAY_HEADER(array)->capacity;
}
F_TRAITS(inline)
usize si_arrayTypeSize(rawptr array) {
	return SI_ARRAY_HEADER(array)->typeSize;
}
F_TRAITS(inline)
usize si_arrayTotalSize(rawptr array) {
	return si_arrayCapacity(array) * si_arrayTypeSize(array);
}
inline siAllocator* si_arrayAllocator(rawptr array) {
	return SI_ARRAY_HEADER(array)->allocator;
}
F_TRAITS(inline)
b32 si_arrayEmpty(rawptr array) {
	return (si_arrayLen(array) == 0 || array == nil);
}

F_TRAITS(inline)
rawptr si_arrayAt(rawptr array, usize index) {
	SI_STOPIF(index >= si_arrayLen(array), return nil);

	return si_arrayGetPtr(array, index);
}
F_TRAITS(inline)
rawptr si_arrayFront(rawptr array) {
	SI_ASSERT_MSG(si_arrayLen(array) != 0, "Array is empty.");
	return array;
}
F_TRAITS(inline)
rawptr si_arrayBack(rawptr array) {
	SI_ASSERT_MSG(si_arrayLen(array) != 0, "Array is empty.");
	return si_arrayGetPtr(array, si_arrayLen(array) - 1);
}

isize si_intern_arrayFind(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start < end, "Value 'start' is larger than 'end'");

	b32 found = false;
	usize i;
	for (i = start; i < end; i++) {
		if (memcmp((siByte*)array + i * si_arrayTypeSize(array), value.ptr, value.typeSize) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}
isize si_intern_arrayRFind(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start >= end, "Value 'end' is larger than 'start'");

	b32 found = false;
	usize i;
	for (i = start; i < end - 1; i--) {
		if (memcmp((siByte*)array + i * si_arrayTypeSize(array), value.ptr, value.typeSize) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}

void si_intern_arrayReplace(rawptr array, siAny oldValue, siAny newValue) {
	SI_ASSERT_NOT_NULL(array);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	SI_ASSERT_MSG(
		si_between(newValue.typeSize, oldValue.typeSize, header->typeSize),
		"The given value's sizeof is too large compared to the elements' in the array."
	);

	isize index = 0;
	while (true) {
		index = si_intern_arrayFind(array, index, header->len, oldValue);
		if (index == SI_ERROR) {
			return ;
		}

		memcpy(si_arrayGetPtr(array, index), newValue.ptr, newValue.typeSize);
	}
}
void si_arrayReverse(rawptr arrayPtr) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = si_cast(siByte*, arrayPtr);

	usize len = si_arrayLen(array);
	usize typeSize = si_arrayTypeSize(array);
	SI_ASSERT_MSG(len != 0, "Array is empty");

	siByte* a = array;
	siByte* b = array + (len - 1) * typeSize;
	rawptr tmp[1];
	len /= 2;

	while (len -= 1) {
		memcpy(tmp, a, typeSize);
		memcpy(a, b, typeSize);
		memcpy(b, tmp, typeSize);

		a += typeSize;
		b -= typeSize;
	}
}
siString si_arrayToSistring(siArray(char*) array, cstring separator) {
	SI_ASSERT_NOT_NULL(array);

	usize separatorLen = si_cstrLen(separator);
	usize totalSize = 0;

	for_range (i, 0, si_arrayLen(array)) {
		totalSize += si_cstrLen(array[i]) + separatorLen;
	}

	siString res = si_stringMakeReverse(SI_ARRAY_HEADER(array)->allocator, totalSize);
	rawptr backPtr = si_arrayBack(array);
	for_each (str, array) {
		if (separator != nil && (&str) != backPtr) {
			si_stringJoin(&res, separator, *str);
		}
		else {
			si_stringAppend(&res, *str);
		}
	}

	return res;
}

F_TRAITS(inline)
void si_arrayPop(rawptr arrayPtr) {
	siByte* array = *si_cast(siByte**, arrayPtr);
	SI_ARRAY_HEADER(array)->len -= 1;
}

void si_intern_arrayInsert(rawptr arrayPtr, siAny newItem, usize index) {
	SI_ASSERT_NOT_NULL(arrayPtr);

	siByte* array = *si_cast(siByte**, arrayPtr);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	usize previousLen = header->len;
	usize beforeIndexLen = previousLen - index;

	SI_ASSERT_FMT(index < previousLen, "Index is higher than the length of the array (array - '%zd', index - '%zd')", previousLen, index);
	header->len += 1;

	if (header->capacity < header->len) {
		rawptr res = si_realloc(
			header->allocator,
			header,
			sizeof(siArrayHeader) + si_arrayTotalSize(array),
			sizeof(siArrayHeader) + 2 * (header->capacity * header->typeSize)
		);

		array = (siByte*)res + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)res;
		header->capacity *= 2;
	}

	memcpy(
		array + (header->len - beforeIndexLen) * header->typeSize,
		array + index * header->typeSize,
		beforeIndexLen * header->typeSize
	);
	memcpy(array + index * header->typeSize, newItem.ptr, newItem.typeSize);
}
F_TRAITS(inline)
void si_arrayErase(rawptr arrayPtr, usize index) {
	si_arrayEraseCount(arrayPtr, index, 1);
}
void si_arrayEraseCount(rawptr arrayPtr, usize index, usize count) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	usize len = si_arrayLen(array);
	SI_ASSERT_MSG(index < len, "Index is higher than the length of the string.");
	SI_ASSERT_FMT(index + count <= len, "Index + count is higher than the length of the array (%zd > %zd).", index + count, len);

	memcpy(array + index * si_arrayTypeSize(array), array + (index + count) * si_arrayTypeSize(array), (len - index - count) * si_arrayTypeSize(array));

	SI_ARRAY_HEADER(array)->len -= count;
}
void si_intern_arrayRemoveItem(rawptr arrayPtr, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	while (header->len != 0) {
		isize index = si_intern_arrayRFind(array, header->len - 1, 0, value);
		if (index == -1) {
			break;
		}

		memcpy(si_arrayGetPtr(array, index), si_arrayGetPtr(array, index + header->len), value.typeSize);

		if (header->len != 0) {
			header->len -= 1;
		}
	}
}
void si_intern_arrayFill(rawptr arrayPtr, usize index, usize count, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(value.typeSize <= header->typeSize, "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(index < header->capacity, "Index is higher than the array's length.");

	usize previousLen = header->len;
	isize size_dif = si_abs((isize)index - (isize)count);
	header->len += size_dif;

	if (header->capacity < header->len) {
		rawptr res = si_realloc(
			header->allocator,
			header,
			sizeof(siArrayHeader) + si_arrayTotalSize(array),
			sizeof(siArrayHeader) + size_dif +  2 * header->capacity * header->typeSize
		);
		SI_ASSERT_NOT_NULL(res);

		array = (siByte*)res + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)res;
		header->capacity = header->capacity * 2 + size_dif;
	}

	for_range(i, index, count) {
		memcpy(si_arrayGetPtr(array, previousLen + i), value.ptr, header->typeSize);
	}
}

rawptr si_intern_arrayAppend(rawptr arrayPtr, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(
		value.typeSize <= header->typeSize,
		"The given value's sizeof is too large compared to the elements' in the array."
	);

	usize previousLen = header->len;
	header->len += 1;

	if (header->capacity < header->len) {
		rawptr res = si_realloc(
			header->allocator,
			header,
			sizeof(siArrayHeader) + si_arrayTotalSize(array),
			sizeof(siArrayHeader) + 2 * si_arrayTotalSize(array)
		);

		array = (siByte*)res + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)res;
		header->capacity *= 2;
	}

	return memcpy(si_arrayGetPtr(array, previousLen), value.ptr, value.typeSize);
}

void si_arrayClear(rawptr arrayPtr) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	rawptr array = si_cast(siByte*, arrayPtr);

	memset(array, 0, si_arrayTotalSize(array));
}

b32 si_arrayEqual(rawptr lha, rawptr rha) {
	siArrayHeader* lha_header = SI_ARRAY_HEADER(lha);
	siArrayHeader* rha_header = SI_ARRAY_HEADER(rha);

	if (lha_header->len != rha_header->len || lha_header->typeSize != rha_header->typeSize) {
		return false;
	}

	usize width = lha_header->typeSize;
	usize i;
	for (i = 0; i < lha_header->len; i++) {
		if (memcmp((siByte*)lha + i * width, (siByte*)rha + i * width, width) != SI_OKAY) {
			return false;
		}
	}

	return true;
}

F_TRAITS(inline)
void si_arrayFree(rawptr array) {
	SI_ASSERT_NOT_NULL(array);
	free(SI_ARRAY_HEADER(array));
}

void si_arrayShrinkToFit(rawptr arrayPtr) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	header = (typeof(header))si_realloc(
		header->allocator,
		header,
		sizeof(siArrayHeader) + si_arrayTotalSize(array),
		sizeof(siArrayHeader) + header->len * header->typeSize
	);
	header->capacity = header->len;
}

F_TRAITS(inline)
u64 si_arraySumU64(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	u64 res = 0;
	siByte* ptr = (siByte*)array;
	siByte* end = ptr + header->len * header->typeSize;

	while (ptr < end) {
		res += si_u64FromPtr(ptr, header->typeSize);
		ptr += header->typeSize;
	}

	return res;
}
F_TRAITS(inline)
i64 si_arraySumI64(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	i64 res = 0;
	siByte* ptr = (siByte*)array;
	siByte* end = ptr + header->len * header->typeSize;

	while (ptr < end) {
		res += (i64)si_u64FromPtr(ptr, header->typeSize);
		ptr += header->typeSize;
	}

	return res;
}
F_TRAITS(inline)
f64 si_arraySumF64(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array );
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	f64 res = 0;
	siByte* ptr = (siByte*)array;
	siByte* end = ptr + header->len * header->typeSize;

	while (ptr < end) {
		res += (f64)si_u64FromPtr(ptr, header->typeSize);
		ptr += header->typeSize;
	}

	return res;
}
#endif /* SI_IMPLEMENTATION_ARRAY */

#if defined(SI_IMPLEMENTATION_STRING) && !defined(SI_NO_STRING)

F_TRAITS(inline)
siString si_stringMake(siAllocator* alloc, cstring str) {
	return si_stringMakeLen(alloc, str, str ? si_cstrLen(str) : 0);
}
F_TRAITS(inline)
siString si_stringMakeLen(siAllocator* alloc, cstring str, usize len) {
	return si_stringMakeEx(alloc, str, len, len);
}
siString si_stringMakeEx(siAllocator* alloc, cstring str, usize len, usize capacity) {
	siString resStr = si_stringMakeReverse(alloc, capacity);
	memcpy(resStr, str, len);

	siStringHeader* header = SI_STRING_HEADER(resStr);
	header->len = len;

	return resStr;
}
siString si_stringMakeFmt(siAllocator* alloc, cstring str, ...) {
	SI_ASSERT_NOT_NULL(str);

	va_list va;
	va_start(va, str);
	char buffer[SI_KILO(4)];
	usize len = si_snprintfVa(buffer, sizeof(buffer), str, va);
	va_end(va);

	return si_stringMakeLen(alloc, buffer, len);
}
siString si_stringMakeReverse(siAllocator* alloc, usize capacity) {
	rawptr ptr = si_malloc(alloc, sizeof(siStringHeader) + (capacity +  1));
	SI_ASSERT_NOT_NULL(ptr);

	siString resStr = (siString)ptr + sizeof(siStringHeader);
	resStr[capacity] = '\0';

	siStringHeader* header = SI_STRING_HEADER(resStr);
	header->len = 0;
	header->capacity = capacity;
	header->allocator = alloc;
	/*header->typeSize = sizeof(char); */

	return resStr;
}

F_TRAITS(inline)
siString si_stringCopy(siAllocator* alloc, siString from) {
	return si_stringMakeLen(alloc, from, si_stringLen(from));
}

F_TRAITS(inline)
usize si_stringLen(siString str) {
	return SI_STRING_HEADER(str)->len;
}
F_TRAITS(inline)
usize si_stringCapacity(siString str) {
	return SI_STRING_HEADER(str)->capacity;
}
F_TRAITS(inline)
usize si_stringTypeSize(siString str) {
	return sizeof(char);
	SI_UNUSED(str);
}
inline siAllocator* si_stringAllocator(siString str) {
	return SI_STRING_HEADER(str)->allocator;
}
F_TRAITS(inline)
b32 si_stringEmpty(siString str) {
	return (str == nil || SI_STRING_HEADER(str)->len == 0);
}

F_TRAITS(inline)
char si_stringAt(siString str, usize index) {
	SI_ASSERT_NOT_NULL(str);
	if (index > si_stringLen(str) || si_stringLen(str) == 0) {
		return SI_ERROR;
	}

	return str[index];
}
F_TRAITS(inline)
char si_stringFront(siString str) {
	SI_ASSERT_NOT_NULL(str);
	if (si_stringLen(str) == 0) {
		return SI_OKAY;
	}

	return str[0];
}
F_TRAITS(inline)
char si_stringBack(siString str) {
	SI_ASSERT_NOT_NULL(str);
	if (si_stringLen(str) == 0) {
		return '\0';
	}

	return str[si_stringLen(str) - 1];
}
char* si_stringSub(siAllocator* alloc, siString str, usize begin, usize len) {
	char* res = si_mallocArray(alloc, char, len);
	memcpy(res, str + begin, len);
	*(res + len) = '\0';
	return res;
}

F_TRAITS(inline)
isize si_stringFind(siString str, cstring cstr) {
	return si_stringFindEx(str, 0, si_stringLen(str), cstr, si_cstrLen(cstr));
}
isize si_stringFindEx(siString str, usize start, usize end, cstring cstr, usize cstrLen) {
	SI_ASSERT_NOT_NULL(str);
	SI_ASSERT_NOT_NULL(cstr);

	usize i;
	usize curCount = 0;
	for (i = start; i < end; i++) {
		if (str[i] == cstr[curCount]) {
			curCount += 1;
			SI_STOPIF(curCount == cstrLen, return i - cstrLen + 1);
			continue;
		}
		curCount = 0;
	}

	return SI_ERROR;
}
F_TRAITS(inline)
isize si_stringFindStopAt(siString str, cstring cstr, char stopAt) {
	return si_stringFindStopAtEx(str, 0, si_stringLen(str) - 1, cstr, si_cstrLen(cstr), stopAt);
}
isize si_stringFindStopAtEx(siString str, usize start, usize end, cstring cstr,
		usize cstrLen, char stopAt) {
	SI_ASSERT_NOT_NULL(str);
	SI_ASSERT_NOT_NULL(cstr);

	usize i;
	usize curCount = 0;
	for (i = start; i < end; i++) {
		if (str[i] == cstr[curCount]) {
			curCount += 1;
			SI_STOPIF(curCount == cstrLen, return i - cstrLen + 1);
			continue;
		}
		else if (str[i] == stopAt) {
			break;
		}
		curCount = 0;
	}

	return SI_ERROR;
}
F_TRAITS(inline)
isize si_stringRFind(siString str, cstring cstr) {
	return si_stringRFindEx(str, si_stringLen(str) - 1, 0, cstr, si_cstrLen(cstr));
}
isize si_stringRFindEx(siString str, usize start, usize end, cstring cstr, usize
		cstrLen) {
	SI_ASSERT_NOT_NULL(str);

	usize i;
	isize curCount = cstrLen - 1;
	for (i = start; i < end - 1; i--) {
		if (str[i] == cstr[curCount]) {
			curCount -= 1;
			SI_STOPIF(curCount <= 0, return i);
			continue;
		}
		curCount = 0;
	}

	return SI_ERROR;
}
F_TRAITS(inline)
isize si_stringRFindStopAt(siString str, cstring cstr, char stopAt) {
	return si_stringRFindStopAtEx(str, 0, si_stringLen(str) - 1, cstr, si_cstrLen(cstr), stopAt);
}
isize si_stringRFindStopAtEx(siString str, usize start, usize end, cstring cstr,
		usize cstrLen, char stopAt) {
	SI_ASSERT_NOT_NULL(str);
	SI_ASSERT_NOT_NULL(cstr);

	usize i;
	isize curCount = cstrLen - 1;
	for (i = start; i < end - 1; i--) {
		if (str[i] == cstr[curCount]) {
			curCount -= 1;
			SI_STOPIF(curCount <= 0, return i);
			continue;
		}
		else if (str[i] == stopAt) {
			break;
		}
	}

	return SI_ERROR;
}

void si_stringJoin(siString* str, cstring cstr, cstring separator) {
	usize cstrLen = si_cstrLen(cstr);
	usize separatorLen = si_cstrLen(separator);

	siAllocator* alloc = si_allocatorMakeStack(SI_KILO(8));
	char* buf = si_mallocArray(alloc, char, cstrLen + separatorLen);

	memcpy(buf, separator, separatorLen);
	memcpy(buf + separatorLen, cstr, cstrLen);

	si_stringAppendLen(str, (char*)buf, separatorLen + cstrLen);
}
void si_stringSet(siString* str, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);
	siString curStr = *str;

	usize len = si_cstrLen(cstr);

	siStringHeader* header = SI_STRING_HEADER(curStr);

	if (header->capacity < len) {
		si_stringMakeSpaceFor(str, len - header->capacity);
		curStr = *str;
		header = SI_STRING_HEADER(curStr);
	}
	header->len = len;

	memcpy(curStr, cstr, len);
	curStr[len] = '\0';
}
F_TRAITS(inline)
void si_stringReplace(siString* str, cstring oldValue, cstring newValue) {
	si_stringReplaceEx(str, oldValue, si_cstrLen(oldValue), newValue, si_cstrLen(newValue));
}
void si_stringReplaceEx(siString* str, cstring oldValue, usize oldLen, cstring newValue,
		usize newLen) {
	isize index = 0;

	while (true) {
		siString curStr = *str;
		index = si_stringFindEx(curStr, index, si_stringLen(curStr), oldValue, oldLen);
		if (index == SI_ERROR) {
			break;
		}

		si_stringErase(curStr, index, oldLen);
		si_stringInsertEx(str, newValue, newLen, index, false);
	}
}
F_TRAITS(inline)
void si_stringTrim(siString str, cstring cutSet) {
	si_stringTrimLen(str, cutSet, strlen(cutSet));
}
void si_stringTrimLen(siString str, cstring cutSet, usize cutLen) {
	char* start = str;
	char* end  = str + si_stringLen(str) - 1;
	char* start_pos = start;
	char* end_pos = end;

	siStringHeader* header = SI_STRING_HEADER(str);
	header->len -= cutLen;

	while (start_pos <= end && strchr(cutSet, *start_pos)) {
		start_pos += 1;
	}
	while (end_pos > start_pos && strchr(cutSet, *end_pos)) {
		end_pos -= 1;
	}

	usize len = (start_pos > end_pos) ? 0 : ((end_pos - start_pos) + 1);

	if (str != start_pos) {
		memmove(str, start_pos, len);
	}
	str[len] = '\0';
}
void si_stringEnquote(siString* str) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += 2;

	if (header->capacity < header->len) {
		si_stringMakeSpaceFor(str, 2);
		header = SI_STRING_HEADER(*str);
	}
	siString curStr = *str;

	si_ptrMoveBy(curStr, header->len, 1, SI_DIRECTION_RIGHT);
	curStr[0] = '\"';
	curStr[header->len - 1] = '\"';
}

F_TRAITS(inline)
void si_stringAppend(siString* str, cstring other) {
	si_stringAppendLen(str, other, si_cstrLen(other));
}
void si_stringAppendLen(siString* str, cstring other, usize otherLen) {
	SI_ASSERT_NOT_NULL(str);
	siString curStr = *str;

	siStringHeader* header = SI_STRING_HEADER(curStr);
	usize previousLen = header->len;
	header->len += otherLen;

	if (header->capacity < header->len) {
		si_stringMakeSpaceFor(str, otherLen);
		curStr = *str;
		header = SI_STRING_HEADER(curStr);
	}

	memcpy(curStr + previousLen, other, otherLen);
	curStr[header->len] = '\0';
}
F_TRAITS(inline)
void si_stringPush(siString* str, char other) {
	si_stringAppendLen(str, &other, 1);
}
F_TRAITS(inline)
void si_stringPop(siString str) {
	SI_ASSERT_NOT_NULL(str);
	str[si_stringLen(str)] = '\0';
	SI_STRING_HEADER(str)->len -= 1;
}

F_TRAITS(inline)
void si_stringInsert(siString* str, cstring cstr, usize index) {
	si_stringInsertEx(str, cstr, si_cstrLen(cstr), index, false);
}
F_TRAITS(inline)
void si_stringInsertLen(siString* str, cstring cstr, usize cstrLen, usize index) {
	si_stringInsertEx(str, cstr, cstrLen, index, false);
}
void si_stringInsertEx(siString* str, cstring cstr, usize cstrLen, usize index, b32 eraseIndex) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize previousLen = header->len;
	usize beforeIndexLen = previousLen - index;

	SI_ASSERT_FMT(index < previousLen, "Index is higher than the length of the string (strLen - '%zd', index - '%zd')", previousLen, index);
	header->len += cstrLen - eraseIndex;

	if (header->capacity < header->len) {
		si_stringMakeSpaceFor(str, header->len - header->capacity);
		header = SI_STRING_HEADER(*str);
	}
	siString curStr = *str;

	char* ptr = (char*)memcpy(curStr + header->len - beforeIndexLen, curStr + index, beforeIndexLen);
	memcpy(curStr + index, cstr, cstrLen);
	ptr[beforeIndexLen] = '\0';
}
void si_stringErase(siString str, usize index, usize eraseLen) {
	SI_ASSERT_NOT_NULL(str);

	usize strLen = si_stringLen(str);
	SI_ASSERT_FMT(index < strLen, "Index is higher than the length of the string. (strLen - '%zd', index - '%zd')", strLen, index);

	usize afterIndexLen = index + eraseLen;

	char* ptr = (char*)memcpy(
		str + index,
		str + afterIndexLen,
		strLen - afterIndexLen
	);
	ptr[strLen - afterIndexLen] = '\0';

	SI_STRING_HEADER(str)->len -= eraseLen;
}
void si_stringRemoveCstr(siString str, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);

	siStringHeader* header = SI_STRING_HEADER(str);
	usize cstrLen = si_cstrLen(cstr);
	while (true) {
		isize index = si_stringRFindEx(str, header->len - 1, 0, cstr, cstrLen);
		SI_STOPIF(index == SI_ERROR, break);

		usize afterIndexLen = index + cstrLen;

		char* ptr = (char*)memcpy(
			str + index,
			str + afterIndexLen,
			header->len - afterIndexLen
		);
		ptr[header->len - afterIndexLen] = '\0';

		header->len -= cstrLen;
	}
}
void si_string_swap(siString* str, cstring cstr1, cstring cstr2);

void si_stringStrip(siString str) {
	SI_ASSERT_NOT_NULL(str);

	usize start = 0, end = 0;
	isize i;

	for_range (i, 0, si_stringLen(str)) {
		SI_STOPIF(!si_charIsSpace(str[i]), break);
		start += 1;
	}

	for (i = si_stringLen(str) - 1; i != -1; i--) {
		SI_STOPIF(!si_charIsSpace(str[i]), break);
		end += 1;
	}
	char* strippedStr = &str[start];
	usize newLen = si_stringLen(str) - start - end;
	SI_STOPIF(newLen == si_stringLen(str), return);

	memcpy(str, strippedStr, newLen);
	strippedStr[newLen] = '\0';

	SI_STRING_HEADER(strippedStr)->len = newLen;
}
F_TRAITS(inline)
void si_stringReverse(siString str) {
	si_stringReverseLen(str, si_stringLen(str));
}
void si_stringReverseLen(siString str, usize len) {
	SI_ASSERT_NOT_NULL(str);

	char* a = str;
	char* b = str + len - 1;
	len /= 2;

	while (len--) {
		si_swap(*a, *b);
		a += 1;
		b -= 1;
	}
}

siArray(siString) si_stringSplit(siAllocator* alloc, siString str, cstring delimiter) {
	SI_ASSERT_NOT_NULL(str);

	usize strLen = si_stringLen(str);
	usize separatorLen = si_cstrLen(delimiter);

	usize count = 0;
	usize beginPos = 0;
	usize posBuffer[SI_KILO(1)];

	while (true) {
		isize pos = si_stringFindEx(str, beginPos, strLen, delimiter, separatorLen);
		if (pos == SI_ERROR) {
			posBuffer[count] = posBuffer[count - 1];
			count++;
			break;
		}
		posBuffer[count] = pos - beginPos;
		count++;

		beginPos = pos + separatorLen;
	}
	siArray(siString) res = (siString*)si_arrayMakeReserve(alloc, sizeof(*res), count);
	SI_ARRAY_HEADER(res)->len = count;

	for_range (i, 0, count) {
		res[i] = si_stringMakeLen(alloc, str, posBuffer[i]);
		str += posBuffer[i] + separatorLen;
	}

	return res;
}
F_TRAITS(inline)
void si_stringClear(siString str) {
	SI_ASSERT_NOT_NULL(str);
	*str = '\0';
	SI_STRING_HEADER(str)->len = 0;
}

F_TRAITS(inline)
void si_stringFree(siString str) {
	SI_ASSERT_NOT_NULL(str);
	si_free(SI_STRING_HEADER(str)->allocator, SI_STRING_HEADER(str));
}
void si_stringMakeSpaceFor(siString* str, usize add_len) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize oldSize = sizeof(siStringHeader) + header->len + 1;
	usize newSize = oldSize + add_len;

	siStringHeader* newHeader = (siStringHeader*)si_realloc(
		header->allocator,
		header,
		oldSize,
		newSize
	);

	*str = (char*)newHeader + sizeof(siStringHeader);
	newHeader->capacity += add_len;
}
void si_stringShrinkToFit(siString* str) {
	SI_ASSERT_NOT_NULL(str);
	siString curStr = *str;
	siStringHeader* header = SI_STRING_HEADER(curStr);

	header = (siStringHeader*)si_realloc(
		header->allocator,
		header,
		sizeof(siStringHeader*) + header->capacity + 1,
		sizeof(siStringHeader*) + header->len + 1
	);
	header->capacity = header->len;
}

inline char* si_cstrMake(siAllocator* alloc, cstring cstr) {
	return si_cstrMakeLen(alloc, cstr, si_cstrLen(cstr));
}
inline char* si_cstrMakeLen(siAllocator* alloc, cstring cstr, usize len) {
	char* str = si_mallocArray(alloc, char, len + 1);
	memcpy(str, cstr, len + 1);

	return str;
}
char* si_cstrMakeFmt(siAllocator* alloc, cstring cstr, ...) {
	SI_ASSERT_NOT_NULL(cstr);
	char buffer[SI_KILO(4)];

	va_list va;
	va_start(va, cstr);
	usize len = si_snprintfVa(buffer, sizeof(buffer), cstr, va);
	va_end(va);

	char* res = si_mallocArray(alloc, char, len + 1);
	res[len] = '\0';
	return memcpy(res, buffer, len);
}

F_TRAITS(inline)
usize si_cstrLen(cstring str) {
	return strlen(str); /* NOTE(EimaMei): Generally for performance, OG strlen is best for both portability and performance. */
}

F_TRAITS(inline)
void si_cstrUpper(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_charUpper(x);
		str++;
	}
}
F_TRAITS(inline)
void si_cstrLower(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_charLower(x);
		str += 1;
	}
}
void si_cstrTitle(char* str) {
	SI_ASSERT_NOT_NULL(str);

	b32 change = true;
	char x = '\0';
	while ((x = *str)) {
		if (si_charIsSpace(x)) {
			change = true;
		}
		else if (change) {
			*str = si_charUpper(x);
			change = false;
		}
		str += 1;
	}
}
F_TRAITS(inline)
void si_cstrCapitalize(char* str) {
	SI_ASSERT_NOT_NULL(str);

	si_cstrLower(str);
	*str = si_charUpper(*str);
}

F_TRAITS(inline)
b32 si_cstrEqual(cstring str1, cstring str2) {
	return strcmp(str1, str2) == 0;
}
b32 si_cstrEqualLen(cstring str1, usize str1Len, cstring str2, usize str2Len) {
	SI_ASSERT_NOT_NULL(str1);
	SI_ASSERT_NOT_NULL(str2);

	SI_STOPIF(str1 == str2, return true);
	SI_STOPIF(str1Len != str2Len, return false);

	usize i;
	for (i = 0; i < str1Len; i++) {
		SI_STOPIF(str1[i] != str2[i], return false);
	}

	return true;
}

static const char SI_NUM_TO_CHAR_TABLE[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"@$";

F_TRAITS(inline)
char* si_u64ToCstr(siAllocator* alloc, u64 num) {
	return si_u64ToCstrEx(alloc, num, 10, nil);
}
char* si_u64ToCstrEx(siAllocator* alloc, u64 num, i32 base, usize* outLen) {
	SI_ASSERT_NOT_NULL(alloc);

	usize len = si_numLenEx(num, base);
	char* res = si_mallocArray(alloc, char, len + 1);

	char* endPtr = res + len;
	*endPtr = '\0';

	do {
		*(--endPtr) = SI_NUM_TO_CHAR_TABLE[num % base]; /* NOTE(EimaMei): We build the string from behind (not the start) so that we
														   wouldn't have to reverse the string after we make the string. */
		num /= base;
	} while (num > 0);

	SI_STOPIF(outLen != nil, *outLen = len);
	return res;
}
u64 si_cstrToU64(cstring str) {
	SI_ASSERT_NOT_NULL(str);

	u64 res = 0;
	char cur;
	while ((cur = *str++)) {
		if (si_between(cur, '0', '9')) {
			res = (res * 10) + (cur - '0');
		}
		else {
			SI_ASSERT_MSG(si_between(cur, '0', '9'), "Attempted to use `si_cstrToU64` with a string that contains non numbers.");
		}
	}

	return res;
}
u64 si_cstrToU64Len(cstring str, usize len) {
	SI_ASSERT_NOT_NULL(str);
/*TODO(EimaMei): Update this to include other bases */
	u64 res = 0;
	while (len != 0) {
		if (si_between(*str, '0', '9')) {
			res = (res * 10) + (*str - '0');
		}
		else {
			SI_ASSERT_MSG(si_between(*str, '0', '9'), "Attempted to use `si_cstrToU64` with a string that contains non numbers.");
		}
		len -= 1;
		str += 1;
	}

	return res;
}
F_TRAITS(inline)
char* si_i64ToCstr(siAllocator* alloc, i64 num) {
	return si_i64ToCstrEx(alloc, num, 10, nil);
}
char* si_i64ToCstrEx(siAllocator* alloc, i64 num, i32 base, usize* outLen) {
	SI_ASSERT_NOT_NULL(alloc);

	usize len = si_numLenI64Ex(num, base);
	char* res = si_mallocArray(alloc, char, len + 1);

	char* endPtr = res + len;
	*endPtr = '\0';

	b32 isNegative = si_numIsNeg(num);
	u64 unsignedNum = (num ^ -isNegative) + isNegative;

	do {
		*(--endPtr) = SI_NUM_TO_CHAR_TABLE[unsignedNum % base];
		unsignedNum /= base;
	} while (unsignedNum > 0);

	SI_STOPIF(isNegative, *(--endPtr) = '-');
	SI_STOPIF(outLen != nil, *outLen = len);
	return res;
}
char* si_f64ToCstr(siAllocator* alloc, f64 num) {
	return si_f64ToCstrEx(alloc, num, 10, 6, nil);
}
char* si_f64ToCstrEx(siAllocator* alloc, f64 num, i32 base, u32 afterPoint,
		usize* outLen) {
	SI_ASSERT_NOT_NULL(alloc);

	b32 isNegative = num < 0;
	num = si_fabs(num);
	usize len = isNegative + si_numLenEx((i64)num, base) + 1 * (afterPoint != 0) + afterPoint;

	char* res = si_mallocArray(alloc, char, len + 1);
	char* endPtr = res + len;
	*endPtr = '\0';

	f64 baseForm = num;
	u64 pow10val = si_pow10(afterPoint);
	u64 remainder = (u64)((baseForm - (u64)baseForm) * pow10val + 0.5);

	usize count = 0;
	while (count < afterPoint) {
		*(--endPtr) = SI_NUM_TO_CHAR_TABLE[remainder % base];
		remainder /= base;
		count += 1;
	}
	SI_STOPIF(count != 0, *(--endPtr) = '.');

	do {
		*(--endPtr) = SI_NUM_TO_CHAR_TABLE[(u64)baseForm % base];
		baseForm /= base;
	} while ((u64)baseForm > 0);


	SI_STOPIF(isNegative, *(--endPtr) = '-');
	SI_STOPIF(outLen != nil, *outLen = len);
	return res;
}

i64 si_cstrToI64(cstring str) {
	SI_ASSERT_NOT_NULL(str);

	i64 res = 0;
	char cur;
	b32 negative = false;

	if ((*str) == '-') {
		negative = true;
		str++;
	}

	while ((cur = *str++)) {
		if (cur >= '0' && cur <= '9') {
			res = (res * 10) + (cur - '0');
		}
		else {
			SI_ASSERT_MSG(!(cur >= '0' && cur <= '9'), "Attempted to use `si_cstrToI64` with a string that contains non numbers.");
		}
	}

	if (negative) {
		res = -res;
	}

	return res;
}
u64 si_cstrToI64Len(cstring str, usize len) {
	SI_ASSERT_NOT_NULL(str);

	i64 res = 0;
	b32 negative = false;

	if ((*str) == '-') {
		negative = true;
		str += 1;
		len -= 1;
	}

	while (len != 0) {
		if (*str >= '0' && *str <= '9') {
			res = (res * 10) + (*str - '0');
		}
		else {
			SI_ASSERT_MSG(!(*str >= '0' && *str <= '9'), "Attempted to use `si_cstrToI64` with a string that contains non numbers.");
		}
		len -= 1;
		str += 1;
	}

	if (negative) {
		res = -res;
	}

	return res;
}

#endif

#if defined(SI_IMPLEMENTATION_UNICODE) && !defined(SI_NO_UNICODE)

siUtf32Char si_utf8Decode(const char character[4]) {
	#define FAILURE 12

	u32 state = 0, codepoint;
	u8* ogPtr = (u8*)character;

	do {
		/* Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de> */
		/* See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details. */
		static const u8 utf8d[] = {
			/* The first part of the table maps bytes to character classes that */
			/* to reduce the size of the transition table and create bitmasks. */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
			7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
			8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
			/* The second part is a transition table that maps a combination */
			/* of a state of the automaton and a character class to a state. */
			0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
			12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
			12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
			12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
			12,36,12,12,12,12,12,12,12,12,12,12,
		 };

		u8 byte = *character;
		u32 type = utf8d[byte];

		codepoint = (state != 0)
			? ((byte & 0x3F) | (codepoint << 6))
			: ((0xFF >> type) & byte);
		state = utf8d[256 + state + type];

		character += 1;
	} while (state != 0 && state != FAILURE);

	switch (state) {
		case 0: return (siUtf32Char){codepoint, character - (cstring)ogPtr};
		default: return (siUtf32Char){SI_UTF8_INVALID, 0};
	}
	#undef FAILURE
}
siUtf8Char si_utf8Encode(i32 codepoint) {
	siUtf8Char result;

	u32 len = (codepoint >= 0x80) + (codepoint >= 0x800) + (codepoint >= 0x10000);
	result.codepoint[0] = ((len != 0) * ((0xFF80 >> len) & 0xFF)) | (codepoint >> (6 * len));
	result.len = len + 1;

	usize i;
	for (i = 1; i < result.len; i += 1) {
		result.codepoint[i] = 0x80 | ((codepoint >> (6 * (len - i))) & 0x3F);
	}

	return result;
}

F_TRAITS(inline)
usize si_utf8StrLen(siUtf8String str) {
	SI_ASSERT_NOT_NULL(str);

	usize offset = 0;
	usize len = 0;
	while (str[offset] != '\0') {
		offset += si_utf8Decode(&str[offset]).len;
		len += 1;
	}

	return len;
}

F_TRAITS(inline)
siUtf32Char si_utf8StrAt(siUtf8String str, usize charIndex) {
	SI_ASSERT_NOT_NULL(str);

	siUtf32Char character = si_utf8Decode(str);
	usize offset = character.len;

	while (charIndex) {
		character = si_utf8Decode(&str[offset]);
		offset = character.len;
		charIndex -= 1;
	}

	return character;
}

F_TRAITS(inline)
siUtf16String si_utf8ToUtf16Str(siAllocator* alloc, siUtf8String str, usize* strLenOut) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(str);

	siUtf16String res = (siUtf16String)si_allocatorCurPtr(alloc);
	usize len = 0;
	siUtf8String utf8Ptr = str;

	while (true) {
		siUtf32Char utf32 = si_utf8Decode(utf8Ptr);
		i32 codepoint = utf32.codepoint;
		SI_ASSERT_MSG(codepoint != SI_UTF8_INVALID, "Invalid UTF-8 character.");

		SI_STOPIF(codepoint == '\0', break);
		utf8Ptr += utf32.len;

		if (codepoint > 0xFFFF) {
			u32* curPos = si_mallocItem(alloc, u32);

			u32 t = codepoint - 0x10000;
			u32 high = (t << 10) + 0xD800;
			u32 low = t + 0xDC00;

			*curPos = (high << 16) | (low & 0x0000FFFF);
			len += 4;
		}
		else {
			u16* curPos = si_mallocItem(alloc, u16);
			*curPos = codepoint;
			len += 2;
		}
	}
	u16* curPos = si_mallocItem(alloc, u16);
	*curPos = '\0';
	len += 2;

	SI_STOPIF(strLenOut != nil, *strLenOut = len);

	return res;
}
siUtf8String si_utf16ToUtf8Str(siAllocator* alloc, siUtf16String str, usize* strLenOut) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(str);

	siUtf8String string = (siUtf8String)si_allocatorCurPtr(alloc);
	usize len = 0;

	while (true) {
		u32 chr = *str;
		str += 1;
		SI_STOPIF(chr == '\0', break);

		if (chr <= 0xFF) {
			char* res = si_mallocArray(alloc, char, 1);
			*res = chr;
			len += 1;
		}
		else if (chr <= 0x7FF) {
			char* res = si_mallocArray(alloc, char, 2);
			res[0] = 0xC0 | (chr >> 6);            /* 110xxxxx */
			res[1] = 0x80 | (chr & 0x3F);          /* 10xxxxxx */
			len += 2;
		}
		else if (chr <= (0xD800 - 1)) {
			char* res = si_mallocArray(alloc, char, 3);
			res[0] = 0xE0 | (chr >> 12);           /* 1110xxxx */
			res[1] = 0x80 | ((chr >> 6) & 0x3F);   /* 10xxxxxx */
			res[2] = 0x80 | (chr & 0x3F);          /* 10xxxxxx */
			len += 3;
		}
		else if (chr >= 0xD800) {
			u16 high = chr;
			u16 low = *str;
			str += 1;

			u32 tHigh = (high - 0xD800) << 10;
			u32 tLow = (low - 0xDC00);
			u32 codepoint = (tHigh | tLow) + 0x10000;

			char* res = si_mallocArray(alloc, char, 4);
			res[0] = 0xF0 | (codepoint >> 18);           /* 11110xxx */
			res[1] = 0x80 | ((codepoint >> 12) & 0x3F);  /* 10xxxxxx */
			res[2] = 0x80 | ((codepoint >> 6) & 0x3F);   /* 10xxxxxx */
			res[3] = 0x80 | (codepoint & 0x3F);          /* 10xxxxxx */

			len += 4;
		}
	}
	si_allocatorPush(alloc, '\0');
	SI_STOPIF(strLenOut != nil, *strLenOut = len);

	return string;
}
#endif

#if defined(SI_IMPLEMENTATION_CHAR) && !defined(SI_NO_CHAR)

F_TRAITS(inline)
char si_charLower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c + 32;
	}
	return c;
}

F_TRAITS(inline)
char si_charUpper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 32;
	}
	return c;
}

F_TRAITS(inline)
b32 si_charIsSpace(char c) {
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
F_TRAITS(inline)
b32 si_charIsDigit(char c) {
	return (c >= '0' && c <= '9');
}
F_TRAITS(inline)
b32 si_charIsHexDigit(char c) {
	return (si_charIsDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}
F_TRAITS(inline)
b32  si_charIsAlpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
F_TRAITS(inline)
b32 si_charIsAlphanumeric(char c) {
	return si_charIsAlpha(c) || si_charIsDigit(c);
}
F_TRAITS(inline)
i32 si_charDigitToInt(char c) {
	SI_ASSERT_MSG(si_charIsDigit(c), "Character is not a digit.");
	return (c - '0');
}
i32 si_charHexDigitToInt(char c) {
	if (si_charIsDigit(c)) {
		return si_charDigitToInt(c);
	}
	else if (c <= 'a' && c >= 'f') {
		return c - 'a' + 10;
	}
	else if (c <= 'A' && c >= 'F') {
		return c - 'A' + 10;
	}

	return -1;
}
#endif

#if defined(SI_IMPLEMENTATION_HASHTABLE) && !defined(SI_NO_HASHTABLE)

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

F_TRAITS(intern)
u64 si_hashKey(cstring key) {
	u64 hash = FNV_OFFSET;
	cstring p;
	for (p = key; *p; p++) {
		hash ^= (u64)(*p);
		hash *= FNV_PRIME;
	}
	return hash;
}

#undef FNV_OFFSET
#undef FNV_PRIME

F_TRAITS(inline)
siHashTable si_hashtableMake(siAllocator* alloc, cstring* keyArray, rawptr dataArray,
		usize sizeofElement, usize len) {
	siHashTable table;
	table.alloc = alloc;
	table.entries = si_arrayMakeReserve(alloc, sizeof(siHashEntry), len);

	siArrayHeader* arrayHeader = SI_ARRAY_HEADER(table.entries);
	arrayHeader->len = len;

	siAny any = (siAny){.ptr = dataArray, .typeSize = sizeofElement};
	for_range (i, 0, len) {
		si_hashtableSetEx(&table, keyArray[i], any);
		any.ptr = (siByte*)any.ptr + sizeofElement;
	}

	return table;
}
F_TRAITS(inline)
siHashTable si_hashtableMakeReserve(siAllocator* alloc, usize capacity) {
	siHashTable table;
	table.alloc = alloc;
	table.entries = si_arrayMakeReserve(alloc, sizeof(siHashEntry), capacity);
	memset(table.entries, 0, capacity * sizeof(siHashEntry));

	return table;
}
rawptr si_hashtableGet(siHashTable ht, cstring key) {
	SI_STOPIF(si_arrayLen(ht.entries) == 0, return nil);

	u64 hash = si_hashKey(key);
	usize index = (usize)(hash % (si_arrayCapacity(ht.entries) - 1));

	siHashEntry* entry = ht.entries + index;
	siHashEntry* old_entry = ht.entries + index;
	siHashEntry* end = ht.entries + si_arrayCapacity(ht.entries);
	do {
		SI_STOPIF(entry->key == nil, goto increment_entry);

		if (*key == *entry->key && strcmp(key + 1, entry->key + 1) == 0) {
			return entry->value;
		}

increment_entry:
		entry += 1;
		if (entry == end) {
			entry = ht.entries;
		}
	} while (entry != old_entry);

	return nil;
}
siHashEntry* si_hashtableSetEx(siHashTable* ht, cstring key, siAny value) {
	SI_ASSERT_NOT_NULL(ht);
	siArrayHeader* arrHeader = SI_ARRAY_HEADER(ht->entries);

	SI_ASSERT_NOT_NULL(key);
	SI_ASSERT_MSG(arrHeader->len < arrHeader->capacity, "Not enough capacity in the hash table.");

	u64 hash = si_hashKey(key);
	usize index = hash % (arrHeader->capacity - 1);

	siHashEntry* entry = ht->entries + index;
	siHashEntry* end = ht->entries + arrHeader->capacity;

	while (entry->key != nil) {
		if (*key == *entry->key && strcmp(key + 1, entry->key + 1) == 0) {
			return ht->entries + index;
		}

		entry += 1;
		SI_STOPIF(entry == end, entry = ht->entries);
	}
	entry->key = si_cstrMake(ht->alloc, key);
	entry->value = si_malloc(ht->alloc, value.typeSize);
	memcpy(entry->value, value.ptr, value.typeSize);

	arrHeader->len += 1;
	return ht->entries + index;
}
void si_hashtableFree(siHashTable ht)  {
	usize capacity = si_arrayCapacity(ht.entries);
	usize i;
	for (i = 0; i < capacity; i++) {
		if (ht.entries[i].key != nil) {
			si_free(ht.alloc, (rawptr)ht.entries[i].key);
			si_free(ht.alloc, ht.entries[i].value);
		}
	}
	si_free(ht.alloc, ht.entries);
}
#endif


#if defined(SI_IMPLEMENTATION_IO) && !defined(SI_NO_IO)

siErrorInfo SI_FS_ERROR = {0};
i32 SI_FS_READ_BYTES = 0;

F_TRAITS(inline)
cstring si_pathFsErrorName(siFileSystemError err) {
	switch (err) {
		case SI_FS_ERROR_NONE: return "SI_FS_ERROR_NONE";
		case SI_FS_ERROR_EXISTS: return "SI_FS_ERROR_EXISTS";
		case SI_FS_ERROR_INVALID: return "SI_FS_ERROR_INVALID";
		case SI_FS_ERROR_INVALID_FILENAME: return "SI_FS_ERROR_INVALID_FILENAME";
		case SI_FS_ERROR_DOESNT_EXIST: return "SI_FS_ERROR_DOESNT_EXIST";
		case SI_FS_ERROR_PERMISSION: return "SI_FS_ERROR_PERMISSION";
		case SI_FS_ERROR_TRUNCATE_FAILURE: return "SI_FS_ERROR_TRUNCATE_FAILURE";
	}
	return nil;
}

F_TRAITS(inline)
cstring si_pathFsErrorStr(siFileSystemError err) {
	switch (err) {
		case SI_FS_ERROR_NONE: return "No error has been encountered.";
		case SI_FS_ERROR_EXISTS: return "File or directory already exists.";
		case SI_FS_ERROR_INVALID: return "Provided value is invalid.";
		case SI_FS_ERROR_INVALID_FILENAME: return "Invalid filename.";
		case SI_FS_ERROR_DOESNT_EXIST: return "File or directory doesn't exist.";
		case SI_FS_ERROR_PERMISSION: return "User doesn't have permission to read/write.";
		case SI_FS_ERROR_TRUNCATE_FAILURE: return "Failed to truncate the file.";
	}
	return nil;
}
#if !defined(SI_NO_ERROR_STRUCT)
	#define SI_FS_ERROR_DECLARE_EX(err) \
		{ \
			SI_FS_ERROR.code = err; \
			SI_FS_ERROR.function = __func__; \
			SI_FS_ERROR.time = si_timeNowUTC(); \
			SI_FS_ERROR_LOG(); \
		}
	#define SI_FS_ERROR_DECLARE() \
		SI_FS_ERROR_DECLARE_EX(si_intern_fileGetOSError())

	#if !defined(SI_NO_FS_ERROR_PRINTS)
		#define SI_FS_ERROR_LOG() \
			si_fprintf( \
				SI_STDERR, \
				"%CRFile system error at \"%s\"%C: %s: %s (err number '%i')\n", \
				SI_FS_ERROR.function, si_pathFsErrorName(SI_FS_ERROR.code), \
				si_pathFsErrorStr(SI_FS_ERROR.code), SI_FS_ERROR.code \
			)
	#else
		#define SI_FS_ERROR_LOG() do {} while (0)
	#endif
#else
	#define SI_FS_ERROR_DECLARE_EX(err) \
		{ \
			SI_FS_ERROR.code = err; \
			SI_FS_ERROR_LOG(); \
		}
	#define SI_FS_ERROR_DECLARE() \
		SI_FS_ERROR_DECLARE_EX(si_intern_fileGetOSError())

	#if !defined(SI_NO_FS_ERROR_PRINTS)
		#define SI_FS_ERROR_LOG() \
			si_fprintf( \
				SI_STDERR, \
				"%CRFile system error%C: %s: %s (errno '%i')\n", \
				si_pathFsErrorName(SI_FS_ERROR.code), si_pathFsErrorStr(SI_FS_ERROR.code), \
				SI_FS_ERROR \
			)
	#else
		#define SI_FS_ERROR_LOG() do {} while (0)
	#endif
#endif
F_TRAITS(inline intern)
siFileSystemError si_intern_fileGetOSError(void) {
#if defined(SI_SYSTEM_WINDOWS)
	switch (GetLastError()) {
		case ERROR_SUCCESS: return SI_FS_ERROR_NONE;
		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS: return SI_FS_ERROR_EXISTS;
		case ERROR_INVALID_NAME:
		case ERROR_BAD_PATHNAME: return SI_FS_ERROR_INVALID_FILENAME;
		case ERROR_DIRECTORY:
		case ERROR_INVALID_ADDRESS: return SI_FS_ERROR_INVALID;
		case ERROR_PATH_NOT_FOUND:
		case ERROR_FILE_NOT_FOUND: return SI_FS_ERROR_DOESNT_EXIST;
		case ERROR_ACCESS_DENIED: return SI_FS_ERROR_DOESNT_EXIST;
	}
	return SI_FS_ERROR_INVALID;
#else
	switch (errno) {
		case EEXIST: return SI_FS_ERROR_EXISTS;
		case EINVAL: return SI_FS_ERROR_INVALID;
		case EISDIR:
		case ENAMETOOLONG: return SI_FS_ERROR_INVALID_FILENAME;
		case ENOENT: return SI_FS_ERROR_DOESNT_EXIST;
		case EPERM:
		case EACCES: return SI_FS_ERROR_PERMISSION;
	}

	return SI_FS_ERROR_NONE;
#endif
}

F_TRAITS(inline)
b32 si_pathExists(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);
		DWORD file_attrib = GetFileAttributesW(widePath);
		return file_attrib != INVALID_FILE_ATTRIBUTES;
	#else
		struct stat tmp;
		i32 res = stat(path, &tmp);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });

		return true;
	#endif
}
isize si_pathCopy(cstring existingPath, cstring newPath) {
	SI_ASSERT_NOT_NULL(existingPath);
	SI_ASSERT_NOT_NULL(newPath);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, nil);
		siUtf16String wideNew = si_utf8ToUtf16Str(stack, newPath, nil);

		return (CopyFileW(wideExisting, wideNew, true) != 0);
	#else
		i32 existingFile = open(existingPath, O_RDONLY, 0);
		SI_STOPIF(existingFile != 0, { SI_FS_ERROR_DECLARE(); return false; });

		i32 newFile = open(newPath, O_WRONLY | O_CREAT, 0666);
		SI_STOPIF(newFile != 0, { SI_FS_ERROR_DECLARE(); return false; });

		struct stat statExisting;
		i32 res = fstat(existingFile, &statExisting);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });

		#if defined(SI_SYSTEM_UNIX)
			isize size = sendfile64(newFile, existingFile, 0, statExisting.st_size);
		#else
			isize size = sendfile(newFile, existingFile, 0, &statExisting.st_size, NULL, 0);
		#endif
		SI_STOPIF(size != 0, { SI_FS_ERROR_DECLARE(); return false; });

		close(newFile);
		close(existingFile);

		return size;
	#endif
}
u32 si_pathItemsCopy(cstring pathSrc, cstring pathDst) {
	SI_ASSERT_NOT_NULL(pathSrc);
	SI_ASSERT_NOT_NULL(pathDst);

	siAllocator* stack = si_allocatorMakeStack(SI_KILO(2));
	siDirectory dir = si_dirOpen(stack, pathSrc);
	u32 itemsCopied = 0;

	char* outPath;
	char* outPathEnd;
	{
		usize pathDstLen = si_cstrLen(pathDst);
		outPath = si_malloc(stack, pathDstLen + 1);

		memcpy(outPath, pathDst, pathDstLen);
		outPath[pathDstLen] = SI_PATH_SEPARATOR;

		outPathEnd = outPath + pathDstLen + 1;
	}

	siDirectoryEntry entry;
	while (si_dirPollEntryEx(dir, &entry, true)) {
		cstring filename = si_pathBaseNameLen(entry.path, entry.len);
		usize filenameLen = entry.len - (filename - entry.path);
		memcpy(outPathEnd, filename, filenameLen);

		if (entry.type == SI_IO_TYPE_DIR) {
			si_pathCreateFolder(outPath);
			itemsCopied += si_pathItemsCopy(entry.path, outPath);
			continue;
		}

		si_pathCopy(entry.path, outPath);
		itemsCopied += 1;
	}
	si_dirClose(dir);

	return itemsCopied;
}
F_TRAITS(inline)
b32 si_pathMove(cstring existingPath, cstring newPath) {
	SI_ASSERT_NOT_NULL(existingPath);
	SI_ASSERT_NOT_NULL(newPath);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, nil);
		siUtf16String wideNew = si_utf8ToUtf16Str(stack, newPath, nil);

		return (MoveFileW(wideExisting, wideNew) != 0);
	#else
		i32 res = link(existingPath, newPath);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		res = unlink(existingPath);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });

		return true;
	#endif
}
F_TRAITS(inline)
b32 si_pathRename(cstring oldName, cstring newName) {
	return si_pathMove(oldName, newName);
}
F_TRAITS(inline)
b32 si_pathCreateFolder(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMakeStack(MAX_PATH);
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);

		b32 res = CreateDirectoryW(widePath, nil);
		return res;
	#else
		i32 res = mkdir(path, SI_FS_PERM_ALL);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif
}
F_TRAITS(inline)
b32 si_pathCreateFolderEx(cstring path, siFilePermissions perms) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		return si_pathCreateFolder(path);
		SI_UNUSED(perms);
	#else
		i32 res = mkdir(path, perms);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif
}
F_TRAITS(inline)
b32 si_pathRemove(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		usize len;
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, &len);

		b32 isDir = si_stringRFindStopAtEx(
			(siString)path,
			len / 2, 0,
			".", 1,
			SI_PATH_SEPARATOR
		) == SI_ERROR;
		static WINBOOL SI_FUNC_PTR(funcs[], (LPCWSTR)) = {DeleteFileW, RemoveDirectoryW};

		return funcs[isDir](widePath);
	#else
		i32 res = remove(path);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif
}
F_TRAITS(inline)
b32 si_pathCreateHardLink(cstring existingPath, cstring linkPath) {
	SI_ASSERT_NOT_NULL(existingPath);
	SI_ASSERT_NOT_NULL(linkPath);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMake(SI_KILO(2));
		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, nil);
		siUtf16String wideLink = si_utf8ToUtf16Str(stack, linkPath, nil);

		return CreateHardLinkW(wideLink, wideExisting, nil) != 0;
	#else
		i32 res = link(existingPath, linkPath);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif

}
F_TRAITS(inline)
b32 si_pathCreateSoftLink(cstring existingPath, cstring linkPath) {
	SI_ASSERT_NOT_NULL(existingPath);
	SI_ASSERT_NOT_NULL(linkPath);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMake(SI_KILO(2));
		usize existingLen;

		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, &existingLen);
		siUtf16String wideLink = si_utf8ToUtf16Str(stack, linkPath, nil);

		b32 isDir = si_stringRFindStopAtEx(
			(siString)existingPath,
			existingLen / 2, 0,
			".", 1,
			SI_PATH_SEPARATOR
		) == SI_ERROR;

		return CreateSymbolicLinkW(wideLink, wideExisting, isDir) != 0;
	#else
		i32 res = symlink(existingPath, linkPath);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif

}
F_TRAITS(inline)
b32 si_pathEditPermissions(cstring path, siFilePermissions newPerms) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		/* TODO(EimaMei): Fix this. */
		SI_UNUSED(newPerms);
		return true;
	#else
		i32 res = chmod(path, newPerms);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif
}

F_TRAITS(inline)
cstring si_pathBaseName(cstring path) {
	return si_pathBaseNameLen(path, si_cstrLen(path));

}
F_TRAITS(inline)
cstring si_pathBaseNameLen(cstring path, usize len) {
	SI_ASSERT_NOT_NULL(path);

	isize index = len - 1;
	while (index >= 0) {
		i32 value = path[index];

		if (value == SI_PATH_SEPARATOR || value == SI_PATH_SEPARATOR_REVERSE) {
			break;
		}

		index -= 1;
	}
	return path + index + 1;
}
F_TRAITS(inline)
cstring si_pathUnrooted(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		usize offset = 3 * si_pathIsAbsolute(path);
	#else
		usize offset = si_pathIsAbsolute(path);
	#endif
	usize ogOffset = offset;

	while (path[offset] != '\0') {
		if (path[offset] == SI_PATH_SEPARATOR || path[offset] == SI_PATH_SEPARATOR_REVERSE) {
			return path + offset + 1;
		}
		offset += 1;
	}

	return path + ogOffset;
}
F_TRAITS(inline)
cstring si_pathExtension(cstring path) {
	isize pos = si_stringRFindStopAtEx(
		(siString)path,
		si_cstrLen(path) - 1, 0,
		".", 1,
		SI_PATH_SEPARATOR
	);
	return si_cast(cstring, ((usize)path + pos + 1) * (pos != SI_ERROR));
}
F_TRAITS(inline)
siString si_pathGetFullName(siAllocator* alloc, cstring path) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		siAllocator* stack = si_allocatorMake(SI_KILO(1));
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);
		siUtf16String buf = si_mallocArray(stack, u16, MAX_PATH);
		GetFullPathNameW(widePath, MAX_PATH, buf, nil);

		usize len;
		cstring utf8Res = si_utf16ToUtf8Str(stack, buf, &len);

		return si_stringMakeLen(alloc, utf8Res, len);
	#else
		char actualPath[4096];
		realpath(path, actualPath);
		SI_STOPIF(path == nil, { SI_FS_ERROR_DECLARE(); return nil; });

		return si_stringMake(alloc, actualPath);
	#endif
}

F_TRAITS(inline)
b32 si_pathIsAbsolute(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		return (
			si_charIsAlpha(path[0])
			&& path[1] == ':'
			&& path[2] == SI_PATH_SEPARATOR
			&& path[3] != '\0'
		);
	#else
		return (path[0] == SI_PATH_SEPARATOR && path[1] != '\0');
	#endif
}
F_TRAITS(inline)
b32 si_pathIsRelative(cstring path) {
	return !si_pathIsAbsolute(path);
}

u64 si_pathLastWriteTime(cstring filename) {
	SI_ASSERT_NOT_NULL(filename);

#if defined(SI_SYSTEM_WINDOWS)
	FILETIME lastWriteTime = {0};
	WIN32_FILE_ATTRIBUTE_DATA data = {0};

	siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
	siUtf16String wideFilename = si_utf8ToUtf16Str(stack, filename, nil);
	if (GetFileAttributesExW(wideFilename, GetFileExInfoStandard, &data)) {
		lastWriteTime = data.ftLastWriteTime;
	}

	ULARGE_INTEGER res;
	res.LowPart = lastWriteTime.dwLowDateTime;
	res.HighPart = lastWriteTime.dwHighDateTime;
	return res.QuadPart;
#else
	struct stat fs;
	u64 res = stat(filename, &fs);
	SI_STOPIF(res != 0, { SI_FS_ERROR_LOG(); return false; });

	return fs.st_mtime;
#endif
}
F_TRAITS(inline)
cstring si_pathGetTmp(void) {
#if defined(SI_SYSTEM_WINDOWS)
	static char tmp[MAX_PATH + 1];
	u16 wideTmp[SI_KILO(1)];
	GetTempPathW(SI_KILO(1), wideTmp);

	siAllocator tmpAlloc = si_allocatorMakeTmp(tmp, sizeof(tmp));
	si_utf16ToUtf8Str(&tmpAlloc, wideTmp, nil);

	return tmp;
#else
	static cstring tmpDirsEnv[] = {"TMPDIR", "TMP", "TEMP", "TEMPDIR"};
	for_range (i, 0, countof(tmpDirsEnv)) {
		cstring dir = getenv(tmpDirsEnv[i]);
		SI_STOPIF(dir != nil, return dir);
	}
	return "/tmp";
#endif
}

F_TRAITS(inline)
siFilePermissions si_pathPermissions(cstring path) {
	SI_ASSERT_NOT_NULL(path);
#if defined(SI_SYSTEM_WINDOWS)
	ACCESS_MASK mask;
	{
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);

		SECURITY_INFORMATION info = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
		SECURITY_DESCRIPTOR* desc = (SECURITY_DESCRIPTOR*)si_allocatorCurPtr(stack);
		GetFileSecurityW(widePath, info, desc, si_allocatorAvailable(stack), nil);

		GetEffectiveRightsFromAclA(desc->Dacl, desc->Owner, &mask);
	}

	siFilePermissions perms = SI_FS_PERM_GROUP_EXEC | SI_FS_PERM_OTHERS_EXEC | SI_FS_PERM_OWNER_EXEC;
	perms |= ((mask & READ_CONTROL) != 0) * (SI_FS_PERM_GROUP_READ | SI_FS_PERM_OWNER_READ | SI_FS_PERM_OTHERS_READ);
	perms |= ((mask & WRITE_OWNER) != 0) * (SI_FS_PERM_OWNER_WRITE | SI_FS_PERM_GROUP_WRITE | SI_FS_PERM_OTHERS_WRITE);

	return perms;
#else
	struct stat fs;
	i32 res = stat(path, &fs);
	SI_STOPIF(res != 0, { SI_FS_ERROR_LOG(); return false; });

	return fs.st_mode;
#endif
}

static b32 SI_STD_FILE_SET = false;
static siFile SI_STD_FILE_ARR[SI_STDTYPE_COUNT] = {0};

F_TRAITS(inline)
siFile* si_fileGetStdFile(siStdFileType type) {
	SI_ASSERT_MSG(si_between(type, SI_STDTYPE_INPUT, SI_STDTYPE_ERROR), "Invalid STD file type.");

	#define SET_STD_FILE(index, type) SI_STD_FILE_ARR[index].handle = type

	if (!SI_STD_FILE_SET) {
		#if defined(SI_SYSTEM_WINDOWS)
			SET_STD_FILE(0, GetStdHandle(STD_INPUT_HANDLE));
			SET_STD_FILE(1, GetStdHandle(STD_OUTPUT_HANDLE));
			SET_STD_FILE(2, GetStdHandle(STD_ERROR_HANDLE));
			if (IsValidCodePage(CP_UTF8)) {
				SetConsoleOutputCP(CP_UTF8);
			}
		#else
			SET_STD_FILE(0, 0);
			SET_STD_FILE(1, 1);
			SET_STD_FILE(2, 2);
		#endif
		SI_STD_FILE_SET = true;
	}
	return &SI_STD_FILE_ARR[type];
	#undef SET_STD_FILE
}

F_TRAITS(inline)
siFile si_fileCreate(cstring path) {
	return si_fileOpenMode(path, SI_FILE_WRITE | SI_FILE_PLUS);
}
F_TRAITS(inline)
siFile si_fileOpen(cstring path) {
	return si_fileOpenMode(path, SI_FILE_READ | SI_FILE_PLUS);
}
siFile si_fileOpenMode(cstring path, siFileMode mode) {
	SI_ASSERT_NOT_NULL(path);

#if defined(SI_SYSTEM_WINDOWS)
	u32 access, disposition;
	switch (mode) {
		case SI_FILE_READ:
			access = GENERIC_READ;
			disposition = OPEN_EXISTING;
			break;
		case SI_FILE_WRITE:
			access = GENERIC_WRITE;
			disposition = CREATE_ALWAYS;
			break;
		case SI_FILE_APPEND:
			access = GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			break;
		case SI_FILE_READ | SI_FILE_PLUS:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_EXISTING;
			break;
		case SI_FILE_WRITE | SI_FILE_PLUS:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = CREATE_ALWAYS;
			break;
		case SI_FILE_APPEND | SI_FILE_PLUS:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			break;
		default:
			disposition = access = 0;
			SI_PANIC_MSG("Invalid file mode.");
	}

	rawptr handle;
	{
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);
		handle = CreateFileW(
			widePath,
			access, FILE_SHARE_READ | FILE_SHARE_DELETE, nil,
			disposition, FILE_ATTRIBUTE_NORMAL, nil
		);
	}
	SI_STOPIF(handle == INVALID_HANDLE_VALUE, { SI_FS_ERROR_DECLARE(); return (siFile){0}; });

	if (mode & SI_FILE_APPEND) {
		LARGE_INTEGER offset = {0};
		if (!SetFilePointerEx(handle, offset, nil, FILE_END)) {
			SI_FS_ERROR_DECLARE();
			CloseHandle(handle);
			return (siFile){0};
		}
	}
#else
	i32 flags;
	switch (mode & SI_FILE_ALL) {
		case SI_FILE_READ:
			flags = O_RDONLY;
			break;
		case SI_FILE_WRITE:
			flags = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case SI_FILE_APPEND:
			flags = O_WRONLY | O_APPEND | O_CREAT;
			break;
		case SI_FILE_READ | SI_FILE_PLUS:
			flags = O_RDWR;
			break;
		case SI_FILE_WRITE | SI_FILE_PLUS:
			flags = O_RDWR | O_CREAT | O_TRUNC;
			break;
		case SI_FILE_APPEND | SI_FILE_PLUS:
			flags = O_RDWR | O_APPEND | O_CREAT;
			break;
		default:
			flags = 0;
			SI_PANIC_MSG("Invalid file mode.");
		}

		i64 handle = open(path, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		SI_STOPIF(handle == -1, { SI_FS_ERROR_DECLARE(); return (siFile){0}; });
#endif
	siFile res = {
		handle,
		0,
		path,
		si_pathLastWriteTime(path)
	};
	si_fileSizeUpdate(&res);

	return res;
}

usize si_fileSize(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);

#if defined(SI_SYSTEM_WINDOWS)
	LARGE_INTEGER size;
	GetFileSizeEx(file.handle, &size);
	return size.QuadPart;
#else
	usize prevOffset = si_fileTell(file);
	usize bytes = si_fileSeek(file, 0, SI_FILE_MOVE_END);
	si_fileSeek(file, prevOffset, SI_FILE_MOVE_BEGIN);

	return bytes;
#endif
}
F_TRAITS(inline)
void si_fileSizeUpdate(siFile* file) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL((rawptr)file->handle);
#if defined(SI_SYSTEM_WINDOWS)
	LARGE_INTEGER res;
	GetFileSizeEx(file->handle, &res);
	file->size = res.QuadPart;
#else
	file->size = si_fileSeek(*file, 0, SI_FILE_MOVE_END);
#endif
}

F_TRAITS(inline)
rawptr si_fileRead(siAllocator* alloc, siFile file, usize len) {
	return si_fileReadAtEx(alloc, file, si_fileTell(file) - 1, len, len);
}
F_TRAITS(inline)
rawptr si_fileReadBuf(siFile file, usize len, rawptr outBuffer) {
	return si_fileReadAtBufEx(file, si_fileTell(file) - 1, len, outBuffer, len);
}
F_TRAITS(inline)
rawptr si_fileReadEx(siAllocator* alloc, siFile file, usize len, usize bufferLen) {
	return si_fileReadAtEx(alloc, file, si_fileTell(file) - 1, len, bufferLen);
}
F_TRAITS(inline)
rawptr si_fileReadBufEx(siFile file, usize len, rawptr outBuffer, usize bufferLen) {
	return si_fileReadAtBufEx(file, si_fileTell(file) - 1, len, outBuffer, bufferLen);
}
F_TRAITS(inline)
rawptr si_fileReadAt(siAllocator* alloc, siFile file, isize offset, usize len) {
	return si_fileReadAtEx(alloc, file, offset, len, len);
}
F_TRAITS(inline)
rawptr si_fileReadAtBuf(siFile file, isize offset, usize len, rawptr outBuffer) {
	return si_fileReadAtBufEx(file, offset, len, outBuffer, len);
}
F_TRAITS(inline)
rawptr si_fileReadAtEx(siAllocator* alloc, siFile file, isize offset, usize len,
		usize bufferLen) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);

	len = si_min(len, bufferLen);
	rawptr buffer = si_malloc(alloc, len);
	si_fileReadAtBufEx(file, offset, len, buffer, len);
	return buffer;
}
F_TRAITS(inline)
rawptr si_fileReadAtBufEx(siFile file, isize offset, usize len, rawptr outBuffer,
		usize bufferLen) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
	len = si_min(len, bufferLen);

#if defined(SI_SYSTEM_WINDOWS)
	si_fileSeek(file, offset, SI_FILE_MOVE_BEGIN);

	DWORD bytesRead;
	b32 res = ReadFile(file.handle, outBuffer, len, &bytesRead, NULL);
	SI_STOPIF(!res, { SI_FS_ERROR_DECLARE(); return nil; });
#else
	isize bytesRead = pread(file.handle, buffer, len, offset);
	SI_STOPIF(bytesRead == -1, { SI_FS_ERROR_DECLARE(); return nil; });
#endif

	SI_FS_READ_BYTES = bytesRead;
	return outBuffer;
}
F_TRAITS(inline)
rawptr si_fileReadContents(siAllocator* alloc, siFile file) {
	return si_fileReadAtEx(alloc, file, 0, file.size, file.size);
}
F_TRAITS(inline)
rawptr si_fileReadContentsBuf(siFile file, rawptr outBuffer) {
	return si_fileReadAtBufEx(file, 0, file.size, outBuffer, file.size);
}
F_TRAITS(inline)
rawptr si_fileReadContentsEx(siAllocator* alloc, siFile file, usize bufferLen) {
	return si_fileReadAtEx(alloc, file, 0, file.size, bufferLen);
}
F_TRAITS(inline)
rawptr si_fileReadContentsBufEx(siFile file, rawptr outBuffer, usize bufferLen) {
	return si_fileReadAtBufEx(file, 0, file.size, outBuffer, bufferLen);
}
siArray(siString) si_fileReadlines(siAllocator* alloc, siFile file) {
	SI_ASSERT_NOT_NULL(alloc);
	siAllocator* tmp = si_allocatorMake(2 * file.size * (sizeof(siString) + sizeof(siStringHeader)));

	siString buffer = (siString)si_fileReadContents(tmp, file);
	siArray(siString) res = si_stringSplit(alloc, buffer, "\n");

	si_allocatorFree(tmp);
	return res;
}

F_TRAITS(inline)
usize si_fileWrite(siFile* file, cstring content) {
	return si_fileWriteAtLen(file, (rawptr)content, si_cstrLen(content), si_fileTell(*file) - 1);
}
F_TRAITS(inline)
usize si_fileWriteLen(siFile* file, rawptr content, usize len) {
	return si_fileWriteAtLen(file, content, len, si_fileTell(*file) - 1);
}
F_TRAITS(inline)
usize si_fileWriteAt(siFile* file, cstring content, isize offset) {
	return si_fileWriteAtLen(file, (rawptr)content, si_cstrLen(content), offset);
}
usize si_fileWriteAtLen(siFile* file, rawptr content, usize contentLen, isize offset) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL((rawptr)file->handle);
	SI_ASSERT_NOT_NULL(content);

#if defined(SI_SYSTEM_WINDOWS)
	si_fileSeek(*file, offset, SI_FILE_MOVE_BEGIN);
	DWORD bytesWritten;
	WriteFile(file->handle, content, contentLen, &bytesWritten, NULL);
#else
	isize bytesWritten;
	isize curOffset = si_fileSeek(*file, 0, SI_FILE_MOVE_CURRENT);
	if (curOffset == offset) {
		bytesWritten = write(file->handle, content, contentLen);
	}
	else {
		bytesWritten = pwrite(file->handle, content, contentLen, offset);
	}
	SI_STOPIF(bytesWritten == -1, { SI_FS_ERROR_DECLARE(); return 0; });
#endif

	file->size += bytesWritten;
	return bytesWritten;
}
usize si_fileWriteAtLine(siFile* file, cstring content, usize index) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL((rawptr)file->handle);
	SI_ASSERT_NOT_NULL(content);

	siAllocator* tmp = si_allocatorMake(2 * file->size * (sizeof(siStringHeader) + sizeof(siString)));
	siArray(siString) buffer = si_fileReadlines(tmp, *file);
#if defined(SI_NO_ALLOCATOR)
	siString previousLine = buffer[index];
#endif
	SI_ASSERT_MSG(index < si_arrayLen(buffer), "Index is either not 0 or higher than the amount of lines in the file.");

	buffer[index] = (siString)content;
	siString newFileContent = si_arrayToSistring(buffer, "\n");

	usize len = si_fileWriteLen(file, newFileContent, si_stringLen(newFileContent));
#if defined(SI_NO_ALLOCATOR)
	usize i;
	for (i = 0; i < si_arrayLen(buffer); i++) {
		if (i == index) {
			continue;
		}
		si_stringFree(buffer[i]);
	}
	si_stringFree(previousLine);
	si_arrayFree(buffer);
	si_stringFree(newFileContent);
#endif

	return len;
}

F_TRAITS(inline)
usize si_fileTell(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
	usize len = si_fileSeek(file, 0, SI_FILE_MOVE_CURRENT);
	return len;
}
F_TRAITS(inline)
usize si_fileSeek(siFile file, usize offset, siFileMoveMethod method) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
#if defined(SI_SYSTEM_WINDOWS)
	LARGE_INTEGER win32Offset;
	win32Offset.QuadPart = offset;

	isize res = SetFilePointerEx(file.handle, win32Offset, &win32Offset, method);
#else
	#if defined(SI_SYSTEM_OSX)
		isize res = lseek(file.handle, offset, method);
	#else
		isize res = lseek64(file.handle, offset, method);
	#endif
#endif
	return res;
}
F_TRAITS(inline)
b32 si_fileSeekFront(siFile file) {
	return si_fileSeek(file, 0, SI_FILE_MOVE_BEGIN) != 0;
}
F_TRAITS(inline)
b32 si_fileSeekBack(siFile file) {
	return si_fileSeek(file, 0, SI_FILE_MOVE_END) != 0;
}
b32 si_fileTruncate(siFile* file, usize size) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL((rawptr)file->handle);

	usize prevOffset = si_fileTell(*file);
#if defined(SI_SYSTEM_WINDOWS)
	b32 res = si_fileSeek(*file, size, SI_FILE_MOVE_BEGIN);
	SI_STOPIF(!res, return false);

	res = SetEndOfFile(file->handle);
	SI_STOPIF(!res, SI_FS_ERROR_DECLARE_EX(SI_FS_ERROR_TRUNCATE_FAILURE));

	si_fileSeek(*file, prevOffset, SI_FILE_MOVE_BEGIN);

	return res;
#else
	i32 res = ftruncate(file->handle, size);
	SI_STOPIF(res == -1, SI_FS_ERROR_DECLARE_EX(SI_FS_ERROR_TRUNCATE_FAILURE));
	si_fileSeek(*file, prevOffset, SI_FILE_MOVE_BEGIN);
	return (res == 0);
#endif
}

void si_fileClose(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
#if defined(SI_SYSTEM_WINDOWS)
	CloseHandle(file.handle);
#else
	close(file.handle);
#endif
}

siDirectory si_dirOpen(siAllocator* alloc, cstring path) {
	SI_ASSERT_NOT_NULL(path);

#if defined(SI_SYSTEM_WINDOWS)
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));

	usize len;
	siUtf16String widePath = si_utf8ToUtf16Str(stack, path, &len);
	SI_ASSERT_MSG(len + 4 <= MAX_PATH, "Path is larger than 260 bytes.");

	// NOTE(EimaMei): Reason why this works is because 'appendText' is right
	// after 'widePath' in memory.
	u16* appendText = si_mallocArray(stack, u16, 2);
	appendText[-1] = '\\';
	appendText[0] = '*';
	appendText[1] = '\0';

	WIN32_FIND_DATAW tmp;
	HANDLE handle = FindFirstFileW(widePath, &tmp);
	SI_ASSERT(handle != INVALID_HANDLE_VALUE);
	FindNextFileW(handle, &tmp);
#else
	DIR* handle = opendir(path);
	SI_STOPIF(handle == nil, { SI_FS_ERROR_DECLARE(); return (siDirectory){0}; });
	{ (void)readdir(handle); (void)readdir(handle); }
#endif

	siDirectory dir;
	dir.handle = handle;
	dir.pathLen = si_cstrLen(path);

	if (path[dir.pathLen - 1] == '/' || path[dir.pathLen - 1] == '\\') {
		dir.path = si_cstrMakeLen(alloc, path, dir.pathLen);
	}
	else {
		dir.path = si_cstrMakeLen(alloc, path, dir.pathLen + 1);
		dir.path[dir.pathLen] = SI_PATH_SEPARATOR;
		dir.pathLen += 1;
	}

	return dir;
}

F_TRAITS(inline)
b32 si_dirPollEntry(siDirectory dir, siDirectoryEntry* entry) {
	return si_dirPollEntryEx(dir, entry, false);
}
b32 si_dirPollEntryEx(siDirectory dir, siDirectoryEntry* entry, b32 fullPath) {
	SI_ASSERT_NOT_NULL(entry);
	SI_ASSERT_NOT_NULL(dir.handle);

#if defined(SI_SYSTEM_WINDOWS)
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
	WIN32_FIND_DATAW file;
	if (FindNextFileW(dir.handle, &file) == 0) {
		return false;
	}

	usize len;
	siUtf8String utf8Text = si_utf16ToUtf8Str(stack, file.cFileName, &len);
	entry->len = len + dir.pathLen * fullPath;

	memcpy(entry->path, dir.path, dir.pathLen * fullPath);
	memcpy(entry->path + dir.pathLen * fullPath, utf8Text, entry->len + 1);

	if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		entry->type = SI_IO_TYPE_DIR;
	}
	else if (file.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
		entry->type = SI_IO_TYPE_LINK;
	}
	else {
		entry->type = SI_IO_TYPE_FILE;
	}

	return true;
#else
	struct dirent* direntEntry = readdir((DIR*)dir.handle);
	SI_STOPIF(direntEntry == nil, return false);

	entry->len = strlen(direntEntry->d_name);

	memcpy(entry->path, dir.path, dir.pathLen * fullPath);
	memcpy(entry->path + dir.pathLen * fullPath, direntEntry->d_name, entry->len + 1);

	switch (direntEntry->d_type) {
		case DT_REG:  entry->type = SI_IO_TYPE_FILE; break;
		case DT_DIR:  entry->type = SI_IO_TYPE_DIR; break;
		case DT_LNK:  entry->type = SI_IO_TYPE_LINK; break;
		case DT_SOCK: entry->type = SI_IO_TYPE_SOCKET; break;
		case DT_CHR:  entry->type = SI_IO_TYPE_DEVICE; break;
		case DT_BLK:  entry->type = SI_IO_TYPE_BLOCK; break;
		case DT_FIFO: entry->type = SI_IO_TYPE_FIFO; break;
		default:      entry->type = 0;
	}

	return true;
#endif
}
void si_dirClose(siDirectory dir) {
#if defined(SI_SYSTEM_WINDOWS)
	CloseHandle(dir.handle);
#else
	closedir(dir.handle);
#endif
}

#endif /* SI_IO_IMPLEMENTATION */

#if defined(SI_IMPLEMENTATION_THREAD) && !defined(SI_NO_THREAD)

#if defined(SI_SYSTEM_WINDOWS)
F_TRAITS(inline intern)
DWORD WINAPI si_impl_threadProc(LPVOID arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func.ptr(t->arg);
	t->isRunning = false;

	return 0;
}
#else
F_TRAITS(inline intern)
rawptr si_impl_threadProc(rawptr arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func.ptr(t->arg);
	t->isRunning = false;

	return nil;
}
#endif

F_TRAITS(inline)
siThread si_threadCreateEx(siFunction function, rawptr arg) {
	siThread t;
	t.isRunning = false;
	t.func = function;
	t.arg = arg;

	return t;
}


F_TRAITS(inline)
void si_threadStart(siThread* t) {
	si_threadStartStack(t, 0);
}

void si_threadStartStack(siThread* t, usize stackSize) {
	#if defined(SI_SYSTEM_WINDOWS)
		t->id = CreateThread(nil, stackSize, si_impl_threadProc, t, 0, nil);
		t->isRunning = true;
		SI_ASSERT_MSG(t->id != nil, "Something went wrong with creating a new thread.");
	#else
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_attr_setstacksize(&attr, stackSize);
		t->isRunning = true;

		usize error_code = pthread_create(&t->id, &attr, si_impl_threadProc, t);

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY: break;
			case EAGAIN:  error_msg = "The system lacked the necessary resources to create another thread, or the system-imposed"
						  "limit on the total number of threads in a process PTHREAD_THREADS_MAX would be exceeded."; break;
			case EINVAL:  error_msg = "The value specified by attr is invalid at 'pthread_create'."; break;
			case EPERM:   error_msg = "The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy."; break;
			default:      error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);
		pthread_attr_destroy(&attr);
	#endif
}

void si_threadJoin(siThread* t) {
	t->isRunning = true;

	#if defined(SI_SYSTEM_WINDOWS)
		WaitForSingleObject(t->id, INFINITE);
		CloseHandle(t->id);
		t->id = INVALID_HANDLE_VALUE;
	#else
		usize error_code = pthread_join(t->id, nil);

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY: break;
			case EDEADLK: error_msg = "A deadlock was detected."; break;
			default:      error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);
	#endif

	t->isRunning = false;
}
void si_threadCancel(siThread* t) {
	#if defined(SI_SYSTEM_WINDOWS)
		si_fprintf(SI_STDERR, "%CRsi_threadCancel: This feature on Windows is not supported as of now.%C");
		SI_UNUSED(t);
	#else

		usize error_code = pthread_cancel(t->id);
		t->isRunning = false;

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY:   break;
			case ESRCH:     error_msg = "No thread could be found corresponding to that specified by the given thread ID."; break;
			default:        error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);

	#endif
}
F_TRAITS(inline)
void si_threadDestroy(siThread* t) {
	si_threadJoin(t);
}

void si_threadPrioritySet(siThread t, i32 priority) {
	#if defined(SI_SYSTEM_WINDOWS)
		isize res = SetThreadPriority(t.id, priority);
		SI_ASSERT_MSG(res != 0, "Something went wrong setting the thread priority.");
	#elif defined(SI_SYSTEM_UNIX)
		usize error_code = pthread_setschedprio(t.id, priority);

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY:  break;
			case EINVAL:   error_msg = "The value of 'priority' is invalid for the scheduling policy of the specified thread."; break;
			case ENOTSUP:  error_msg = "An attempt was made to set the priority to an unsupported value."; break;
			case EPERM:    error_msg = "The caller does not have the appropriate permission to set the scheduling policy"
						   "of the specified thread OR the implementation does not allow the application to modify the"
							"priority to the value specified."; break;
			case ESRCH:    error_msg = "The value specified by thread does not refer to an existing thread."; break;
			default:       error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);
	#else
		SI_PANIC_MSG("si_threadPrioritySet: Not supported on MacOS.");
		SI_UNUSED(t);
		SI_UNUSED(priority);
	#endif
}
#endif /* SI_IMPLEMENTATION_THREAD */

#if defined(SI_IMPLEMENTATION_BIT) && !defined(SI_NO_BIT)

F_TRAITS(inline)
usize si_numCountBitsU8(u8 num) {
	return (num * (u64)01001001001L & (u64)042104210421) % 017;
}

F_TRAITS(inline)
usize si_numCountBitsU32(u32 num) {
	 num -= ((num >> 1) & 0x55555555);
	 num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
	 num = (num + (num >> 4)) & 0x0F0F0F0F;
	 return (num * 0x01010101) >> 24;
}

F_TRAITS(inline)
usize si_numCountBitsU64(u64 num) {
	num = num - ((num >> 1) & 0x5555555555555555);
	num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
	num = (num + (num >> 4)) & 0xF0F0F0F0F0F0F0F;
	return (num * 0x101010101010101) >> 56;
}

F_TRAITS(inline)
usize si_numLeadingBitEx(u64 num, usize total_bits, siBitType bit) {
	u64 count = 0;

	usize i;
	for (i = total_bits - 1; i < USIZE_MAX; i -= 1) {
		if (SI_NUM_BIT_GET(num, i) == bit) {
			count += 1;
		}
		else {
			return count;
		}
	}

	return count;
}
F_TRAITS(inline)
usize si_numTrailingBitEx(u64 num, usize total_bits, siBitType bit) {
	u64 count = 0;

	for_range (i, 0, total_bits) {
		if (SI_NUM_BIT_GET(num, i) == bit) {
			count += 1;
		}
		else {
			return count;
		}
	}

	return count;
}
F_TRAITS(inline)
u64 si_numRotateLeftEx(u64 num, usize total_bits, usize bits) {
	return (num << bits) | (num >> (total_bits - bits));
}
F_TRAITS(inline)
u64 siNumRotateRightEx(u64 num, usize total_bits, usize bits) {
	return (num >> bits) | (num << (total_bits - bits));
}

F_TRAITS(inline)
u64 siNumReverseBitsEx(u64 num, usize total_bits) {
	u64 res = 0;

	for_range(i, 0, total_bits) {
		res <<= 1;
		res |= (num & 1);
		num >>= 1;
	}

	return res;
}

inline siArray(u8) si_numToBytesEx(siAllocator* alloc, u64 num, usize num_sizeof) {
	siArray(u8) res = (u8*)si_arrayMakeReserve(alloc, sizeof(*res), num_sizeof);
	SI_ARRAY_HEADER(res)->len = num_sizeof;

	for_range (i, 0, num_sizeof) {
		res[i] = (num >> ((num_sizeof - 1 - i) * 8)) & 255;
	}

	return res;
}
F_TRAITS(inline)
u64 si_bytesToNumSiArr(siArray(u8) bytes) {
	SI_ASSERT_NOT_NULL(bytes);
	u64 res = 0;
	usize shift = (si_arrayLen(bytes) - 1) * 8;

	for_each (byte, bytes) {
		res |= si_cast(u64, *byte) << shift;
		shift -= 8;
	}

	return res;
}
F_TRAITS(inline)
u64 si_bytesToNumArr(siByte* array, usize len) {
	SI_ASSERT_NOT_NULL(array);
	usize res = 0;
	usize shift = (len - 1) * 8;

	for_range (i, 0, len) {
		res |= (u64)array[i] << shift;
		shift -= 8;
	}

	return res;
}

F_TRAITS(inline)
u64 si_pow10(u32 exponent) {
	static const u64 allPossibleValues[] = {
		1e+0, 1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 1e+6, 1e+7,
		1e+8, 1e+9, 1e+10, 1e+11, 1e+12, 1e+13, 1e+14, 1e+15,
		1e+16, 1e+17, 1e+18, 1e+19
	};
	SI_ASSERT_MSG(exponent < countof(allPossibleValues), "Exponent must be in range from 0 to 18 to not overflow.");

	return allPossibleValues[exponent];
}

F_TRAITS(inline)
f64 si_pow10F64(i32 exponent) {
	static const f64 allPossibleValues[] = {
		1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13,
		1e-12, 1e-11, 1e-10, 1e-09, 1e-08, 1e-07,
		1e-06, 1e-05, 1e-04, 1e-03, 1e-02, 1e-01,

		1e+0,

		1e+01, 1e+02, 1e+03, 1e+04, 1e+05, 1e+06,
		1e+07, 1e+08, 1e+09, 1e+10, 1e+11, 1e+12,
		1e+13, 1e+14, 1e+15, 1e+16, 1e+17, 1e+18
	};
	SI_ASSERT_MSG(si_between(exponent, -18, 18), "Exponent large. Must be from 0 to 19 to not overflow.");

	return allPossibleValues[exponent + 18];
}

F_TRAITS(inline)
usize si_numLen(u64 num) {
	return si_numLenEx(num, 10);
}
F_TRAITS(inline)
usize si_numLenEx(u64 num, u32 base) {
	usize count = 0;
	do {
		count += 1;
		num /= base;
	} while (0 < num);

	return count;
}
F_TRAITS(inline)
usize si_numLenI64(i64 num) {
	return si_numLenEx(num, 10);
}
F_TRAITS(inline)
usize si_numLenI64Ex(i64 num, u32 base) {
	b32 isNegative = si_numIsNeg(num);
	u64 unsignedNum = (num ^ -isNegative) + isNegative;
	return si_numLenEx(unsignedNum, base);
}

F_TRAITS(inline)
u64 si_numRoundNearestMultiple(u64 num, usize multiple) {
	return ((num + multiple / 2) / multiple) * multiple;
}

#endif

#if defined(SI_IMPLEMENTATION_CPU) && !defined(SI_NO_CPU)

f32 SI_CPU_FREQ_GHZ = 0;

F_TRAITS(inline)
f32 si_cpuClockSpeed(void) {
	SI_STOPIF(SI_CPU_FREQ_GHZ != 0, return SI_CPU_FREQ_GHZ);
	u64 begin = si_RDTSC();
	si_sleep(100);
	u64 end = si_RDTSC();

	SI_CPU_FREQ_GHZ = si_numRoundNearestMultiple((end - begin) / 100000.0, 10) / 1000.0;
	return SI_CPU_FREQ_GHZ;
}

#endif /* SI_IMPLEMENTATION_CPU */

#if defined(SI_IMPLEMENTATION_PRINT) && !defined(SI_NO_PRINT)

F_TRAITS(inline)
isize si_print(cstring str) {
	return si_fprint(SI_STDOUT, str);
}
F_TRAITS(inline)
isize si_printf(cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_printfVa(fmt, va);
	va_end(va);

	return res;
}
F_TRAITS(inline)
isize si_printfVa(cstring fmt, va_list va) {
	return si_fprintfVa(SI_STDOUT, fmt, va);
}

F_TRAITS(inline)
isize si_fprint(siFile* file, cstring str) {
	return si_fileWrite(file, str);
}
F_TRAITS(inline)
isize si_fprintf(siFile* file, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_fprintfVa(file, fmt, va);
	va_end(va);

	return res;
}
F_TRAITS(inline)
isize si_fprintfVa(siFile* file, cstring fmt, va_list va) {
	char buffer[SI_KILO(4)];
	isize count = si_snprintfVa(buffer, sizeof(buffer), fmt, va) - 1;
	count = si_fileWriteLen(file, buffer, count);
	return count;
}

F_TRAITS(inline)
isize si_sprintf(char* buffer, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_sprintfVa(buffer, fmt, va);
	va_end(va);

	return res;
}
F_TRAITS(inline)
isize si_snprintf(char* buffer, usize outCapacity, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_snprintfVa(buffer, outCapacity, fmt, va);
	va_end(va);

	return res;
}
F_TRAITS(inline)
isize si_sprintfVa(char* buffer, cstring fmt, va_list va) {
	return si_snprintfVa(buffer, USIZE_MAX, fmt, va);
}
isize si_snprintfVa(char* buffer, usize outCap, cstring fmt, va_list va) {
	union { u64 U64; i64 I64; f64 F64; i32 I32; u32 U32; cstring STR; siByte* PTR; } vaValue;
	usize bufIndex = 0;
	usize len;
	siAllocator* stack = si_allocatorMakeStack(64);

	cstring fmtPtr = fmt;

	#define SET_FMT_PTR(x, ptr) x = *ptr; ptr += 1
	#define CHECK_FOR_PAD(padSize, strLen, buffer, bufIndex) \
		do { \
			if (padSize != 0) { \
				isize padLen = padSize - strLen; \
				memset(&buffer[bufIndex], padLetter, padLen * !si_numIsNeg(padLen)); \
				bufIndex += padLen * !si_numIsNeg(padLen); \
				padSize = 0; \
			} \
		} while (0)
	#define COLOR_CASE(code) \
		memcpy(&buffer[bufIndex], "\33[0;" code "m", countof("\33[0;" code "m") - 1); \
		bufIndex += countof("\33[0;" code "m") - 1

	while (--outCap != 0) {
		char x = *fmtPtr;
		fmtPtr += 1;
		SI_STOPIF(x == '\0', break);
		SI_STOPIF(x != '%', { buffer[bufIndex] = x; bufIndex += 1; continue; });

		x = *fmtPtr;
		fmtPtr += 1;

		static u32 padSize = 0;
		u32 afterPoint = 6;

		i32 padLetter = ' ';
		b32 argIsLong = false;

		b32 afterPointIsSet = false;
		u32* ptrToVar = &padSize;

		vaValue.U64 = 0;

SI_GOTO_LABEL(GOTO_PRINT_SWITCH)
		switch (x) {
			case 'C': {
				SET_FMT_PTR(x, fmtPtr);

				SI_GOTO_LABEL(GOTO_COLOR_CASE_SWTICH);
				switch (x) {
					case 'R': { COLOR_CASE("31"); break; }
					case 'G': { COLOR_CASE("32"); break; }
					case 'Y': { COLOR_CASE("33"); break; }
					case 'B': { COLOR_CASE("34"); break; }
					case 'M': { COLOR_CASE("35"); break; }
					case 'C': { COLOR_CASE("36"); break; }
					case 'W': { COLOR_CASE("37"); break; }
					case '*': { x = va_arg(va, i32); goto GOTO_COLOR_CASE_SWTICH; }
					default:  { COLOR_CASE("0");  fmtPtr -= 1; break; }
				}
				break;
			}
			case 'n': {
				(void)va_arg(va, signed int*);
				break;
			}
			case '%': {
				buffer[bufIndex] = '%';
				bufIndex += 1;
				break;
			}
			case 'l': {
				SET_FMT_PTR(x, fmtPtr);
				SI_STOPIF(x == 'l', SET_FMT_PTR(x, fmtPtr));
				argIsLong = true;
				goto GOTO_PRINT_SWITCH;
			}
			case 'u': {
				goto GOTO_UNSIGNED_DECIMAL;
			}
			case '#': {
				do {
					SET_FMT_PTR(x, fmtPtr);
				} while (x == 'l' || si_charIsDigit(x) || x == '.' || x == '*');
				buffer[bufIndex + 0] = '0';
				buffer[bufIndex + 1] = si_charLower(x);
				bufIndex += 2;
				goto GOTO_PRINT_SWITCH;
			}
			case '*': {
				SET_FMT_PTR(x, fmtPtr);
				*ptrToVar = va_arg(va, u32);
				goto GOTO_PRINT_SWITCH;
			}
			case '.': {
				SET_FMT_PTR(x, fmtPtr);
				ptrToVar = &afterPoint;
				afterPointIsSet = true;
				goto GOTO_PRINT_SWITCH;
			}
			case ' ': case '-': case '+': {
				char sign = x;
				SET_FMT_PTR(x, fmtPtr);
				buffer[bufIndex] = sign;
				bufIndex += 1;
				goto GOTO_PRINT_SWITCH;
			}
			case '0': {
				SET_FMT_PTR(x, fmtPtr);
				if (si_charIsDigit(x)) {
					padLetter = '0';
				}
				else {
					*ptrToVar = 0;
				}

				goto GOTO_PRINT_SWITCH;
			}
			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			case '8': case '9': {
				siByte* buf = stack->ptr;
				*buf = x;
				buf += 1;

				while (si_charIsDigit(*fmtPtr)) {
					*buf = *fmtPtr;
					buf += 1;
					fmtPtr += 1;
				}
				SET_FMT_PTR(x, fmtPtr);

				usize count = si_cstrToU64Len((char*)stack->ptr, buf - stack->ptr);
				*ptrToVar = count;
				goto GOTO_PRINT_SWITCH;
			}
			case 'X': case 'x': {
				vaValue.U64 = (argIsLong) ? va_arg(va, u64) : va_arg(va, u32);

				char* str = si_u64ToCstrEx(stack, vaValue.U64, 16, &len);
				SI_STOPIF(x == 'x', si_cstrLower(str));

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], str, len);
				bufIndex += len;
				break;
			}
			case 'O': case 'o': {
				vaValue.U64 = (argIsLong) ? va_arg(va, u64) : va_arg(va, u32);
				char* str = si_u64ToCstrEx(stack, vaValue.U64, 8, &len);

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], str, len);
				bufIndex += len;
				break;
			}
			case 'b': {
				vaValue.U64 = (argIsLong) ? va_arg(va, u64) : va_arg(va, u32);
				char* str = si_u64ToCstrEx(stack, vaValue.U64, 2, &len);

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], str, len);
				bufIndex += len;
				break;
			}
			case 'c': {
				vaValue.I32 = (char)va_arg(va, i32);

				CHECK_FOR_PAD(padSize, 1, buffer, bufIndex);
				buffer[bufIndex] = (char)vaValue.I32;
				bufIndex += 1;
				break;
			}
			case 's': {
				vaValue.STR = va_arg(va, cstring);
				len = (afterPoint * afterPointIsSet);
				if (vaValue.STR == nil) {
					static char nilStr[] = "(nil)";
					vaValue.STR = nilStr;
					len = countof("(nil)") - 1;
				}

				if (len == 0) {
					if (!argIsLong) {
						len = si_cstrLen(vaValue.STR);
					}
					else {
						// NOTE(EimaMei): Won't work for strings larger than 4096
						// bytes, but I will just playfully ignore that issue.
						// I mean, I doubt anyone even really prints such large
						// UTF-16 strings nowadays.
						siAllocator* tmpStack2 = si_allocatorMakeStack(SI_KILO(4));
						vaValue.STR = si_utf16ToUtf8Str(
							tmpStack2,
							(siUtf16String)vaValue.STR,
							&len
						);
					}
				}

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], vaValue.STR, len);
				bufIndex += len;
				break;
			}
			case 'h': {
				SI_STOPIF(fmtPtr[bufIndex + 1] == 'h', SET_FMT_PTR(x, fmtPtr));
				goto GOTO_SIGNED_DECIMAL;
			}
			case 'Z': case 'z': {
				argIsLong = (sizeof(usize) == 8);
				if (*fmtPtr == 'u') {
					SET_FMT_PTR(x, fmtPtr);
					goto GOTO_UNSIGNED_DECIMAL;
				}
				else {
					goto GOTO_SIGNED_DECIMAL;
				}
			}
			case 't': case 'i': case 'd': {
				cstring str;

				SI_LABEL(LABEL_PICK_DECIMAL) {
					SI_GOTO_LABEL(GOTO_SIGNED_DECIMAL) {
						vaValue.I64 = (argIsLong) ? va_arg(va, i64) : va_arg(va, i32);
						str = si_i64ToCstrEx(stack, vaValue.I64, 10, &len);
						goto GOTO_UNSIGNED_DECIMAL_END;
					}
					SI_GOTO_LABEL(GOTO_UNSIGNED_DECIMAL) {
						vaValue.U64 = (argIsLong) ? va_arg(va, u64) : va_arg(va, u32);
						str = si_u64ToCstrEx(stack, vaValue.U64, 10, &len);
						SI_GOTO_LABEL(GOTO_UNSIGNED_DECIMAL_END);
					}
				}

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], str, len);
				bufIndex += len;
				break;
			}
			case 'F': case 'f': {
				vaValue.F64 = va_arg(va, f64);
				SI_GOTO_LABEL(GOTO_FLOAT64)

				cstring str = si_f64ToCstrEx(stack, vaValue.F64, 10, afterPoint, &len);
				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], str, len);
				bufIndex += len;
				break;
			}
			case 'E': case 'e': {
				vaValue.F64 = va_arg(va, f64);
				SI_GOTO_LABEL(GOTO_SCIENTIFIC_NOTATION)

				usize exponent = 0;
				while (vaValue.F64 > 10) {
					vaValue.F64 /= 10;
					exponent += 1;
				}
				cstring str = si_f64ToCstrEx(stack, vaValue.F64, 10, afterPoint, &len);

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], str, len);
				bufIndex += len;

				buffer[bufIndex + 0] = x;
				buffer[bufIndex + 1] = '+';
				if (exponent < 10) {
					buffer[bufIndex + 2] = '0';
					buffer[bufIndex + 3] = exponent + '0';
				}
				else {
					buffer[bufIndex + 2] = (exponent / 10) + '0';
					buffer[bufIndex + 3] = (exponent % 10) + '0';
				}
				bufIndex += 4;
				break;
			}
			case 'G': case 'g': {
				vaValue.F64  = va_arg(va, f64);
				u64 pow10Val = 0;
				if (vaValue.F64 < 0.0001) {
					x = 'E' + (x - 'G');
					goto GOTO_SCIENTIFIC_NOTATION;

				}
				else if (vaValue.F64 > (pow10Val = si_pow10(afterPoint))) {
					afterPoint -= (u64)vaValue.F64 / pow10Val;
					x = 'E' + (x - 'G');
					goto GOTO_SCIENTIFIC_NOTATION;
				}
				else {
					f64 remainder = vaValue.F64 - (u64)vaValue.F64;
					u64 newAfterPoint = 0;
					f64 maxZeroValue = si_pow10F64(-(i32)afterPoint);
					while (newAfterPoint < afterPoint) {
						remainder *= 10;
						i64 digit = (i64)(remainder + (0.5 * ((i64)remainder != 0)));
						if (si_between(si_abs(remainder), 0, maxZeroValue)) {
							break;
						}
						remainder -= digit;
						newAfterPoint += 1;

					}
					afterPoint = newAfterPoint;

					goto GOTO_FLOAT64;
				}


				break;
			}
			case 'p': {
				vaValue.U64 = (u64)va_arg(va, rawptr);
				cstring str = si_u64ToCstrEx(stack, vaValue.U64, 16, &len);

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], "0x", 2);
				memcpy(&buffer[bufIndex + 2], str, len);
				bufIndex += len + 2;
				break;
			}
			case 'B': { /* Boolean */
				vaValue.I32 = va_arg(va, b32);
				cstring str;
				switch (vaValue.I32) {
					case true: {
						str = "true";
						len = 4;
						break;
					}
					case false: {
						str = "false";
						len = 5;
						break;
					}
				}

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], str, len);
				bufIndex += len;
				break;
			}
			case 'M': case 'm': { /* Memory */
				vaValue.PTR = (siByte*)va_arg(va, rawptr);
				len = va_arg(va, usize);

				CHECK_FOR_PAD(padSize, len, buffer, bufIndex);
				memcpy(&buffer[bufIndex], vaValue.PTR, len);
				bufIndex += len;
				break;
			}
			case 'S': { /* Sili */
				SET_FMT_PTR(x, fmtPtr);
				switch (x) {
					case 'A': {
						vaValue.PTR = (siByte*)va_arg(va, rawptr);
						SET_FMT_PTR(x, fmtPtr);
						x = si_charLower(x);

						buffer[bufIndex] = '{';
						bufIndex += 1;

						/* TODO(EimaMei): Add custom base support too. */
						union { i64 I64; u64 U64; siString STR; } itemValue;
						memset(&itemValue, 0, sizeof(itemValue));

						usize len;
						cstring str;

						static char intComma[] = ", ";
						static char strComma[] = "\", ";
						char* commaPtr = intComma;
						usize commaLen = countof(intComma) - 1;

						for_range (i, 0, si_arrayLen(vaValue.PTR) - 1) {
							memcpy(
								&itemValue,
								si_arrayGetPtr(vaValue.PTR, i),
								si_arrayTypeSize(vaValue.PTR)
							);
							switch (x) {
								case 'i': {
									str = si_i64ToCstrEx(stack, itemValue.I64, 10, &len);
									break;
								}
								case 'u': {
									str = si_u64ToCstrEx(stack, itemValue.U64, 10, &len);
									break;
								}
								case 's': {
									str = itemValue.STR; buffer[bufIndex] = '\"';
									bufIndex += 1;
									commaPtr = strComma;
									commaLen = countof(strComma) - 1;
									break;
								}
							}
							memcpy(&buffer[bufIndex], str, len);
							bufIndex += len;
							memcpy(&buffer[bufIndex], commaPtr, commaLen);
							bufIndex += commaLen;
							si_allocatorReset(stack);
						}
						memcpy(
							&itemValue,
							si_arrayGetPtr(vaValue.PTR, si_arrayLen(vaValue.PTR) - 1),
							si_arrayTypeSize(vaValue.PTR)
						);
						switch (x) {
							case 'i': {
								str = si_i64ToCstrEx(stack, itemValue.I64, 10, &len);
								break;
							}
							case 'u': {
								str = si_u64ToCstrEx(stack, itemValue.U64, 10, &len);
								break;
							}
							case 's': {
								str = itemValue.STR; buffer[bufIndex] = '\"';
								bufIndex += 1;
								break;
							}
						}
						memcpy(&buffer[bufIndex], str, len);
						bufIndex += len;

						buffer[bufIndex] = '}';
						bufIndex += 1;
						break;
					}
					case 'C': { /* Color */
						siColor value = va_arg(va, siColor);
						cstring rgba[4];
						usize lengths[4];

						for_range (i, 0, countof(rgba)) {
							u8* channel = (u8*)&value + i;
							rgba[i] = si_u64ToCstrEx(stack, *channel, 10, &lengths[i]);
							len += lengths[i];
						}
						len += 2 + 3 + 3;

						CHECK_FOR_PAD(padSize, len, buffer, bufIndex);

						buffer[bufIndex] = '{';
							bufIndex += 1;

						for_range (i, 0, countof(rgba) - 1) {
							memcpy(&buffer[bufIndex], rgba[i], lengths[i]);
							bufIndex += lengths[i];

							buffer[bufIndex + 0] = ',';
							buffer[bufIndex + 1] = ' ';
							bufIndex += 2;
						}
						memcpy(&buffer[bufIndex], rgba[3], lengths[3]);
						bufIndex += lengths[3];
						buffer[bufIndex] = '}';

						bufIndex += 1;
						break;
					}
				}
				break;
			}
			default: {
				buffer[bufIndex + 0] = '%';
				buffer[bufIndex + 1] = x;
				bufIndex += 2;
				break;
			}
		}

		si_allocatorReset(stack);
	}
	buffer[bufIndex] = '\0';
	bufIndex += 1;

	#undef SET_FMT_PTR
	#undef CHECK_FOR_PAD
	#undef COLOR_CASE

	return bufIndex;
}

#endif /* SI_IMPLEMENTATION_PRINT */

#if defined(SI_IMPLEMENTATION_DLL) && !defined(SI_NO_DLL)

F_TRAITS(inline)
siDllHandle si_dllLoad(cstring path) {
	SI_ASSERT_NOT_NULL(path);
#if defined(SI_SYSTEM_WINDOWS)
	return (siDllHandle)LoadLibraryA(path);
#else
	return (siDllHandle)dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
#endif
}
F_TRAITS(inline)
void si_dllUnload(siDllHandle dll) {
	SI_ASSERT_NOT_NULL(dll);
#if defined(SI_SYSTEM_WINDOWS)
	FreeLibrary((HMODULE)dll);
#else
	dlclose(dll);
#endif
}
F_TRAITS(inline)
siDllProc si_dllProcAddress(siDllHandle dll, cstring name) {
	SI_ASSERT_NOT_NULL(dll);
	SI_ASSERT_NOT_NULL(name);
#if defined(SI_SYSTEM_WINDOWS)
	return (siDllProc)GetProcAddress((HMODULE)dll, name);
#else
	return (siDllProc)dlsym(dll, name);
#endif
}
#endif /* defined(SI_IMPLEMENTATION_DLL) */

#if defined(SI_IMPLEMENTATION_MATH) && !defined(SI_NO_MATH)
inline i64 si_min(i64 a, i64 b) { return a < b ? a : b; }
inline f64 si_fmin(f64 a, f64 b) { return a < b ? a : b; }
inline f32 si_fminf(f32 a, f32 b) { return a < b ? a : b; }

inline i64 si_min3(i64 a, i64 b, i64 c) { return si_min(si_min(a, b), c); }
inline f64 si_fmin3(f64 a, f64 b, f64 c) { return si_fmin(si_fmin(a, b), c); }
inline f32 si_fminf3(f32 a, f32 b, f32 c) { return si_fminf(si_fminf(a, b), c); }

inline i64 si_max(i64 a, i64 b) { return a > b ? a : b; }
inline f64 si_fmax(f64 a, f64 b) { return a > b ? a : b; }
inline f32 si_fmaxf(f32 a, f32 b) { return a > b ? a : b; }

inline i64 si_max3(i64 a, i64 b, i64 c) { return si_max(si_max(a, b), c); }
inline f64 si_fmax3(f64 a, f64 b, f64 c) { return si_fmax(si_fmax(a, b), c); }
inline f32 si_fmaxf3(f32 a, f32 b, f32 c)  { return si_fmaxf(si_fmaxf(a, b), c); }

inline i64 si_abs(i64 x) { return x < 0 ? -x : x; }
inline f64 si_fabs(f64 x) { return x < 0 ? -x : x; }
inline f32 si_fabsf(f32 x) { return x < 0 ? -x : x; }

/* Returns a boolean that indicates if 'x' is between 'lower' and 'upper'. */
b32 si_between(i64 x, i64 lower, i64 upper) { return lower <= x && x <= upper; }
b32 si_betweenu(u64 x, u64 lower, u64 upper) { return lower <= x && x <= upper; }
b32 si_fbetween(f64 x, f64 lower, f64 upper) { return lower <= x && x <= upper; }
b32 si_fbetweenf(f32 x, f32 lower, f32 upper) { return lower <= x && x <= upper; }
#endif

#if defined(SI_IMPLEMENTATION_BENCHMARK) && !defined(SI_NO_BENCHMARK)

F_TRAITS(inline)
void si_intern_benchmarkThread(u32* arg) {
	si_sleep(*arg);
}

typedef struct {
	char runsCstr[countof("runs")];
	char spacePad[SI_NUM_MAX_DIGITS];
} siPerformanceMSG;

F_TRAITS(intern)
siPerformanceMSG* si_benchmarkGetMsgVars(siAllocator* alloc,
		usize multiplesOf10, usize medianIndex) {

	char runsCstr[] = "runs";
	SI_STOPIF(medianIndex == 1, runsCstr[3] = ' ');

	isize spacePadLen = si_numLen(multiplesOf10) - si_numLen(medianIndex);
	char spacePad[SI_NUM_MAX_DIGITS] = {0};
	memset(spacePad, ' ', spacePadLen);


	siPerformanceMSG* msg = (siPerformanceMSG*)si_malloc(alloc, sizeof(*msg));
	memcpy(msg->runsCstr, runsCstr, countof(runsCstr));
	memcpy(msg->spacePad, spacePad, spacePadLen);

	return msg;
}

void si_benchmarkLoopsAvgPrint(siAllocator* alloc, cstring funcname,
		usize start, usize end, siArray(u64) cycles, f32 ghzFreq) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction - '%s'\n"
		"Runs:\n",
		funcname
	);
	const siBenchmarkLimit* element = nil;

	usize arrayIndex = 0, medianIndex;
	for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10, arrayIndex += 1) {
		u64 amountCycles = cycles[arrayIndex];
		f64 amountTime = (f64)amountCycles / ghzFreq;

		element = si_benchmarkLimitLoop(amountTime);
		amountTime /= element->first;

		siPerformanceMSG* msg = si_benchmarkGetMsgVars(alloc, end, medianIndex);
		si_printf(
			"\t%s%zu %s - %9.4f %s (%lu cycles)\n",
			msg->spacePad, medianIndex, msg->runsCstr,
			amountTime, element->second, amountCycles
		);
	}

	f64 cyclesMedian = (f64)si_arraySumU64(cycles) / si_arrayLen(cycles);
	f64 timeMedian = cyclesMedian / ghzFreq;
	element = si_benchmarkLimitLoop(timeMedian);
	timeMedian /= element->first;

	si_printf(
		"Final result:\n"
			"\tTime average   - %9.4f %s\n"
			"\tCycles average - %lu cycles\n",
		timeMedian, element->second,
		(u64)cyclesMedian
	);
}

void si_benchmarkLoopsAvgCmpPrint(siAllocator* alloc, cstring funcname[2],
		usize start, usize end, siArray(u64) cycles[2], f32 ghzFreq) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction 1 - '%s'\n"
			"\tFunction 2 - '%s'\n"
		"Runs:\n",
		funcname[0], funcname[1]
	);
	const siBenchmarkLimit* element0 = nil, *element1 = nil;

	usize arrayIndex = 0, medianIndex;
	for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) {
		u64 cycles0 = cycles[0][arrayIndex],
			cycles1 = cycles[1][arrayIndex];
		f64 time_0 = (f64)cycles0 / ghzFreq,
			time_1 = (f64)cycles1 / ghzFreq;

		b32 zeroIsSlower = (time_0 > time_1);
		f64 ratio = zeroIsSlower ? (time_0 / time_1) : (time_1 / time_0);
		i32 clr0 = zeroIsSlower ? 'R' : 'G',
				clr1 = zeroIsSlower ? 'G' : 'R';

		element0 = si_benchmarkLimitLoop(time_0);
		element1 = si_benchmarkLimitLoop(time_1);
		time_0 /= element0->first;
		time_1 /= element1->first;

		siPerformanceMSG* msg = si_benchmarkGetMsgVars(alloc, end, medianIndex);
		si_printf(
			"\t%s%zu %s - %C*%9.4f%C %s vs %C*%9.4f%C %s (%4.4f ratio, %6lu vs %6lu cycles)\n",
			msg->spacePad, medianIndex, msg->runsCstr,
			clr0, time_0, element0->second, clr1, time_1, element1->second,
			ratio, cycles0, cycles1
		);

		arrayIndex += 1;
	}

	f64 cyclesMedian0 = (f64)si_arraySumU64(cycles[0]) / si_arrayLen(cycles[0]),
		cyclesMedian1 = (f64)si_arraySumU64(cycles[1]) / si_arrayLen(cycles[1]);
	f64 timeMedian0 = cyclesMedian0 / ghzFreq,
		timeMedian1 = cyclesMedian1 / ghzFreq;

	b32 zeroIsSlower = (timeMedian0 > timeMedian1);
	f64 ratio = zeroIsSlower ? (timeMedian0 / timeMedian1) : (timeMedian1 / timeMedian0);
	i32 clr0 = zeroIsSlower ? 'R' : 'G',
			clr1 = zeroIsSlower ? 'G' : 'R';

	element0 = si_benchmarkLimitLoop(timeMedian0);
	element1 = si_benchmarkLimitLoop(timeMedian1);
	timeMedian0 /= element0->first;
	timeMedian1 /= element1->first;

	si_printf(
		"Final result:\n"
			"\tTime average   - %C*%9.4f%C %s vs %C*%.4f%C %s (%4.4f ratio)\n"
			"\tCycles average - %lu cycles vs %lu cycles\n",
		clr0, timeMedian0, element0->second,
		clr1, timeMedian1, element1->second,
		ratio,
		(u64)cyclesMedian0, (u64)cyclesMedian1
	);
}

const siBenchmarkLimit siBenchLimit[] = {
	{(u64)0001, "ns"},
	{(u64)1000, "μs"},
	{(u64)1000 * 1000, "ms"},
	{(u64)1000 * 1000 * 1000, " s"},
	{(u64)1000 * 1000 * 1000 * 60, "min"},
	{(u64)1000 * 1000 * 1000 * 60 * 60, " h"},
	{(u64)1000 * 1000 * 1000 * 60 * 60 * 24, " d"} /* NOTE(EimaMei): I'll be shocked if someone someone's performance loop lasted an entire day. Also a bug happens here, I think. Oops. */
};

const siBenchmarkLimit* si_benchmarkLimitLoop(siTimeStamp time) {
	const siBenchmarkLimit* element = siBenchLimit;
	const siBenchmarkLimit* end = siBenchLimit + countof(siBenchLimit);

	while (element != end) {
		if (si_between(time, element->first, (element + 1)->first)) {
			break;
		}
		element += 1;
	}

	return element;
}

#endif


#if !defined(SI_COMPILER_MSVC) && defined(SI_ASM_USE_ATT_SYNTAX)
	#undef si_asm
	#undef SI_ASM_NL
	#define si_asm(asm, ...)  __asm__ __volatile__(asm __VA_ARGS__)
	#define SI_ASM_NL "\n\t"
#endif

#if defined(__cplusplus)
}
#endif

#endif /* SI_IMPLEMENTATION */

#if defined(SI_NO_ASSERTIONS_IN_HEADER)
	#undef SI_ASSERT
	#undef SI_ASSERT_MSG
	#undef SI_ASSERT_FMT
	#undef SI_ASSERT_NULL

	/* condition - EXPRESSION
	 * Crashes the app if the condition is not met. */
	#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
	/* condition - EXPRESSION | message - cstring
	 * Crashes the app with a message if the condition is not met. */
	#define SI_ASSERT_MSG(condition, message) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, "")
	/* condition - EXPRESSION | message - cstring | ...FMT - VARIADIC ARGUMENTS
	 * Crashes the app with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, ...) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__)
	/* ptr - rawptr
	 * Crashes the app if a pointer is NULL. */
	#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")
	/* path - cstring
	 * Crashes the app if the provided path doesn't exist. */
	#define SI_ASSERT_PATH_EXISTS(path) SI_ASSERT_FMT(si_pathExists(path), "Path '%s' doesn't exist.", path)

#endif /* SI_NO_ASSERTIONS_IN_HEADER */

/*
------------------------------------------------------------------------------
Copyright (C) 2023 EimaMei

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
------------------------------------------------------------------------------
*/
