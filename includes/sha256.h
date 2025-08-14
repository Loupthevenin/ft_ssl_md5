#ifndef SHA256_H
# define SHA256_H

typedef struct s_sha256
{
	unsigned int state[8];       // 8 registres A-H
	unsigned long long bitcount; // compteur de bits total
	unsigned char buffer[64];    // bloc de 512 bits
}	t_sha256;

// Ces constantes -> racine carré des 8 premiers nombres premiers
# define INIT_DATA_A (unsigned int)0x6a09e667
# define INIT_DATA_B (unsigned int)0xbb67ae85
# define INIT_DATA_C (unsigned int)0x3c6ef372
# define INIT_DATA_D (unsigned int)0xa54ff53a
# define INIT_DATA_E (unsigned int)0x510e527f
# define INIT_DATA_F (unsigned int)0x9b05688c
# define INIT_DATA_G (unsigned int)0x1f83d9ab
# define INIT_DATA_H (unsigned int)0x5be0cd19

// Rotation circulaire à droite.
# define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))
/*
Exemple : word = 0b10010011, bits = 3
ROTRIGHT(word, 3) déplace les 3 derniers bits à gauche :
Résultat = 0b01110010
Rôle : mélange les bits pour diffusion non linéaire
*/

# define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
/*
Exemple : x=1, y=0, z=1 (bit par bit)
Si x=1 → prend y (0)
Si x=0 → prend z (1)
Rôle : introduit de la non-linéarité dans la boucle de compression
*/

# define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
/*
Exemple : x=1, y=0, z=1 (bit par bit)
Résultat = 1 si au moins deux bits sont 1
Rôle : ajoute un effet de majorité, renforce la diffusion
*/

# define BS1G0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
# define BS1G1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
/*
Exemple : x = 0b101010... (32 bits)
BS1G0 combine 3 rotations différentes → mélange fort
BS1G1 idem mais avec autres rotations
Rôle : transformation non linéaire des variables de l'état (a..h)
*/
# define SSIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
# define SSIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))
/*
Exemple : x = 0b101010... (32 bits)
SSIG0 : rotation + décalage logique → étend w[i-15]
SSIG1 : rotation + décalage logique → étend w[i-2]
Rôle : préparer les 64 mots de message pseudo-aléatoires
*/

#endif
