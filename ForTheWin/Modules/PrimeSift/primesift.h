/*
 *	ForTheWin: A modular windoze utility program.
 *	This file is part of ForTheWin.
 *
 *	Copyright (C) 2012 Zach Caldwell
 *
 * ForTheWin is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * ForTheWin is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *	along with ForTheWin.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _PRIMESIFT_H_
#define _PRIMESIFT_H_

/* Our C prime-sifting function.
 *
 * Params: An array of integers to sift and a variable to tell it whether to be verbose or not.
 * Returns: Nothing, but nulls nonprimes in the array.
*/
void SiftPrimes(int *intarry, const int nums, int verbose);

#endif //_PRIMESIFT_H_
