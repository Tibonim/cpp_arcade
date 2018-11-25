#pragma once

#include <functional>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "Protocol.hpp"
#include "AGraph.hh"

#define W_WIDTH (800)
#define W_HEIGHT (800)
#define W_SIZE (W_WIDTH * W_HEIGHT)
#define W_BPP (32)

namespace	arcade {
  class		GraphSDL : public AGraph {
    typedef void (AGraph::*toPush_t) ()const;
    typedef std::function<void()> update_t;
    typedef std::function<void(KeyEvent, char)> broadCmd_t;
  private:
    std::map<int, toPush_t>	_cmdToPush;
    std::map<int, int>		_shift_code;
    std::vector<int>		_key_event;
    SDL_Surface			*window;
    SDL_Surface			*Fruit;
    SDL_Surface			*Block;
    SDL_Surface			*Evil_dude;
    SDL_Surface			*My_shoot;
    SDL_Surface			*Head;
    SDL_Surface			*Body;
    SDL_Surface			*Obstacle;
    SDL_Event			event;
    bool			_isRunning;
  public:
    GraphSDL(std::string const& path = "");
    virtual ~GraphSDL();
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
    virtual void		drawMap(GetMap const *map,
					WhereAmI const *character,
					PointDraw const& pts) override;
    virtual void	loop(update_t const&, broadCmd_t const&)override;
    void		stop() override;
    void		setPath(std::string const& path) override;
  private:
    SDL_Surface		*drawColoredBlock(Uint32, int width, int height) const;
    void		drawPixel(const SDL_Surface *, int, int, Uint32) const;
    void        stackInQueue(int character) const;
    void	push(int ch) const;
    int		getShiftKey(int ch) const;
    void	pushKeyEvent(int);
    void	unstackKeyEvent(int);
    void	pushMyEvents()const;
    void	drawPlayer(WhereAmI const *, GetMap const *, SDL_Rect const *, PointDraw const& pts);
    void	drawEdges(PointDraw const&, bool)const;
  };
}

extern "C" {
  arcade::IGraph	*createGraph(std::string const& path);
}
