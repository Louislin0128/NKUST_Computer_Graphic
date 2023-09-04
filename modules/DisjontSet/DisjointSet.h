#pragma once

/*
 * class DisjointSet: 
 *
 *	implement the disjoint set data structure by using array
 */
class DisjointSet {
	// data field
	private:
		int	 mi_no_set;		// number of sets
		int* mpi_parent;	// parent array of every item

	// constructor
	public:
		DisjointSet(int n_item);

	// public methods
	public:
		int		get_no_set(void);	// get the number of sets

		int		find(int);			// find the representative of this set
		void	union_(int, int);	// union two sets
};// class DisjointSet