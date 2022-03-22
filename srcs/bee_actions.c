/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bee_actions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plehtika <plehtika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 11:06:54 by plehtika          #+#    #+#             */
/*   Updated: 2022/03/22 14:43:29 by plehtika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/deep_blue_c.h"

int	find_neighbour_wall_spot(agent_info_t info, int map[25][30])
{
	coords_t	center = {info.row, info.col};

	for (int dir = 0; dir < 8; dir++)
	{
		coords_t	coords = direction_to_coords(center, dir);
		if (coords.row >= 0 && coords.row < 25 && coords.col >= 0
			&& coords.col < 30)
		{
			int	neighbour = map[coords.row][coords.col];
			if (neighbour == WALLSPOT)
			{
				return (dir);
			}
		}
	}
	return (-1);
}

int	check_builder_side(agent_info_t info, int row)
{
	if (info.bee == 1 && (row == 11 || row == 12))
		return (1);
	if (info.bee == 3 && (row == 12 || row == 13))
		return (1);
	return (0);
}

coords_t	find_builder_spot(agent_info_t info)
{
	if (info.player == 0)
	{
		if (info.bee == 1)
		{
			if (info.row == 11 && info.col == 28)
				return (coords_t) {
					.row = -1,
					.col = -1
			};
			return (coords_t) {
				.row = 11,
				.col = 28
			};
		}
		if (info.bee == 3)
		{
			if (info.row == 13 && info.col == 28)
				return (coords_t) {
					.row = -1,
					.col = -1
			};
			return (coords_t) {
				.row = 13,
				.col = 28
			};
		}
	}
	if (info.player == 1)
	{
		if (info.bee == 1)
		{
			if (info.row == 11 && info.col == 1)
				return (coords_t) {
					.row = -1,
					.col = -1
			};
			return (coords_t) {
				.row = 11,
				.col = 1
			};
		}
		if (info.bee == 3)
		{
			if (info.row == 13 && info.col == 1)
				return (coords_t) {
					.row = -1,
					.col = -1
			};
			return (coords_t) {
				.row = 13,
				.col = 1
			};
		}
	}
	return (coords_t) {
		.row = -1,
		.col = -1
	};
}

coords_t	find_closest_wall_spot(agent_info_t info, int map[25][30])
{
	static target_t	reserved;
	static int		check = -1;
	int				distance = 2;
	int				i = -2;
	int 			j = -2;

	check++;
	if (check == 0) /* Initialize struct reserved */
	{
		i = 0;
		while (i < 5)
		{
			reserved.row[i] = -1;
			reserved.col[i] = -1;
			i++;
		}
		i = -2;
	}
	while (distance < 15)
	{
		while (i <= distance)
		{
			while (j <= distance)
			{
				if (info.row + i >= 0 && info.row + i < 25
				&& info.col + j >= 0 && info.col + j < 30
				&& map[info.row + i][info.col + j] == WALLSPOT
				&& check_builder_side(info, info.row + i) == 1
				&& is_target_reserved(info.row + i, info.col + j, info, &reserved) == 0)
					return (coords_t) {
						.row = info.row + i,
						.col = info.col + j
					};
				j++;
			}
			j = -distance;
			i++;
		}
		distance++;
		j = -distance;
		i = -distance;
	}
	int opponent = info.player ^ 1;
	int	flowerbee_dir = find_neighbour(info, bee_cell(opponent, 1));
	if (flowerbee_dir >= 0)
		return (coords_t) {
			.row = -1,
			.col = -1
		};
	if (info.bee == 1)
	{
		if (info.player == 0)
			return (coords_t) {
		.row = 10,
		.col = 26
		};
		if (info.player == 1)
			return (coords_t) {
		.row = 10,
		.col = 3
		};
	}
	if (info.bee == 3)
	{
		if (info.player == 0)
			return (coords_t) {
		.row = 14,
		.col = 26
		};
		if (info.player == 1)
			return (coords_t) {
		.row = 14,
		.col = 3
		};
	}
	return (coords_t) {
		.row = rand () % 24,
		.col = rand () % 29
	};
}

int	check_empty_direction(agent_info_t info, int direction)
{
	coords_t	center = {VIEW_DISTANCE, VIEW_DISTANCE};
	coords_t	target;
	int			new_direction;
	int			range;

	target = direction_to_coords(center, direction);
	new_direction = direction;
	range = 1;
	while (info.cells[target.row][target.col] != EMPTY && range < 5)
	{
		new_direction = direction - range; /* for example E -> NE */
		if (new_direction < 0)
			new_direction = 7; /* NW */
		target = direction_to_coords(center, new_direction);
		if (info.cells[target.row][target.col] == EMPTY)
			break;
		else
		{
			new_direction = direction + range; /* for example E -> SE */
			if (new_direction > 7)
				new_direction = 0; /* N */
			target = direction_to_coords(center, new_direction);
		}
		range++;
	}
	return (new_direction);
}

int	direction_to_target(agent_info_t info, coords_t target)
{
	if (info.row < target.row && info.col < target.col)
		return (SE);
	if (info.row < target.row && info.col == target.col)
		return (S);
	if (info.row < target.row && info.col > target.col)
		return (SW);
	if (info.row == target.row && info.col > target.col)
		return (W);
	if (info.row > target.row && info.col > target.col)
		return (NW);
	if (info.row > target.row && info.col == target.col)
		return (N);
	if (info.row > target.row && info.col < target.col)
		return (NE);
	if (info.row == target.row && info.col < target.col)
		return (E);
	return (rand() % 8);
}

command_t	forager_bee(agent_info_t info, int map[25][30])
{
	coords_t	flower;

	if (info.bee == 0)
	{
		if (info.player == 0)
		{
			if (info.turn < 40)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NE)
				};
			if (info.turn < 70)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NW)
				};
		}
		if (info.player == 1)
		{
			if (info.turn < 40)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NW)
				};
			if (info.turn < 70)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NE)
				};
		}
	}
	if (info.bee == 2)
	{
		if (info.player == 0)
		{
			if (info.turn < 70)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, E)
				};
		}
		if (info.player == 1)
		{
			if (info.turn < 70)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, W)
				};
		}
	}
	if (info.bee == 4)
	{
		if (info.player == 0)
		{
			if (info.turn < 40)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SE)
				};
			if (info.turn < 70)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SW)
				};
		}
		if (info.player == 1)
		{
			if (info.turn < 40)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SW)
				};
			if (info.turn < 70)
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SE)
				};
		}
	}
	flower = find_closest_flower(info, map);
	int opponent = info.player ^ 1;
	int	flower_dir = direction_to_target(info, flower);
	int	wall_dir = find_neighbour(info, WALL);
	if (wall_dir >= 0 && (wall_dir == flower_dir || flower_dir == find_neighbour(info, hive_cell(info.player))
	|| flower_dir == find_neighbour(info, bee_cell(opponent, 0)))) /*DO WE NEED TO CHECK FOR EMPTY ROUTE?*/
		// && is_target_near(info, hive_cell(info.player)) == 1)
	{
		return (command_t) {
			.action = GUARD,
			.direction = wall_dir
		};
	}
	return (command_t) {
		.action = MOVE,
		.direction = check_empty_direction(info, direction_to_target(info, flower))
	};
}

command_t	destroyer_bee(agent_info_t info, int map[25][30])
{
	coords_t	flower;

	if (info.bee == 1)
	{
		if (info.player == 0)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 140)
				|| (info.turn > 170 && info.turn < 200))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NE)
				};
			if (info.turn < 110 || (info.turn > 140 && info.turn < 170)
				|| (info.turn > 200 && info.turn < 230))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SE)
				};
		}
		if (info.player == 1)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 140)
				|| (info.turn > 170 && info.turn < 200))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NW)
				};
			if (info.turn < 110 || (info.turn > 140 && info.turn < 170)
				|| (info.turn > 200 && info.turn < 230))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SW)
				};
		}
	}
	if (info.bee == 3)
	{
		if (info.player == 0)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 140)
				|| (info.turn > 170 && info.turn < 200))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SE)
				};
			if (info.turn < 110 || (info.turn > 140 && info.turn < 170)
				|| (info.turn > 200 && info.turn < 230))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NE)
				};
		}
		if (info.player == 1)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 140)
				|| (info.turn > 170 && info.turn < 200))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SW)
				};
			if (info.turn < 110 || (info.turn > 140 && info.turn < 170)
				|| (info.turn > 200 && info.turn < 230))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NW)
				};
		}
	}
	flower = find_closest_flower(info, map);
	return (command_t) {
		.action = MOVE,
		.direction = check_empty_direction(info, direction_to_target(info, flower))
	};
}

command_t	builder_bee(agent_info_t info, int map[25][30])
{
	coords_t	builder_spot;

	if (info.bee == 1)
	{
		if (info.player == 0)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 170))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NE)
				};
			if (info.turn < 110 || (info.turn > 170 && info.turn < 250))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SE)
				};
		}
		if (info.player == 1)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 170))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NW)
				};
			if (info.turn < 110 || (info.turn > 170 && info.turn < 250))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SW)
				};
		}
	}
	if (info.bee == 3)
	{
		if (info.player == 0)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 170))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SE)
				};
			if (info.turn < 110 || (info.turn > 170 && info.turn < 250))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NE)
				};
		}
		if (info.player == 1)
		{
			if (info.turn < 80 || (info.turn > 110 && info.turn < 170))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, SW)
				};
			if (info.turn < 110 || (info.turn > 170 && info.turn < 250))
				return (command_t) {
					.action = MOVE,
					.direction = check_empty_direction(info, NW)
				};
		}
	}
	builder_spot = find_builder_spot(info);
	if (builder_spot.row != -1)
		return (command_t) {
		.action = MOVE,
		.direction = check_empty_direction(info, direction_to_target(info, builder_spot))
	};
	int spot_dir = find_neighbour_wall_spot(info, map);
	if (spot_dir >= 0)
		return (command_t) {
		.action = BUILD,
		.direction = spot_dir
	};
	int opponent = info.player ^ 1;
	int	enemy_bee = find_neighbour(info, bee_cell(opponent, 1));
	if (enemy_bee >= 0)
		return (command_t) {
		.action = GUARD,
		.direction = enemy_bee
	};
	return (command_t) {
	.action = MOVE,
	.direction = find_neighbour(info, hive_cell(opponent))
	};
}
