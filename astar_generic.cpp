#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <queue>
#include "puzzle.hh"
#define DUMMY 1
using namespace std;

int num_steps = 0;
int mr_proof = 0;
typedef long long int lli;
typedef std::set<pair<int, lli> >::iterator It;
set<pair<int, lli> > openlist;
map <lli,int> fval;
map <lli,int> closedlist;
map <lli, lli> child_parent;
lli goal_node;
lli start_node;
puzzle p(352814670);

// backtracing path
void backtrace_path(lli node){
	if(node != start_node){
		backtrace_path(child_parent[node]);
	}
	p.print(node);
	num_steps++;
}




// function to modify openlist
void olMod(char op, lli node, int new_fval){
	
	if(op == '*'){ /*for modification */
		int old_fval = fval[node];
		It it = openlist.find(pair<lli,int>(old_fval,node));

		if(it == openlist.end())
			cout << "ERROR: Node not present in openlist to modify\n";
		
		else {
			openlist.erase(it);
			openlist.insert(pair<lli,int>(new_fval, node));
			fval.erase(node);
			fval[node] = new_fval;
		} 

	}

	if(op == '+'){ /*additon to openlist */
		openlist.insert(pair<lli,int>(new_fval, node));
		fval[node] = new_fval;
	}

	if(op == '-'){ /*deleting form the openlist */
		openlist.erase(openlist.begin());
	} 
}

//function to modify openlist
void clMod(lli node){
	closedlist[node] = DUMMY;
}

//main function to run search algo.
int main(){
	
	goal_node = 123456780;
	start_node = p.start();
	
	//inserting the start node into openlist.
	olMod('+', start_node, p.h(start_node));
	cout << "*SEARCH STARTED*" << endl;
	while(openlist.begin()->second != goal_node){
		num_steps++;
		lli node = openlist.begin()->second;
		int par_gval = openlist.begin()->first - p.h(node);

		//removing the node from openlist and insering it into closedlist.
		olMod('-',DUMMY, DUMMY);
		clMod(node);
		vector<pair<lli,int> > neighbours = p.get_neighs(node);

		for(int i = 0; i < neighbours.size(); i++){
			int neigh_node = neighbours[i].first;
			int edge_cost = neighbours[i].second;

			//if neighbour is in closedlist
			if(closedlist.find(neigh_node) != closedlist.end()){
				if((fval[neigh_node] - p.h(neigh_node)) > (par_gval + edge_cost)){
					mr_proof++;
				}
			}

			//neighbour not in closedlist
			else{
				//neigh in openlist
				if(fval.find(neigh_node) != fval.end()){
					//if new short path found
					if(fval[neigh_node] - p.h(neigh_node) > par_gval + edge_cost){
						olMod('*', neigh_node, par_gval + edge_cost + p.h(neigh_node));
						child_parent.erase(neigh_node);
						child_parent[neigh_node] = node;
					}
				}

				//neigh not in openlist
				else{
					olMod('+', neigh_node, par_gval+edge_cost+ p.h(neigh_node));
					child_parent.erase(neigh_node);
					child_parent[neigh_node] = node;
				}
			}
		}

	}
	
	cout << "*GOAL NODE REACHED*" << endl;
	backtrace_path(goal_node);
	// cout << "Number of nodes whose gval changed after adding to closed list: " << mr_proof << endl;
	cout << "Number of nodes expanded: " << num_steps << endl;

	return 0;
}
