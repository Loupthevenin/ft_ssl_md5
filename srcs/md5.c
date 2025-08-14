#include "../includes/ft_ssl.h"
#include "../includes/md5.h"

/*
** decode :
** Convertit un bloc de 64 octets (512 bits) en un tableau de 16 entiers 32 bits.
** MD5 travaille en little-endian → le byte le moins significatif vient en premier.
** Exemple : octets [0x01, 0x02, 0x03, 0x04] → 0x04030201
*/
static void	decode(unsigned int *output, const unsigned char *input, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (j < len)
	{
		output[i] = ((unsigned int)input[j] |
						(unsigned int)input[j + 1] << 8 |
						(unsigned int)input[j + 2] << 16 |
						(unsigned int)input[j + 3] << 24);
		i++;
		j += 4;
	}
}

/*
** md5_init :
** Initialise le contexte MD5 avec :
** - bitcount = nombre total de bits traités (commence à 0)
** - state[] = valeurs initiales A, B, C, D (définies dans RFC 1321)
**   Pourquoi ces valeurs ? C’est un choix empirique des auteurs pour
**   obtenir une bonne diffusion/avalanche.
*/
static void	md5_init(t_md5 *md5)
{
	ft_memset(md5, 0, sizeof(*md5));
	md5->bitcount = 0;
	md5->state[0] = INIT_DATA_A;
	md5->state[1] = INIT_DATA_B;
	md5->state[2] = INIT_DATA_C;
	md5->state[3] = INIT_DATA_D;
}

/*
** md5_transform :
** Cœur de l’algorithme.
** - Entrée : un bloc de 64 octets (512 bits)
** - Les 512 bits sont vus comme 16 mots de 32 bits → x[0..15]
** - On fait 64 opérations, divisées en 4 "rounds" de 16 étapes chacun :
**   ROUND 1 : fonction F, mélange basique binaire
**   ROUND 2 : fonction G, permutation différente des mots
**   ROUND 3 : fonction H, XOR
**   ROUND 4 : fonction I, mélange final
**
** Pourquoi 64 opérations ?
**  → Chaque round applique une fonction non linéaire différente
**    sur une combinaison de A,B,C,D et d’un mot x[i] avec une constante T[i].
**    Cela crée une bonne avalanche : une modification minime change tout le digest.
*/
static void	md5_transform(unsigned int state[4], const unsigned char block[64])
{
	unsigned int	a;
	unsigned int	b;
	unsigned int	c;
	unsigned int	d;
	unsigned int	x[16];

	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	decode(x, block, 64);
	// ROUND 1
	FF(a, b, c, d, x[0], S11, 0xd76aa478);  /* 1 */
	FF(d, a, b, c, x[1], S12, 0xe8c7b756);  /* 2 */
	FF(c, d, a, b, x[2], S13, 0x242070db);  /* 3 */
	FF(b, c, d, a, x[3], S14, 0xc1bdceee);  /* 4 */
	FF(a, b, c, d, x[4], S11, 0xf57c0faf);  /* 5 */
	FF(d, a, b, c, x[5], S12, 0x4787c62a);  /* 6 */
	FF(c, d, a, b, x[6], S13, 0xa8304613);  /* 7 */
	FF(b, c, d, a, x[7], S14, 0xfd469501);  /* 8 */
	FF(a, b, c, d, x[8], S11, 0x698098d8);  /* 9 */
	FF(d, a, b, c, x[9], S12, 0x8b44f7af);  /* 10 */
	FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */
	// ROUND 2
	GG(a, b, c, d, x[1], S21, 0xf61e2562);  /* 17 */
	GG(d, a, b, c, x[6], S22, 0xc040b340);  /* 18 */
	GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);  /* 20 */
	GG(a, b, c, d, x[5], S21, 0xd62f105d);  /* 21 */
	GG(d, a, b, c, x[10], S22, 0x2441453);  /* 22 */
	GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);  /* 24 */
	GG(a, b, c, d, x[9], S21, 0x21e1cde6);  /* 25 */
	GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[3], S23, 0xf4d50d87);  /* 27 */
	GG(b, c, d, a, x[8], S24, 0x455a14ed);  /* 28 */
	GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[2], S22, 0xfcefa3f8);  /* 30 */
	GG(c, d, a, b, x[7], S23, 0x676f02d9);  /* 31 */
	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */
	// ROUND 3
	HH(a, b, c, d, x[5], S31, 0xfffa3942);  /* 33 */
	HH(d, a, b, c, x[8], S32, 0x8771f681);  /* 34 */
	HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], S31, 0xa4beea44);  /* 37 */
	HH(d, a, b, c, x[4], S32, 0x4bdecfa9);  /* 38 */
	HH(c, d, a, b, x[7], S33, 0xf6bb4b60);  /* 39 */
	HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[0], S32, 0xeaa127fa);  /* 42 */
	HH(c, d, a, b, x[3], S33, 0xd4ef3085);  /* 43 */
	HH(b, c, d, a, x[6], S34, 0x4881d05);   /* 44 */
	HH(a, b, c, d, x[9], S31, 0xd9d4d039);  /* 45 */
	HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], S34, 0xc4ac5665);  /* 48 */
	// ROUND 4
	II(a, b, c, d, x[0], S41, 0xf4292244);  /* 49 */
	II(d, a, b, c, x[7], S42, 0x432aff97);  /* 50 */
	II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], S44, 0xfc93a039);  /* 52 */
	II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[3], S42, 0x8f0ccc92);  /* 54 */
	II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], S44, 0x85845dd1);  /* 56 */
	II(a, b, c, d, x[8], S41, 0x6fa87e4f);  /* 57 */
	II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], S43, 0xa3014314);  /* 59 */
	II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], S41, 0xf7537e82);  /* 61 */
	II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], S43, 0x2ad7d2bb);  /* 63 */
	II(b, c, d, a, x[9], S44, 0xeb86d391);  /* 64 */
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

/*
** md5_update :
** Découpe l'entrée en blocs de 64 octets et appelle md5_transform pour chacun.
** Si le dernier bloc n’est pas complet, il est stocké dans md5->buffer.
*/
static void	md5_update(t_md5 *md5, const unsigned char *data, size_t len)
{
	size_t	i;
	size_t	buffer_index;
	size_t	space_in_buffer;

	i = 0;
	buffer_index = (md5->bitcount / 8) % 64;
	md5->bitcount += (unsigned long long)len * 8;
	space_in_buffer = 64 - buffer_index;
	if (len >= space_in_buffer)
	{
		ft_memcpy(&md5->buffer[buffer_index], data, space_in_buffer);
		md5_transform(md5->state, md5->buffer);
		i = space_in_buffer;
		while (i + 63 < len)
		{
			md5_transform(md5->state, &data[i]);
			i += 64;
		}
		buffer_index = 0;
	}
	ft_memcpy(&md5->buffer[buffer_index], &data[i], len - i);
}

/*
** md5_final :
** - Ajoute le bit '1' (0x80) puis des zéros jusqu'à ce que
**   la longueur soit congruente à 56 mod 64.
** - Ajoute la longueur totale en bits (8 octets, little-endian).
** - Transforme le dernier bloc.
** - Écrit le state final (A,B,C,D) dans digest[16] (toujours en little-endian).
*/
static void	md5_final(unsigned char digest[16], t_md5 *md5)
{
	unsigned char	padding[64];
	unsigned char	bits[8];
	unsigned int	index;
	unsigned int	pad_len;
	size_t			i;

	ft_memset(padding, 0, sizeof(padding));
	padding[0] = 0x80;
	i = 0;
	while (i < 8)
	{
		bits[i] = (unsigned char)((md5->bitcount >> (8 * i)) & 0xFF);
		i++;
	}
	index = (md5->bitcount / 8) % 64;
	pad_len = (index < 56) ? (56 - index) : (120 - index);
	md5_update(md5, padding, pad_len);
	md5_update(md5, bits, 8);
	i = 0;
	while (i < 4)
	{
		digest[i * 4] = (unsigned char)(md5->state[i] & 0xFF);
		digest[i * 4 + 1] = (unsigned char)((md5->state[i] >> 8) & 0xFF);
		digest[i * 4 + 2] = (unsigned char)((md5->state[i] >> 16) & 0xFF);
		digest[i * 4 + 3] = (unsigned char)((md5->state[i] >> 24) & 0xFF);
		i++;
	}
}

// RFC 1321
char	*md5_string(const char *input)
{
	t_md5			md5;
	unsigned char	digest[16];
	char			*result;
	size_t			i;
	const char		*hex;

	md5_init(&md5);
	md5_update(&md5, (const unsigned char *)input, ft_strlen(input));
	md5_final(digest, &md5);
	result = malloc(33);
	if (!result)
		return (NULL);
	i = 0;
	hex = "0123456789abcdef";
	while (i < 16)
	{
		// Chaque byte du digest (8 bits) doit être converti en 2 caractères hexadécimaux.
		// Exemple : digest[i] = 0xBC (binaire : 10111100)
		//
		// 1) (digest[i] >> 4) décale à droite de 4 bits -> 00001011 (0x0B)
		// 2) & 0xF garde seulement les 4 bits de poids fort (high nibble)
		// -> 0x0B
		// 3) hex[...] va chercher le caractère correspondant dans "0123456789abcdef"
		// -> 'b'
		result[i * 2] = hex[(digest[i] >> 4) & 0xF];
		// 1) digest[i] & 0xF garde uniquement les 4 bits de poids faible (low nibble)
		// -> 0x0C
		// 2) hex[...] va chercher le caractère correspondant -> 'c'
		result[i * 2 + 1] = hex[digest[i] & 0xF];
		i++;
	}
	result[32] = '\0';
	return (result);
}

int	md5_main(int argc, char **argv)
{
	t_input	input;

	ft_memset(&input, 0, sizeof(t_input));
	input.cmd_name = "md5";
	if (parse_flags(&input, argc, argv) == -1)
		return (EXIT_FAILURE);
	return (process_input(&input, argc, argv, &md5_string));
}
