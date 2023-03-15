typedef unsigned char uchar;
typedef unsigned short ushort;
typedef UINTN uint;//unsigned int
typedef unsigned long ulong;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef UINTN u_int;//unsigned int
typedef unsigned long u_long;
typedef unsigned long long bigtime_t;
typedef unsigned long int	uintptr_t;
typedef uintptr_t vaddr_t;
#define TIME_GTE(a, b) ((long)((a) - (b)) >= 0)
#define TIME_LTE(a, b) ((long)((a) - (b)) <= 0)
#define TIME_GT(a, b) ((long)((a) - (b)) > 0)
#define TIME_LT(a, b) ((long)((a) - (b)) < 0)
