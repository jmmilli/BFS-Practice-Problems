#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
using namespace std;

// Reflects what each node represents...
// int with each bit == 0 left of river, bit == 1 right of river
typedef int state;

// Bit position representation for wolf/goat/cabbage/me
bool bit(int x, int i) { return (x>>i) & 1; }
const int wolf=0, goat=1, cabbage=2, me=3;

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from curnode)
void search(state source_node)
{
  queue<state> to_visit;
  to_visit.push(source_node);
  visited[source_node] = true;
  dist[source_node] = 0;
  
  while (!to_visit.empty()) {
    state curnode = to_visit.front();
    to_visit.pop();
    for (state n : nbrs[curnode])
      if (!visited[n]) {
	pred[n] = curnode;
	dist[n] = 1 + dist[curnode];
	visited[n] = true;
	to_visit.push(n);
      }
  }
}

string state_string(state s)
{
  string items[4] = { "wolf", "goat", "cabbage", "you" }, result = "";
  for (int i=0; i<4; i++)
    if (!bit(s, i)) result += items[i] + " ";
  result += " |river| ";
  for (int i=0; i<4; i++)
    if (bit(s, i)) result += items[i] + " ";
  return result;
}

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << state_string(t) << "\n";
  } else {
    cout << "Initial state: " << state_string(s) << "\n";
  }
}

string neighbor_label(int s, int t)
{
  string items[3] = { "wolf", "goat", "cabbage" }, which_cross;
  if (bit(s,me) == bit(t,me)) return "";  // must cross river
  int cross_with = 0;
  for (int i=0; i<3; i++) {
    if (bit(s,i) != bit(t,i) && bit(s,i)==bit(s,me)) { cross_with++; which_cross = items[i]; }
    if (bit(s,i) != bit(t,i) && bit(s,i)!=bit(s,me)) return "";
  }
  if (cross_with > 1) return "";
  if (cross_with == 0) return "Cross alone";
  else return "Cross with " + which_cross;
}

//function to build the graph
void build_graph(void)
{
    
    //iterate through all 16 states
    for (int i = 0; i < 16; i++) 
    {
        //first check to see if goat and cabbage are left alone
        if (bit(i, goat) == bit(i, cabbage)) {
            //then check if you are not with the goat (it is alone)
            if (bit(i, me) != bit(i, goat)) {
                //if these are met then the state is invalid 
                continue;
            }
        }
        
        //first check to see if goat and wolf are left alone
        if (bit(i, goat) == bit(i, wolf)) {
            //then check if you are not with the goat or wolf (they are alone)
            if (bit(i, me) != bit(i, goat) || bit(i, me) != (i, wolf)) {
                //if these are met then the state is invalid 
                continue; 
            }
        }
        
        //iterate through all states again to check the adjacent states
        for (int j = 0; j < 16; j++) 
        {
            //create a label for the transition between i and j and then transition
            string baseLabel;
            baseLabel = neighbor_label(i, j);
            
            //if baseLabel is not empty (false)
            if (baseLabel.empty() == 0)
            {   
                //add j as a neighbor of i and set the edgelabel
                nbrs[i].push_back(j);
                edge_label[make_pair(i, j)] = baseLabel;
            }
        }
    }
}


int main(void)
{
  build_graph();

  state start = 0, end = 15;
  
  search(start);
  if (visited[end])
    print_path (start, end);
  else
    cout << "No path!\n";
  
  return 0;
}


