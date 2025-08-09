#include "../includes/ft_ssl.h"

// TODO: attention au -p strings
int	parse_flags(t_input *input, int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] && ft_strcmp(argv[i], "--") != 0)
		{
			j = 1;
			while (argv[i][j])
			{
				if (argv[i][j] == 'p')
					input->flags.p = 1;
				else if (argv[i][j] == 'q')
					input->flags.q = 1;
				else if (argv[i][j] == 'r')
					input->flags.r = 1;
				else if (argv[i][j] == 's')
				{
					input->flags.s = 1;
					if (argv[i][j + 1] == '\0')
					{
						// argument in next argv
						if (i + 1 >= argc)
						{
							ft_putstr_fd("ft_ssl: option requires an argument -- s\n",
											2);
							return (-1);
						}
						input->str_arg = argv[i + 1];
						i++; // consume the next argument
					}
					else
					{
						// inline argument like: -s"hello"
						input->str_arg = &argv[i][j + 1];
					}
					break ;
				}
				else
				{
					print_invalid_option(argv[i][j]);
					print_usage();
					return (-1);
				}
				j++;
			}
			i++;
		}
		else
			break ;
	}
	input->arg_i = i;
	return (0);
}
