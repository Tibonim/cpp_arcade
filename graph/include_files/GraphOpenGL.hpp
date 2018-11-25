#pragma once

#include <functional>
#include <map>
#include <SDL/SDL.h>
#include <GL/glut.h>
#include "Protocol.hpp"
#include "AGraph.hh"
#include "argPtr.hpp"

#define W_WIDTH (800)
#define W_HEIGHT (800)
#define W_SIZE (W_WIDTH * W_HEIGHT)
#define W_BPP (32)

namespace	arcade {
  class		GraphOpenGL : public AGraph {
    typedef void (AGraph::*toPush_t) ()const;
    typedef std::function<void()> update_t;
    typedef std::function<void(KeyEvent, char)> broadCmd_t;
  private:
    std::map<int, toPush_t>	_cmdToPush;
    std::map<int, int>		_shift_code;
    std::vector<int>		_key_event;
    SDL_Surface			*window;
    SDL_Event			event;
    bool			_isRunning;
    argPtr<char>		_argc;
  public:
    GraphOpenGL(std::string const& path = "");
    virtual ~GraphOpenGL();
    virtual int		keyHook(broadCmd_t const& broadCmd_t) override;
    virtual void	drawWindow() override;
    virtual void	drawSection(IGraph::section_t const& section,
				    PointDraw const& pts,
				    ColorToDraw const color,
				    bool select) override;
    virtual void	drawSection(std::string const& section,
				    PointDraw const& pts,
				    ColorToDraw const color,
				    bool select) override;
    virtual void	drawMap(GetMap const *map,
				WhereAmI const *character,
				PointDraw const& pts) override;
    virtual void	loop(update_t const&, broadCmd_t const&)override;
    void		stop() override;
    void		setPath(std::string const& path) override;
  private:
    void        stackInQueue(int character) const;
    void	push(int ch) const;
    int		getShiftKey(int ch) const;
    void	pushKeyEvent(int);
    void	unstackKeyEvent(int);
    void	pushMyEvents()const;
    void	initDrawMap() const;
    void	drawPlayer(WhereAmI const *character, GetMap const *map, PointDraw const&pts);
    void	drawType(arcade::TileType type, float limitx, float limity) const;
    void	renderBitmapString(float, float, bool, const std::string&) const;
    void	drawEdges(PointDraw const&, bool) const;
  };
}

extern "C" {
  arcade::IGraph	*createGraph(std::string const& path);
}
