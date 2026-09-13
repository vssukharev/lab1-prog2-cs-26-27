
#pragma once

#include <vector>
#include <utility>
#include <limits>

using Worker = std::size_t;
using Labour = std::size_t;
using Torture = std::vector<std::pair<Worker, Labour>>;

class Tormentor {
private:
  static constexpr std::size_t MAX = std::numeric_limits<std::size_t>::max();

  struct Vertex {
    std::vector<std::size_t> edges;
    size_t index;
    // BFS and DFS helper fields
    size_t dist = MAX;
    bool visited = false;
  };

  std::vector<Vertex> parts[2];    // 0 - workers, 1 - labours, bipartile graph
  std::vector<std::size_t> pos[2]; // current cardinality
  std::vector<Worker> saved_pos;   // saved positions of labours

  bool initialized = false;
  bool finished = false;

  /*
   Changes current cardinality to the next one
  **/
  void update();

  std::size_t get_pos_adj(std::size_t part, std::size_t i) { 
    return parts[part][pos[part][i]]; 
  }

  /*
   Next position of labour i. While moving, it finds
   the closest free position, which is set by labour
   j < i. If in the result i is on the saved position,
   returns false. Otherwise, recalculates positions
   j < i and returns true.
  **/
  bool next_pos(Labour i);

  /*
   Make fast copy of pos[1] into saved_pos
  **/
  void make_save();

  /*
   Initializes the graph via Hopcroft-Karp algorithm with helpers.
   TODO: error handling
  **/
  void init();
  void xor_cardinality();
  bool update_distances();

  /*
   Dumps current graph to outer representation
  **/
  Torture dump();

public:
  Tormentor() = default;

  class finished_error : std::exception {};

  /*
   Initializes Tormentor with the given numbers, cannot be changed
  **/
  Tormentor(std::size_t workers, std::size_t labours);

  /*
   Getters
  **/
  std::size_t workers() { return parts[0].size(); }
  std::size_t labours()  { return parts[1].size(); }

  /*
   Add labour l to worker w. Throws an exception in case of overflow
  **/
  void add_work(Worker w, Labour l);

  /*
   Returns next worker-labour schema. If all combinations are already
   enumerated, throws Tormentor::finished exception. Other possible errors:
   - Not all labours can be covered by workers in any combination
  **/
  Torture operator()();

  /*
   Returns false, if all enumerations are finished; true otherwise
  **/
  operator bool() noexcept;
};

