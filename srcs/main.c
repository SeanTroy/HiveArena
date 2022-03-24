/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plehtika <plehtika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 10:50:41 by plehtika          #+#    #+#             */
/*   Updated: 2022/03/24 18:19:50 by plehtika         ###   ########.fr       */
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

int	check_flower_location(agent_info_t info, int direction)
{
	coords_t	center = {info.row, info.col};
	coords_t	flower = direction_to_coords(center, direction);
	if (info.player == 0 && info.bee != 0
	&& (flower.row == 12 || flower.row == 13 || flower.row == 14) && flower.col == 3)
		return (-1);
	if (info.player == 1 && info.bee != 0
	&& (flower.row == 12 || flower.row == 13 || flower.row == 14) && flower.col == 26)
		return (-1);
	return (1);
}

int	find_neighbour_goal_spot(agent_info_t info, int map[25][30])
{
	coords_t	center = {info.row, info.col};

	for (int dir = 0; dir < 8; dir++)
	{
		coords_t	coords = direction_to_coords(center, dir);
		if (coords.row >= 0 && coords.row < 25 && coords.col >= 0
			&& coords.col < 30)
		{
			int	neighbour = map[coords.row][coords.col];
			if (neighbour == -2)
			{
				return (dir);
			}
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

int	is_target_near2(agent_info_t info, cell_t type, cell_t enemybee)
{
	int	i = 0;
	int j = 0;
	int	count = 0;
	int	beecount = 0;

	while (i < 7)
	{
		while (j < 7)
		{
			if (info.cells[i][j] == type)
				count++;
			if (info.cells[i][j] == enemybee)
				beecount++;
			j++;
		}
		j = 0;
		i++;
	}
	if (count > 2 || beecount > 0)
		return (1);
	return (-1);
}

coords_t	get_home_hive(agent_info_t info, int map[25][30])
{
	coords_t	hive;

	if (info.player == 0 && (info.turn < 150 || info.bee == 0))
	{
		hive.row = 13;
		hive.col = 2;
	}
	if (info.player == 0 && info.turn >= 150 && info.bee != 0)
	{
		hive.row = 14;
		hive.col = 3;
	}
	if (info.player == 0 && info.turn >= 400 && map[13][2] != (int)bee_cell(info.player, 0) && info.bee != 0)
	{
		hive.row = 13;
		hive.col = 2;
	}
	if (info.player == 1 && (info.turn < 150 || info.bee == 0))
	{
		hive.row = 13;
		hive.col = 27;
	}
	if (info.player == 1 && info.turn >= 150 && info.bee != 0)
	{
		hive.row = 14;
		hive.col = 26;
	}
	if (info.player == 1 && info.turn >= 400 && map[13][27] != (int)bee_cell(info.player, 0) && info.bee != 0)
	{
		hive.row = 13;
		hive.col = 27;
	}
	return (hive);
}

command_t	end_game_moves(agent_info_t info, int map[25][30])
{
	if (info.player == 0)
	{
		if (info.row == 14 && info.col == 1)
		{
			if (map[14][2] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = E
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = E
				};
		}
		if (info.row == 15 && info.col == 1)
		{
			if (map[14][2] == EMPTY)
			{
				return (command_t) {
					.action = MOVE,
					.direction = NE
				};
			}
			if (map[15][2] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = E
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = E
				};
		}
		if (info.row == 15 && info.col == 2)
		{
			if (map[14][3] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = NE
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = NE
				};
		}
		if (info.row == 14 && info.col == 2)
		{
			if (map[13][3] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = NE
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = NE
				};
		}
		if (info.row == 14 && info.col == 3)
		{
			if (map[13][3] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = N
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 13 && info.col == 3)
		{
			if (map[12][3] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = N
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 12 && info.col == 3)
		{
			if ((map[11][3] == BEE_0 || map[11][3] == BEE_1
			|| map[11][3] == BEE_0_WITH_FLOWER
			|| map[11][3] == BEE_1_WITH_FLOWER)
			&& map[11][2] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = NW
				};
			}
			if (map[11][2] == EMPTY)
			{
				return (command_t) {
					.action = MOVE,
					.direction = NW
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 11 && info.col == 3)
		{
			if ((map[10][3] == BEE_0 || map[10][3] == BEE_1
			|| map[10][3] == BEE_0_WITH_FLOWER
			|| map[10][3] == BEE_1_WITH_FLOWER)
			&& map[11][2] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = W
				};
			}
			if (map[11][2] == EMPTY)
			{
				return (command_t) {
					.action = MOVE,
					.direction = W
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 10 && info.col == 3)
		{
			if (map[11][2] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = SW
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = SW
				};
		}
	}
	if (info.player == 1)
	{
		if (info.row == 14 && info.col == 28)
		{
			if (map[14][27] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = W
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = W
				};
		}
		if (info.row == 15 && info.col == 28)
		{
			if (map[14][27] == EMPTY)
			{
				return (command_t) {
					.action = MOVE,
					.direction = NW
				};
			}
			if (map[15][27] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = W
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = W
				};
		}
		if (info.row == 14 && info.col == 27)
		{
			if (map[13][26] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = NW
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = NW
				};
		}
		if (info.row == 15 && info.col == 27)
		{
			if (map[14][26] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = NW
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = NW
				};
		}
		if (info.row == 14 && info.col == 26)
		{
			if (map[13][26] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = N
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 13 && info.col == 26)
		{
			if (map[12][26] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = N
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 12 && info.col == 26)
		{
			if ((map[11][26] == BEE_0 || map[11][26] == BEE_1
			|| map[11][26] == BEE_0_WITH_FLOWER
			|| map[11][26] == BEE_1_WITH_FLOWER)
			&& map[11][27] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = NE
				};
			}
			if (map[11][27] == EMPTY)
			{
				return (command_t) {
					.action = MOVE,
					.direction = NE
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 11 && info.col == 26)
		{
			if ((map[10][26] == BEE_0 || map[10][26] == BEE_1
			|| map[10][26] == BEE_0_WITH_FLOWER
			|| map[10][26] == BEE_1_WITH_FLOWER)
			&& map[11][27] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = E
				};
			}
			if (map[11][27] == EMPTY)
			{
				return (command_t) {
					.action = MOVE,
					.direction = E
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = N
				};
		}
		if (info.row == 10 && info.col == 26)
		{
			if (map[11][27] == WALL)
			{
				return (command_t) {
					.action = GUARD,
					.direction = SE
				};
			}
			return (command_t) {
				.action = MOVE,
				.direction = SE
				};
		}
	}
	return (command_t) {
		.action = GUARD,
		.direction = -1
	};
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
		// command_t	end_game = end_game_moves(info, map);
		// if ((int)end_game.direction != -1)
		// 	return (end_game);
		coords_t	home_hive = get_home_hive(info, map);
		int	goal_spot = find_neighbour_goal_spot(info, map);
		if (info.bee != 0 && goal_spot != -1)
		{
			return (command_t) {
				.action = FORAGE,
				.direction = goal_spot
			};
		}
		// int opponent = info.player ^ 1;
		hive_dir = direction_to_target(info, home_hive);
		int	wall_dir = find_neighbour(info, WALL);
		if (wall_dir >= 0 && are_you_stuck(info, hive_dir) == -1)
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
	else if (info.turn > 70 || info.bee == 0)
	{
		int	flower_dir = find_neighbour(info, FLOWER);
		if (flower_dir >= 0 && check_flower_location(info, flower_dir) != -1)
		{
			// if (info.bee == 0 || info.bee == 2 || info.bee == 4)
				return (command_t) {
					.action = FORAGE,
					.direction = flower_dir
				};
			// else if (info.turn > 250) /* && if bee is builder bee */
			// 	return (command_t) {
			// 		.action = GUARD,
			// 		.direction = flower_dir
			// 	};
		}
		// int	wall_dir = find_neighbour(info, WALL);
		// if (wall_dir >= 0 && is_target_near(info, hive_cell(info.player)) == 1)
		// {
		// 	return (command_t) {
		// 		.action = GUARD,
		// 		.direction = wall_dir
		// 	};
		// }
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
	// if (info.bee == 1 || info.bee == 3)
	// 	return (builder_bee(info, map));
		// return (destroyer_bee(info, map));
	// else
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
