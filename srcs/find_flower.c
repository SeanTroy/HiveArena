/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_flower.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plehtika <plehtika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 10:49:36 by plehtika          #+#    #+#             */
/*   Updated: 2022/03/23 17:33:33 by plehtika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/deep_blue_c.h"

int	is_target_reserved(int target_row, int target_col, agent_info_t info, target_t *reserved)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (target_row == reserved->row[i] && target_col == reserved->col[i]
			&& i != info.bee)
			return (1);
		i++;
	}
	if (info.bee != 0)
	{
		reserved->row[info.bee] = target_row;
		reserved->col[info.bee] = target_col;
	}
	return (0);
}

coords_t	find_opponent_hive(agent_info_t info)
{
	coords_t	hive;

	if (info.player == 1)
	{
		hive.row = 12;
		hive.col = 3;
	}
	if (info.player == 0)
	{
		hive.row = 12;
		hive.col = 26;
	}
	return (hive);
}

int	forbidden_flowers(agent_info_t info, int row, int col)
{
	if (info.player == 0)
	{
		if ((row == 10 || row == 11 || row == 12) && col == 3)
			return (-1);
	}
	if (info.player == 1)
	{
		if ((row == 10 || row == 11 || row == 12) && col == 26)
			return (-1);
	}
	return (1);
}

coords_t	find_closest_empty(agent_info_t info, int map[25][30], target_t	reserved)
{
	int	distance = 2;
	int	i = -2;
	int j = -2;

	while (distance < 30)
	{
		while (i <= distance)
		{
			while (j <= distance)
			{
				if (info.row + i >= 0 && info.row + i < 25
				&& info.col + j >= 0 && info.col + j < 30
				&& map[info.row + i][info.col + j] == OUTSIDE
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
	return (coords_t) {
		.row = -1,
		.col = -1 };
}

coords_t	find_closest_flower(agent_info_t info, int map[25][30])
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
	// if (reserved.row[info.bee] != -1 && reserved.col[info.bee] != -1)
	// {
	// 	if (map[reserved.row[info.bee]][reserved.col[info.bee]] == FLOWER
	// 		|| map[reserved.row[info.bee]][reserved.col[info.bee]] == OUTSIDE)
	// 	{
	// 		return (coords_t) {
	// 			.row = reserved.row[info.bee],
	// 			.col = reserved.col[info.bee]
	// 		};
	// 	}
	// 	else
	// 	{
	// 		reserved.row[info.bee] = -1;
	// 		reserved.col[info.bee] = -1;
	// 	}
	// }
	while (distance < 15)
	{
		while (i <= distance)
		{
			while (j <= distance)
			{
				if (info.row + i >= 0 && info.row + i < 25
				&& info.col + j >= 0 && info.col + j < 30
				&& forbidden_flowers(info, info.row + i, info.col + j) == 1
				&& map[info.row + i][info.col + j] == FLOWER
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
	/* if no flowers are found */
	coords_t	empty = find_closest_empty(info, map, reserved);
	if (empty.row != -1)
		return (empty);
	/* if there are no empty cells left and you are not carrying flower */
	cell_t	bee = info.cells[VIEW_DISTANCE][VIEW_DISTANCE];
	if (!is_bee_with_flower(bee))
		return (find_opponent_hive(info));
	return (coords_t) {
		.row = rand() % 24,
		.col = rand() % 29
	};
}
