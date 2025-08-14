#ifndef MD5_H
# define MD5_H

typedef struct s_md5
{
	unsigned int state[4];       // 4 registres 32 bits (ABCD)
	unsigned long long bitcount; // nombre total de bits déjà absorbés
	unsigned char buffer[64];    // tampon pour accumuler les données
}	t_md5;

# define INIT_DATA_A (unsigned long)0x67452301L
# define INIT_DATA_B (unsigned long)0xefcdab89L
# define INIT_DATA_C (unsigned long)0x98badcfeL
# define INIT_DATA_D (unsigned long)0x10325476L

// --- Décalages de rotation pour chaque étape ---
// Utilisés dans les opérations FF, GG, HH, II
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

// --- Fonctions non-linéaires pour chaque round ---
// Elles combinent les bits de 3 variables pour diffuser l'information
# define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
# define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
# define H(x, y, z) ((x) ^ (y) ^ (z))
# define I(x, y, z) ((y) ^ ((x) | (~z)))

// --- Rotation circulaire à gauche sur 32 bits ---
// Exemple : ROTATE_LEFT(0b1011, 2)
//	-> 0b1110 (les bits sortants reviennent à droite)
# define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// FF : Round 1
// a = a + F(b,c,d) + x + ac
// Puis rotation circulaire à gauche de s bits et addition de b
# define FF(a, b, c, d, x, s, ac)                                   \
	{                                                              \
		/* Étape 1 : mélange non-linéaire*/                        \
		(a) += F((b), (c), (d)) + (x) + (unsigned int)(ac);        \
		/* Étape 2 : ajout du mot du message et de la constante */ \
		(a) = ROTATE_LEFT((a), (s));                               \
		(a) += (b);                                                \
	}

// GG : Round 2
// a = a + G(b,c,d) + x + ac, rotation gauche, +b
// G(b,c,d) = (b & d) | (c & ~d)
# define GG(a, b, c, d, x, s, ac)                            \
	{                                                       \
		(a) += G((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT((a), (s));                        \
		(a) += (b);                                         \
	}

// HH : Round 3
// a = a + H(b,c,d) + x + ac, rotation gauche, +b
// H(b,c,d) = b ^ c ^ d (XOR des 3)
# define HH(a, b, c, d, x, s, ac)                            \
	{                                                       \
		(a) += H((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT((a), (s));                        \
		(a) += (b);                                         \
	}

// II : Round 4
// a = a + I(b,c,d) + x + ac, rotation gauche, +b
// I(b,c,d) = c ^ (b | ~d)
# define II(a, b, c, d, x, s, ac)                            \
	{                                                       \
		(a) += I((b), (c), (d)) + (x) + (unsigned int)(ac); \
		(a) = ROTATE_LEFT((a), (s));                        \
		(a) += (b);                                         \
	}

#endif
