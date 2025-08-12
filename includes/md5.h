#ifndef MD5_H
# define MD5_H

typedef struct s_md5
{
	unsigned int		state[4];
	unsigned long long	bitcount;
	unsigned char		buffer[64];
}						t_md5;

# define INIT_DATA_A (unsigned long)0x67452301L
# define INIT_DATA_B (unsigned long)0xefcdab89L
# define INIT_DATA_C (unsigned long)0x98badcfeL
# define INIT_DATA_D (unsigned long)0x10325476L

# define S11 7
# define S12 12
# define S13 17
# define S14 22
# define S21 5
# define S22 9
# define S23 14
# define S24 20
# define S31 4
# define S32 11
# define S33 16
# define S34 23
# define S41 6
# define S42 10
# define S43 15
# define S44 21

# define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
# define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
# define H(x, y, z) ((x) ^ (y) ^ (z))
# define I(x, y, z) ((y) ^ ((x) | (~z)))

# define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

# define FF(a, b, c, d, x, s, ac)                            \
	{                                                       \
		(a) += F((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT((a), (s));                        \
		(a) += (b);                                         \
	}

# define GG(a, b, c, d, x, s, ac)                            \
	{                                                       \
		(a) += G((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT((a), (s));                        \
		(a) += (b);                                         \
	}

# define HH(a, b, c, d, x, s, ac)                            \
	{                                                       \
		(a) += H((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT((a), (s));                        \
		(a) += (b);                                         \
	}

# define II(a, b, c, d, x, s, ac)                            \
	{                                                       \
		(a) += I((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT((a), (s));                        \
		(a) += (b);                                         \
	}

#endif
