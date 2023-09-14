#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>
#include <IO/Node/Node.h>
using std::list;

namespace rgb_matrix {
  class Scheduler {
    public:
      virtual ~Scheduler() {}

      virtual void add_node(Node *node) = 0;

    protected:
      list<Node *> nodes_;
  };
}
#endif
