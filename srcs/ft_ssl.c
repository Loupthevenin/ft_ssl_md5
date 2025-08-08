#include "../includes/ft_ssl.h"

t_command	*get_cmd(t_ssl_ctx *ctx, const char *cmd_name)
{
	size_t	i;

	i = 0;
	while (i < ctx->command_count)
	{
		if (ft_strcmp(cmd_name, ctx->commands[i].name) == 0)
			return (&ctx->commands[i]);
		i++;
	}
	return (NULL);
}

static void	init_ctx(t_ssl_ctx *ctx)
{
	ctx->command_count = 2;
	ctx->commands = malloc(sizeof(t_command) * ctx->command_count);
	if (!ctx->commands)
	{
		ft_putstr_fd("Memory allocation failed\n", 2);
		exit(EXIT_FAILURE);
	}
	ctx->commands[0].name = "md5";
	ctx->commands[0].func = &md5_main;
	ctx->commands[1].name = "sha256";
	ctx->commands[1].func = &sha256_main;
}

int	main(int argc, char **argv)
{
	t_ssl_ctx	ctx;
	t_command	*cmd;

	init_ctx(&ctx);
	if (argc < 2)
	{
		print_usage();
		return (1);
	}
	cmd = get_cmd(&ctx, argv[1]);
	if (!cmd)
	{
		print_invalid_cmd(argv[1]);
		print_usage();
		cleanup(&ctx);
		return (1);
	}
	// Appel de la command
	cmd->func(argc - 1, argv + 1);
	cleanup(&ctx);
	return (0);
}
