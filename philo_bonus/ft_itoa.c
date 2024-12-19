/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:11:56 by mmravec           #+#    #+#             */
/*   Updated: 2024/12/19 20:27:00 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	ft_rev_char_tab(char *tab, int size)
{
	char	temp;
	int		start;
	int		end;

	start = 0;
	end = size - 1;
	while (start < end)
	{
		temp = tab[start];
		tab[start] = tab[end];
		tab[end] = temp;
		start++;
		end--;
	}
}

static int	is_neg(int n)
{
	if (n < 0)
		return (1);
	else
		return (0);
}

static int	count_digits(int n)
{
	int		count;

	if (n == -2147483648)
		return (11);
	count = 0;
	if (n < 0)
	{
		count++;
		n = -n;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		i;
	int		temp;
	int		is_negative;

	is_negative = is_neg(n);
	i = 0;
	result = (char *)malloc((count_digits(n) + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (n == 0)
		result[i++] = '0';
	while (n != 0)
	{
		temp = n % 10;
		if (is_negative)
			temp *= (-1);
		result[i++] = temp + '0';
		n /= 10;
	}
	if (is_negative)
		result[i++] = '-';
	result[i] = '\0';
	ft_rev_char_tab(result, i);
	return (result);
}

void	ft_itoa_to_str(long num, char *str)
{
	int		i;
	int		is_negative;
	char	temp[32];

	is_negative = (num < 0);
	if (is_negative)
		num = -num;
	i = 0;
	if (num == 0)
		temp[i++] = '0';
	while (num > 0)
	{
		temp[i++] = (num % 10) + '0';
		num /= 10;
	}
	if (is_negative)
		temp[i++] = '-';
	temp[i] = '\0';

	// Reverse the string into `str`
	int j = 0;
	while (--i >= 0)
		str[j++] = temp[i];
	str[j] = '\0';
}
