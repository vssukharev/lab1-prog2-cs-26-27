
#include "tormentor.h"
#include <iostream>
#include <queue>
#include <stack>
#include <functional>
#include <cstring>

using namespace std;

void Tormentor::add_work(Worker w, Labour l) {
  parts[0][w].edges.push_back(l);
  parts[0][w].index = w;
  parts[1][l].edges.push_back(w);
  parts[1][l].index = l;
  initialized = finished = false;
}

Tormentor::Tormentor(size_t nw, size_t nl) 
  : parts{vector<Vertex>(nw), vector<Vertex>(nl)} 
  , pos{vector<size_t>(nw, MAX), vector<size_t>(nl, MAX)}
  , saved_pos(nl, MAX)
{
  if (nw < nl) throw runtime_error{"Labours can't be more than workers"};
}

Torture Tormentor::operator()() {
  if (finished) throw Tormentor::finished_error{};
  if (!initialized) {
    init();
    initialized = true;
  }
  Torture res = dump();
  // update();
  return res;
}

Tormentor::operator bool() noexcept {
  return !finished;
}

Torture Tormentor::dump() {
  size_t n = labours();
  Torture res(n);
  for (size_t l = 0; l < n; ++l) {
    size_t w = get_pos_adj(l);
    if (w == MAX) {
      throw runtime_error{"Not all labours can be covered by workers"};
    }
    res[l] = {w, l};
  }
  return res;
}

// void Tormentor::update() {
//   Labour i = 0;
//   while (!finished && !next_pos(i)) ++i;
//   make_save();
// }
//
// void Tormentor::make_save() {
//   memcpy(saved_pos.data(), pos[1].data(), saved_pos.size()*sizeof(Worker));
// }
//
// bool next_pos(Labour i) {
//   size_t m = pos[0].size() - 1;
//   const auto& adj = parts[1];
//
//   size_t k = pos[1][i];
//   for (size_t w : parts[1][i].edges) {
//
//   }
// }

void Tormentor::init() {
  while (update_distances()) xor_cardinality();
  saved_pos = pos[1];
}

// --- Helpers for BFS update_distances
static bool check_edge_even(size_t adj, size_t next) { 
  return adj != next; 
}

static bool check_edge_odd(size_t adj, size_t next) { 
  return adj == next; 
}

bool Tormentor::update_distances() {
  bool last_layer = false;
  bool updated = false;
  queue<Vertex*> q;

  auto is_free = [&](Vertex& v) -> bool {
    return pos[v.dist % 2][v.index] == MAX;
  };

  // Enqueue all free workers
  for (auto& w : parts[0]) {
    w.dist = 0;
    if (is_free(w)) {
      q.push(&w);
    } else {
      w.dist = MAX;
    }
  }

  // Reset distances for labours
  for (auto& l : parts[1]) {
    l.dist = MAX;
  }

  while (!q.empty()) {
    Vertex& cur = *q.front();
    q.pop();

    int part = cur.dist % 2;
    size_t adj = get_pos_adj(part, cur.index);

    // Look for free edges if part == 0. Otherwise, look for the used ones
    auto check = part == 0 ? &check_edge_even : &check_edge_odd;

    for (size_t i_next : cur.edges) {
      int dist_next = cur.dist + 1; 
      Vertex& next = parts[dist_next % 2][i_next];

      // If next.dist < cur.dist + 1, the adjacent vertex is already
      // visited from the LESS layers
      if (next.dist >= dist_next && check(adj, next.index)) {
        updated = true;
        next.dist = dist_next;
        // If free, stop on the current layer. Otherwise, go next
        if (is_free(next)) last_layer = true;
        else if (!last_layer) q.push(&next);
      }
    }
  }

  return updated;
}

void Tormentor::xor_cardinality() {
  stack<Vertex*> path;

  // Clear visited labels
  for (int i : {0, 1}) for (auto& v : parts[i]) v.visited = false;

  auto xor_edge = [&](Vertex& a, Vertex& b) {
    int ap = a.dist % 2;
    int bp = b.dist % 2;
    if (pos[ap][a.index] == b.index) {
      pos[ap][a.index] = MAX;
      pos[bp][b.index] = MAX;
    } else {
      pos[ap][a.index] = b.index;
      pos[bp][b.index] = a.index;
    }
  };

  // Traverse only free unvisited workers
  for (Vertex& w : parts[0]) if (w.dist == 0 && !w.visited) {
    path.push(&w);

    while (!path.empty()) {
      Vertex& cur = *path.top();

      if (!cur.visited) {
        // Go next
        cur.visited = true;
        for (size_t i_next : cur.edges) {
          int dist_next = cur.dist + 1; 
          Vertex& next = parts[dist_next % 2][i_next];

          // Find unvisited vertices of the next layer
          if (!next.visited && next.dist == dist_next) {
            xor_edge(cur, next);
            path.push(&next);
            break;
          }
        }
      } else {
        // Return to previous
        path.pop();
      }
    }
  }
}

