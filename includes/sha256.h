#ifndef SHA256_H
# define SHA256_H

typedef struct s_sha256
{
	unsigned int		state[8];
	unsigned long long	bitcount;
	unsigned char		buffer[64];
}						t_sha256;

# define INIT_DATA_A (unsigned int)0x6a09e667
# define INIT_DATA_B (unsigned int)0xbb67ae85
# define INIT_DATA_C (unsigned int)0x3c6ef372
# define INIT_DATA_D (unsigned int)0xa54ff53a
# define INIT_DATA_E (unsigned int)0x510e527f
# define INIT_DATA_F (unsigned int)0x9b05688c
# define INIT_DATA_G (unsigned int)0x1f83d9ab
# define INIT_DATA_H (unsigned int)0x5be0cd19

# define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))

# define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
# define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

# define BS1G0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
# define BS1G1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
# define SSIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
# define SSIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

#endif
