#pragma once

#include <iostream>
#include <vector>
#include "solving_types.h"

namespace solving {

  template<typename Interface, typename Node>
  class Generic_Solver {
      std::vector<Node *> unresolved;
      std::vector<Node *> changed_buffers[2];
      std::vector<Node *> *changed = &changed_buffers[0];
      std::vector<Node *> *next_changed = &changed_buffers[1];
      Interface &interface;

  public:
      Generic_Solver(Interface &interface) : interface(interface) {}

      void set_changed(Node &node) {
        next_changed->push_back(&node);
        interface.on_changed(node);
      }

      void initialize() {
        for (auto &node : interface.get_nodes()) {
          if (Interface::is_resolved(*node))
            set_changed(*node);
        }
      }

      int update_changed() {
        changed->clear();

        // Flip buffers
        auto temp = changed;
        changed = next_changed;
        next_changed = temp;
        return changed->size();
      }

      Progress inhale(Node &node) {
        for (auto connection : node.get_connections()) {
          auto &other_node = connection->get_other(node);
          if (!Interface::is_resolved(other_node)) {
            if (interface.try_push(other_node, node, *connection, Direction::in))
              return 1;
          }
        }
        return 0;
      }

      Progress exhale(Node &node) {
        Progress progress = 0;
        for (auto connection : node.get_connections()) {
          progress += interface.try_push(node, connection->get_other(node), *connection, Direction::out);
        }
        return progress;
      }

      Progress process_changed() {
        Progress progress = 0;
        for (auto node : *changed) {
          if (!node)
            continue;

          if (Interface::is_resolved(*node))
            progress += exhale(*node);
          else
            progress += inhale(*node);
        }
        return progress;
      }

      bool is_resolved() {
        for (auto &node : interface.get_nodes()) {
          if (!Interface::is_resolved(*node)) {
            return false;
          }
        }

        return true;
      }

      bool loop() {
        int x = 0;
        while (update_changed()) {
          auto progress = process_changed();

          if (x++ > 20) {
            std::cout << "!!! Exceeded maximum solver iterations !!!" << std::endl;
            return false;
          }

          if (progress == 0) {
            if (!is_resolved()) {
              return false;
            }
          }
        }

        return true;
      }

      bool solve() {
        initialize();
        return loop();
      }
  };
}