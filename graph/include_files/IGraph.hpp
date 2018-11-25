#pragma once

#include <functional>
#include <vector>
#include <utility>
#include <string>
#include "Protocol.hpp"

#define W_WIDTH (800)
#define W_HEIGHT (800)
#define W_SIZE (W_WIDTH * W_HEIGHT)
#define W_BPP (32)

namespace	arcade {
  enum	KeyEvent {
    QUIT,
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    ASCII,
    SHOOT,
    EV_SHOOT,
    DELETE
  };

  struct	PointDraw {
    std::size_t	posX;
    std::size_t	posY;
    std::size_t	height;
    std::size_t	width;
  };

  enum		ColorToDraw {
    RED,
    GREEN,
    BLUE
  };

  class		IGraph {
  protected:
    typedef std::vector<std::string>		container_t;
    typedef std::pair<container_t, std::size_t>	section_t;
  public:
    virtual ~IGraph() = default;
    virtual int		keyHook(std::function<void(KeyEvent, char)>
				const& broadCmd) = 0;
    virtual void	drawWindow() = 0;
    virtual void	drawSection(std::string const& section,
				    PointDraw const& point,
				    ColorToDraw const color,
				    bool select = false) = 0;
    virtual void	drawSection(section_t const& section,
				    PointDraw const& point,
				    ColorToDraw const color,
				    bool select = false) = 0;
    virtual void	drawMap(GetMap const *map, WhereAmI const *character,
				PointDraw const& position) = 0;
    virtual void	setPath(std::string const& path) = 0;
    virtual void	stop() = 0;
    virtual void	loop(std::function<void()> const&,
			     std::function<void(KeyEvent, char)> const&) = 0;
  };
}
