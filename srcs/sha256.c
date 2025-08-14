#include "../includes/ft_ssl.h"
#include "../includes/sha256.h"

/**
 * Retourne un pointeur vers la table constante K utilisée dans SHA-256.

	* Cette table contient 64 constantes de 32 bits dérivées des racines cubiques des 64 premiers nombres premiers.

	* Ces constantes sont utilisées à chaque étape de la transformation pour introduire de la non-linéarité.
 */
static const unsigned int	*sha256_get_k(void)
{
	static const unsigned int K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
			0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be,
			0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f,
			0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d,
			0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351,
			0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
			0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1,
			0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624,
			0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c,
			0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa,
			0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
	return (K);
}

/**
 * Initialise une structure t_sha256 pour le calcul d'un hash.

	* Les valeurs initiales sont les constantes standard définies par le SHA-256 (RFC 6234).
 * On initialise également le compteur de bits et le tampon de données.
 */
static void	sha256_init(t_sha256 *sha256)
{
	sha256->state[0] = INIT_DATA_A;
	sha256->state[1] = INIT_DATA_B;
	sha256->state[2] = INIT_DATA_C;
	sha256->state[3] = INIT_DATA_D;
	sha256->state[4] = INIT_DATA_E;
	sha256->state[5] = INIT_DATA_F;
	sha256->state[6] = INIT_DATA_G;
	sha256->state[7] = INIT_DATA_H;
	sha256->bitcount = 0;
	ft_memset(sha256->buffer, 0, 64);
}

/**

	* Transforme un bloc de 512 bits (64 octets) en mettant à jour l'état de hachage.
 * - Remplit d'abord le tableau de mots w[64] à partir des données d'entrée.
 * - Applique les fonctions de mélange et rotations spécifiques SHA-256.
 * - Met à jour les 8 variables de l'état (a-h) pour ce bloc.
 */
static void	sha256_transform(t_sha256 *sha256, const unsigned char data[64])
{
	unsigned int		w[64];
	int					i;
	const unsigned int	*k;

	unsigned int a, b, c, d, e, f, g, h, t1, t2;
	i = 0;
	k = sha256_get_k();
	// Remplissage des 16 premiers mots avec les données brutes
	while (i < 16)
	{
		w[i] = data[i * 4] << 24 | (data[i * 4 + 1] << 16) | (data[i * 4
				+ 2] << 8) | (data[i * 4 + 3]);
		i++;
	}
	i = 16;
	// Extension des 16 mots en 64 mots avec les fonctions de mélange SHA-256
	while (i < 64)
	{
		w[i] = SSIG1(w[i - 2]) + w[i - 7] + SSIG0(w[i - 15]) + w[i - 16];
		i++;
	}
	a = sha256->state[0];
	b = sha256->state[1];
	c = sha256->state[2];
	d = sha256->state[3];
	e = sha256->state[4];
	f = sha256->state[5];
	g = sha256->state[6];
	h = sha256->state[7];
	i = 0;
	while (i < 64)
	{
		t1 = h + BS1G1(e) + CH(e, f, g) + k[i] + w[i];
		t2 = BS1G0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
		i++;
	}
	sha256->state[0] += a;
	sha256->state[1] += b;
	sha256->state[2] += c;
	sha256->state[3] += d;
	sha256->state[4] += e;
	sha256->state[5] += f;
	sha256->state[6] += g;
	sha256->state[7] += h;
}

/**
 * Met à jour le contexte SHA-256 avec de nouvelles données.
 * - Concatène les données entrantes au tampon existant.
 * - Dès qu'un bloc complet de 64 octets est disponible,
	appelle sha256_transform.
 * - Gère le reste des données qui ne constitue pas un bloc complet.
 */
static void	sha256_update(t_sha256 *sha256, const unsigned char *data,
		size_t len)
{
	size_t	i;
	size_t	index;
	size_t	to_fill;

	i = 0;
	index = (sha256->bitcount / 8) % 64;
	sha256->bitcount += (unsigned long long)len * 8;
	if (index > 0)
	{
		to_fill = 64 - index;
		if (len < to_fill)
			to_fill = len;
		ft_memcpy(&sha256->buffer[index], data, to_fill);
		index += to_fill;
		i += to_fill;
		if (index == 64)
		{
			sha256_transform(sha256, sha256->buffer);
			index = 0;
		}
	}
	while (i + 64 <= len)
	{
		sha256_transform(sha256, &data[i]);
		i += 64;
	}
	if (i < len)
		ft_memcpy(sha256->buffer, &data[i], len - i);
}

/**
 * Finalise le calcul SHA-256.
 * - Ajoute le padding nécessaire selon RFC 6234.
 * - Ajoute la longueur du message en bits.
 * - Convertit l'état final en tableau d'octets digest.
 */
static void	sha256_final(unsigned char digest[32], t_sha256 *sha256)
{
	unsigned char	padding[64];
	unsigned char	bit_len[8];
	unsigned int	index;
	unsigned int	pad_len;
	int				i;

	i = 0;
	while (i < 8)
	{
		bit_len[7 - i] = (sha256->bitcount >> (i * 8)) & 0xFF;
		i++;
	}
	index = (sha256->bitcount / 8) % 64;
	pad_len = (index < 56) ? (56 - index) : (120 - index);
	ft_memset(padding, 0, sizeof(padding));
	padding[0] = 0x80;
	sha256_update(sha256, padding, pad_len);
	sha256_update(sha256, bit_len, 8);
	i = 0;
	while (i < 8)
	{
		digest[i * 4] = (sha256->state[i] >> 24) & 0xFF;
		digest[i * 4 + 1] = (sha256->state[i] >> 16) & 0xFF;
		digest[i * 4 + 2] = (sha256->state[i] >> 8) & 0xFF;
		digest[i * 4 + 3] = sha256->state[i] & 0xFF;
		i++;
	}
}

// RFC 6234
char	*sha256_string(const char *input)
{
	t_sha256		sha256;
	unsigned char	digest[32];
	char			*result;
	const char		*hex;
	size_t			i;

	hex = "0123456789abcdef";
	sha256_init(&sha256);
	sha256_update(&sha256, (const unsigned char *)input, ft_strlen(input));
	sha256_final(digest, &sha256);
	result = malloc(65);
	if (!result)
		return (NULL);
	i = 0;
	while (i < 32)
	{
		result[i * 2] = hex[(digest[i] >> 4) & 0xF];
		result[i * 2 + 1] = hex[digest[i] & 0xF];
		i++;
	}
	result[64] = '\0';
	return (result);
}

int	sha256_main(int argc, char **argv)
{
	t_input	input;

	ft_memset(&input, 0, sizeof(t_input));
	input.cmd_name = "sha256";
	if (parse_flags(&input, argc, argv) == -1)
		return (EXIT_FAILURE);
	return (process_input(&input, argc, argv, &sha256_string));
}
