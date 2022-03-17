/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plehtika <plehtika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 11:03:18 by plehtika          #+#    #+#             */
/*   Updated: 2022/03/17 11:05:28 by plehtika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/deep_blue_c.h"

void	initialize_map(int map[25][30])
{
	int	i = 0;
	int	j = 0;

	while (i < 25)
	{
		while (j < 30)
		{
			map[i][j++] = OUTSIDE;
		}
		j = 0;
		i++;
	}
}

void	mask_opponent_hive(int map[25][30], agent_info_t info)
{
	if (info.player == 0)
	{
		if (map[11][27] == EMPTY || map[11][27] == OUTSIDE)
			map[11][27] = WALLSPOT;
		if (map[11][28] == EMPTY || map[11][28] == OUTSIDE)
			map[11][28] = WALLSPOT;
		if (map[11][29] == EMPTY || map[11][29] == OUTSIDE)
			map[11][29] = WALLSPOT;
		if (map[12][27] == EMPTY || map[12][27] == OUTSIDE)
			map[12][27] = WALLSPOT;
		if (map[13][27] == EMPTY || map[13][27] == OUTSIDE)
			map[13][27] = WALLSPOT;
		if (map[13][28] == EMPTY || map[13][28] == OUTSIDE)
			map[13][28] = WALLSPOT;
		if (map[13][29] == EMPTY || map[13][29] == OUTSIDE)
			map[13][29] = WALLSPOT;
		return;
	}
	else /* if (info.player == 1) */
	{
		if (map[11][0] == EMPTY || map[11][0] == OUTSIDE)
			map[11][0] = WALLSPOT;
		if (map[11][1] == EMPTY || map[11][1] == OUTSIDE)
			map[11][1] = WALLSPOT;
		if (map[11][2] == EMPTY || map[11][2] == OUTSIDE)
			map[11][2] = WALLSPOT;
		if (map[12][2] == EMPTY || map[12][2] == OUTSIDE)
			map[12][2] = WALLSPOT;
		if (map[13][0] == EMPTY || map[13][0] == OUTSIDE)
			map[13][0] = WALLSPOT;
		if (map[13][1] == EMPTY || map[13][1] == OUTSIDE)
			map[13][1] = WALLSPOT;
		if (map[13][2] == EMPTY || map[13][2] == OUTSIDE)
			map[13][2] = WALLSPOT;
		return;
	}
}

void	refresh_map(int map[25][30], agent_info_t info)
{
	int	deltarow = -3;
	int	deltacol = -3;
	int	i = 0;
	int	j = 0;

	while (deltarow < 4)
	{
		while (deltacol < 4)
		{
			if (info.cells[i][j] != OUTSIDE) {
				map[info.row + deltarow][info.col + deltacol] = info.cells[i][j];
			}
			j++;
			deltacol++;
		}
		j = 0;
		deltacol = -3;
		i++;
		deltarow++;
	}
	mask_opponent_hive(map, info);
}
