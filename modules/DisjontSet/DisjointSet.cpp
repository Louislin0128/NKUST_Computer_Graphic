#include "DisjointSet.h"
#include <stdio.h>

/*
 * DisjointSet::constructor()
 *
 *	create n single item sets from {0}, {1}, ....{n-1}
 */
DisjointSet::DisjointSet(int n_item) {
	mi_no_set = n_item;

	// set the parent of every item to itself
	mpi_parent = new int[n_item];
	for (int i = 0; i < n_item; i++)
		mpi_parent[i] = i;
}// DisjointSet::constructor

/*
 * DisjointSet::get_no_set()
 *
 *	get the number of set
 */
int DisjointSet::get_no_set(void) {
	return mi_no_set;
}// DisjontSet::get_no_set

/*
 * DisjointSet::find()
 *
 *	find the representative of this set
 */
int DisjointSet::find(int x) {
	if (mpi_parent[x] != x) {
		mpi_parent[x] = find(mpi_parent[x]);
	}// if-condition

	return mpi_parent[x];
}// DisjointSet::find

/*
 * DisjointSet::union_()
 *
 *	union two sets
 */
void DisjointSet::union_(int x, int y) {
	//find the representative
	int xset = find(x);
	int yset = find(y);

	if (xset < yset) {
		mpi_parent[yset] = xset;
		mi_no_set--;
	}else if (xset > yset){
		mpi_parent[xset] = yset;
		mi_no_set--;
	}// if-condition
}// DisjontSet::get_no_set