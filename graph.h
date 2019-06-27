#pragma once

#include <memory>
#include <array>
#include <limits>

#include "list.h"
#include "queue.h"
#include "heap.h"
#include <mutex>

namespace ds {

template <class T>
class AdjacencyListGraph {

private:

  struct Edge {
    size_t src_;
    size_t dest_;
    double weight_;

    Edge()
        : src_(0),
          dest_(0),
          weight_(0) {

    }

    Edge(size_t src, size_t dest, double weight = 1.0) 
        : src_(src),
          dest_(dest),
          weight_(weight) {

    }

    bool operator< (const Edge& other) const { return weight_ < other.weight_; }
    bool operator> (const Edge& other) const { return other.weight_ < weight_; }
    bool operator<=(const Edge& other) const { return !(weight_ > other.weight_); }
    bool operator>=(const Edge& other) const { return !(weight_ < other.weight_); }
  }; // struct Edge

  struct Node {
    std::unique_ptr<T> value_;
    SLList<Edge> adj_;

    Node()
        : value_(),
          adj_() {

    }

    Node(const T& value)
        : value_(std::make_unique<T>(value)),
          adj_() {

    }

    Node(T&& value)
        : value_(std::make_unique<T>(std::move(value))),
          adj_() {

    }

    Node(Node&& other)
        : value_(std::move(other.value_)),
          adj_(std::move(other.adj_)) {
      other.value_.reset();
    }

    ~Node() {

    }
  }; // struct Node

  ArrayList<std::shared_ptr<Node>> nodes_;
  ArrayList<Edge> edges_;

  template <class Func>
  void DFSImpl(size_t start, ArrayList<bool>& visited, Func& f) const {
    visited[start] = true;
    f(start);

    for (auto edge : nodes_[start]->adj_) {
      auto node = edge.dest_;
      if (!visited[node]) {
        DFSImpl(node, visited, f);
      }
    }
  }

  void makePath(size_t start, size_t end, const ArrayList<size_t>& prev, ArrayList<size_t>& out_path) {
    size_t p = end;

    out_path.Append(p);

    while (p != start) {
      p = prev[p];
      out_path.Append(p);
    }

    std::reverse(out_path.begin(), out_path.end());
  }

public:
  AdjacencyListGraph(size_t num_nodes, std::initializer_list<T>&& nodes, std::initializer_list<Edge>&& edges) 
      : nodes_(num_nodes),
        edges_(edges) {
    for (auto val : nodes) {
      nodes_.Append(std::move(std::make_shared<Node>(val)));
    }
    for (auto edge : edges) {
      nodes_[edge.src_]->adj_.Append(edge);
    }
  }

  ~AdjacencyListGraph() {

  }

  size_t NodeCount() const {
    return nodes_.Size();
  }

  size_t EdgeCount() const {
    return edges_.Size();
  }

  template <class Func>
  void DFS(size_t start, Func& f) const {
    auto n = NodeCount();
    ArrayList<bool> visited(n);

    std::for_each(visited.begin(), visited.end(), [](bool& val) { val = false; });

    DFSImpl(start, visited, f);
  }

  template <class Func>
  ArrayList<size_t> BFS(size_t start, size_t end, Func& f) {
    auto n = NodeCount();
    ArrayList<bool> visited(n);
    ArrayList<size_t> prev(n);

    for (size_t i = 0; i < n; ++i) {
      visited[i] = false;
      prev[i] = i;
    }

    Queue<size_t> q;
    q.Push(start);
   
    ArrayList<size_t> path;

    while (!q.isEmpty()) {
      auto current = q.Pop();
      f(current);

      if (current == end) {
        makePath(start, end, prev, path);
        break;
      }

      for (auto edge : nodes_[current]->adj_) {
        q.Push(edge.dest_);
        prev[edge.dest_] = current;
      }
    }

    return path;
  }

  //template <class Func>
  std::pair<ArrayList<size_t>, double> Dikstras(size_t start, size_t end/*, Func& f*/) {

    if (start == end) {
      return { {}, 0 };
    }

    auto count = nodes_.Size();

    ArrayList<double> distance(count, std::numeric_limits<double>::max());
    ArrayList<size_t> prev(count, std::numeric_limits<size_t>::max());
    ArrayList<size_t> visited(count, false);

    BinHeap<Edge, MinHeap> q(count);

    Edge startEdge{ start, start, 0 };
    q.Insert(startEdge);
    distance[start] = 0;

    ArrayList<size_t> path;

    while (!q.isEmpty()) {
      Edge currentEdge = q.Pop();
      size_t n = currentEdge.dest_;

      if (n == end) {
        makePath(start, end, prev, path);
        break;
      }

      for (auto edge : nodes_[n]->adj_) {
        if (!visited[edge.dest_]) {
          auto d = distance[edge.src_] + edge.weight_;
          if (d < distance[edge.dest_]) {
            distance[edge.dest_] = d;
            q.Insert(edge);
            prev[edge.dest_] = edge.src_;
          }
        }
      }
      visited[n] = true;
    }

    return std::make_pair(std::move(path), distance[end]);
  }
};

} //namespace ds