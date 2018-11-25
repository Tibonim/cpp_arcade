#pragma once

#include <map>
#include "IGraph.hpp"

namespace	arcade {
  class		AGraph : public IGraph {
  protected:
    typedef std::function<void(KeyEvent, char)>	broadCmd_t;
  public:
    AGraph();
    virtual ~AGraph() = default;
    void		pushKeyUp() const;
    void		pushKeyDown() const;
    void		pushKeyLeft() const;
    void		pushKeyRight() const;
    void		pushQuit() const;
    void		pushShoot() const;
    void		pushDeletion() const;
    void		pushAscii(int ch) const;
    void		unstackQueueEvent(broadCmd_t const& broad) const;
  protected:
    bool				_isRunning;
  };
}
