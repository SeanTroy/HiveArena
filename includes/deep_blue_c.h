/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deep_blue_c.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plehtika <plehtika@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 08:59:32 by plehtika          #+#    #+#             */
/*   Updated: 2022/03/24 17:56:42 by plehtika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEEP_BLUE_C_H
# define DEEP_BLUE_C_H

# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "agent.h"
# include <fcntl.h>

# define WALLSPOT -1

typedef struct target_s
{
	int	row[5];
	int	col[5];
}	target_t;

void		initialize_map(int map[25][30]);
void		refresh_map(int map[25][30], agent_info_t info);
command_t	forager_bee(agent_info_t info, int map[25][30]);
command_t	destroyer_bee(agent_info_t info, int map[25][30]);
command_t	builder_bee(agent_info_t info, int map[25][30]);
coords_t	find_closest_flower(agent_info_t info, int map[25][30]);
coords_t	find_closest_wall_spot(agent_info_t info, int map[25][30]);
int			direction_to_target(agent_info_t info, coords_t target);
int			check_empty_direction(agent_info_t info, int direction);
int			is_target_reserved(int target_row, int target_col, agent_info_t info, target_t *reserved);
int			find_neighbour(agent_info_t info, cell_t type);
int			is_target_near(agent_info_t info, cell_t type);
int			is_target_near2(agent_info_t info, cell_t type, cell_t enemybee);
int			are_you_stuck(agent_info_t info, int direction);

#endif
