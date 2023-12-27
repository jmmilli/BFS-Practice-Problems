#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// Reflects what each node represents...
// First value units of water in A, second units of water in B
typedef pair<int,int> state;

// Each string in edge_label must be one of the following:
const string actions[] = {"Fill A",
                          "Fill B",
                          "Empty A",
                          "Empty B",
                          "Pour A->B",
                          "Pour B->A"};

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from source_node)
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

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << "[" << t.first << "," << t.second << "]\n";
  } else {
    cout << "Initial state: " << "[" << s.first << "," << s.second << "]\n";
  }
}


bool is_valid_state(state s, int capacity_A, int capacity_B) {
    return s.first >= 0 && s.second >= 0 && s.first <= capacity_A && s.second <= capacity_B;
}

//function to build the graph
void build_graph() {
    
    //set a const for the capacity of both jugs
    const int capacity_A = 3;
    const int capacity_B = 4;
    
    //Iterate through all possible states of jugs A and B
    for (int a = 0; a <= capacity_A; ++a) {
        for (int b = 0; b <= capacity_B; ++b) {
            
            //create the current state
            state s = make_pair(a, b);
            
            //Iterate through all actions for each state
            for (const string& action : actions) {
                
                //New state after performing an action
                state t = s;
                
                //Multiple if else statements to cover the different actions to generate the new state 't'
                if (action == actions[0]) //Fill A
                    t.first = capacity_A;
                else if (action == actions[1]) //Fill B
                    t.second = capacity_B;
                else if (action == actions[2]) //Empty A
                    t.first = 0;
                else if (action == actions[3]) //Empty B
                    t.second = 0;
                else if (action == actions[4]) { //Pour A->B
                    int pour = min(a, capacity_B - b);
                    t.first -= pour;
                    t.second += pour;
                }
                else if (action == actions[5]) { //Pour B->A
                    int pour = min(b, capacity_A - a);
                    t.second -= pour;
                    t.first += pour;
                }
                
                 //If the new state 't' has not been visited, add it to the graph
                if (visited.find(t) == visited.end()) {
                    //Add edge from state 's' to 't'
                    nbrs[s].push_back(t);
                    //Label the edge with the appropriate action
                    edge_label[{s, t}] = action;
                }
            }
        }
    }
}


int main(void)
{
  build_graph();

  state start = make_pair(0,0);
  
  for (int i=0; i<5; i++)
    nbrs[make_pair(i,5-i)].push_back(make_pair(-1,-1));
  search (start);
  if (visited[make_pair(-1,-1)]) 
    print_path (start, pred[make_pair(-1,-1)]);
  else
    cout << "No path!\n";
  
  return 0;
}



