#include "../includes/ft_ssl.h"

int	process_input(t_input *input, int argc, char **argv, t_hash_func hash)
{
	int	i;

	(void)argv;
	(void)hash;
	// Si flag -p
	if (input->flags.p)
	{
	}
	// Si flag -s, hash la string
	if (input->flags.s)
	{
	}
	// fichier restant
	i = input->arg_i;
	while (i < argc)
	{
		i++;
	}
	// pas de flags pas de fichiers -> stdin
	if (!input->flags.p && !input->flags.s && input->arg_i == argc)
	{
	}
	return (EXIT_SUCCESS);
}
