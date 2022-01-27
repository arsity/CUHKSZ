/*
 * File: TraverseTest.cpp
 * ------------------
 * This program reimplements the depth-first search algorithm using an
 * explicit stack
 * or
 * reimplements the breadth-first search algorithm using an
 * explicit queue.
 */

#include <iostream>
#include <fstream>
#include <string>
#include "set.h"
#include "simplegraph.h"
#include "stack.h"
#include "queue.h"
#include "foreach.h"

using namespace std;

/* Main program */
void dfs(Node* start);
void bfs(Node* start);

int TraverseTest() {
  SimpleGraph airline;
  ifstream infile("AirlineGraph.txt");
  readGraph(airline, infile);
  dfs(airline.nodeMap.get("Portland"));
  bfs(airline.nodeMap.get("Portland"));
  return 0;
}

void dfs(Node* start) {
  /* make sure that your implementation is based on an explicit Stack */
  Stack < Node * > st;
  Set < Node * > visited;
  st.add(start);
  visited.add(start);
  Node* temp;
  Arc* cursor;
  Set<Arc*>::iterator iter;
  while (!st.isEmpty()) { // iterate the stack
    temp = st.pop();
    cout << temp->name << " ";
    if (!temp->arcs.isEmpty()) {
      iter = temp->arcs.begin();
      while (iter != temp->arcs.end()) { // iterate the point
        cursor = *iter;
        if (!visited.contains(cursor->finish)) {
          visited.add(cursor->finish);
          st.add(cursor->finish);
        }
        iter++;
      }
    }
  }
}

void bfs(Node* start) {
  /* make sure that your implementation is based on an explicit Queue */
  Queue < Node * > qu;
  Set < Node * > visited;
  qu.add(start);
  visited.add(start);
  Node* temp;
  Arc* cursor;
  Set<Arc*>::iterator iter;
  while (!qu.isEmpty()) { // iterate the stack
    temp = qu.remove();
    cout << temp->name << " ";
    if (!temp->arcs.isEmpty()) {
      iter = temp->arcs.begin();
      while (iter != temp->arcs.end()) { // iterate the point
        cursor = *iter;
        if (!visited.contains(cursor->finish)) {
          visited.add(cursor->finish);
          qu.enqueue(cursor->finish);
        }
        iter++;
      }
    }
  }
}
