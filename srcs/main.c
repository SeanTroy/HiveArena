/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plehtika <plehtika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 10:50:41 by plehtika          #+#    #+#             */
/*   Updated: 2022/03/16 13:16:06 by plehtika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/deep_blue_c.h"

int	find_neighbour(agent_info_t info, cell_t type)
{
	coords_t	center = {VIEW_DISTANCE, VIEW_DISTANCE};

	for (int dir = 0; dir < 8; dir++)
	{
		coords_t	coords = direction_to_coords(center, dir);
		cell_t		neighbour = info.cells[coords.row][coords.col];
		if (neighbour == type)
		{
			return (dir);
		}
	}
	return (-1);
}

int	is_target_near(agent_info_t info, cell_t type)
{
	int	i = 0;
	int j = 0;

	while (i < 7)
	{
		while (j < 7)
		{
			if (info.cells[i][j] == type)
				return (1);
			j++;
		}
		j = 0;
		i++;
	}
	return (-1);
}

coords_t	get_home_hive(agent_info_t info)
{
	coords_t	hive;

	if (info.player == 0)
	{
		hive.row = 12;
		hive.col = 1;
	}
	if (info.player == 1)
	{
		hive.row = 12;
		hive.col = 28;
	}
	return (hive);
}

command_t	think(agent_info_t info)
{
	cell_t		bee = info.cells[VIEW_DISTANCE][VIEW_DISTANCE];
	static int	map[25][30];
	static int	check = -1;

	check++;
	if (check == 0)
		initialize_map(map);
	refresh_map(map, info);
	if (is_bee_with_flower(bee))
	{
		int	hive_dir = find_neighbour(info, hive_cell(info.player));
		if (hive_dir >= 0)
		{
			return (command_t) {
				.action = FORAGE,
				.direction = hive_dir
			};
		}
		coords_t	home_hive = get_home_hive(info);
		hive_dir = direction_to_target(info, home_hive);
		int	wall_dir = find_neighbour(info, WALL);
		if (wall_dir >= 0 && wall_dir == hive_dir
			&& is_target_near(info, hive_cell(info.player)) == 1)
		{
			return (command_t) {
				.action = GUARD,
				.direction = wall_dir
			};
		}
		return (command_t) {
			.action = MOVE,
			.direction = check_empty_direction(info, hive_dir)
		};
	}
	else if (info.turn > 70)
	{
		int	flower_dir = find_neighbour(info, FLOWER);
		if (flower_dir >= 0)
		{
			if (info.bee == 0 || info.bee == 2 || info.bee == 4)
				return (command_t) {
					.action = FORAGE,
					.direction = flower_dir
				};
			else if (info.turn > 250) /* && if bee is builder bee */
				return (command_t) {
					.action = GUARD,
					.direction = flower_dir
				};
		}
		int	wall_dir = find_neighbour(info, WALL);
		if (wall_dir >= 0 && is_target_near(info, hive_cell(info.player)) == 1)
		{
			return (command_t) {
				.action = GUARD,
				.direction = wall_dir
			};
		}
		/* if you are close to opponent's hive and you see an opponent carrying flower,
		HIT THEM! (happens when there are no flowers or empty cells left */
		int opponent = info.player ^ 1;
		int	hive_dir = is_target_near(info, hive_cell(opponent));
		// int	hive_dir = find_neighbour(info, hive_cell(opponent));
		int	flowerbee_dir = find_neighbour(info, bee_cell(opponent, 1));
		if (hive_dir == 1 && flowerbee_dir >= 0
			&& (info.bee == 0 || info.bee == 2 || info.bee == 4))
		{
			return (command_t) {
				.action = GUARD,
				.direction = flowerbee_dir
			};
		}
	}
	if (info.bee == 1 || info.bee == 3)
		return (builder_bee(info, map));
		// return (destroyer_bee(info, map));
	else
		return (forager_bee(info, map));
}

int	main(int argc, char **argv)
{
	if (argc < 3)
		panic("Usage: ./agent arena_host arena_ip");

	srand(time(NULL) + getpid());

	char	*host = argv[1];
	int		port = atoi(argv[2]);
	char	*team_name = "Deep Blue C";

	agent_main(host, port, team_name, think);
}
