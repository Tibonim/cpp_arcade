#include <fstream>
#include <iostream>
#include "encryption.hh"
#include "scoreDisplayer.hh"

namespace	arcade {
  scoreDisplayer::scoreDisplayer(std::string const& game, IGraph *graph)
    : _game{ game }, _graph { graph } {
      feedPlayer();
    }

  void	scoreDisplayer::createOut() const {
    std::string	file { std::string(".") + _game +
	std::string(".score") };
    std::string	out { _game + std::string(".score") };
    Encrypt		en { file, out };
    en.run();
  }

  void	scoreDisplayer::feedPlayer() {
    std::string		file { _game + std::string(".score") };
    std::ifstream	ifs{ file };

    if (!ifs)
      return ;
    std::string		line;
    while (std::getline(ifs, line)) {
      std::size_t	pos;
      if ((pos = line.find(':')) == std::string::npos)
	return ;
      std::string	firstOccur{ line.substr(0, pos) };
      std::string	lastOccur{ line.substr(pos + 1, line.size()) };
      int		score{ std::stoi(lastOccur) };
      _players[score] = firstOccur;
    }
  }

  void	scoreDisplayer::drawFinal(PointDraw& point) {
    point.posY += 5;
    _graph->drawSection("Quit", point, ColorToDraw::RED, true);
  }

  void	scoreDisplayer::update() {
    std::size_t	posx{ 15 };
    std::size_t	posy{ 0 };
    std::size_t	width{ 30 };
    std::size_t	height{ 5 };
    bool	display{ true };
    auto	it = _players.end();
    if (it != _players.begin())
      --it;
    else {
      PointDraw			firstP{ posx, posy, height, width };
      drawFinal(firstP);
      return ;
    }
    for (std::size_t idx = 0; idx < 4; ++idx) {
      PointDraw			firstP{ posx, posy, height, width };
      scoreDisplayer::section_t	first{ it->second + std::string("        ")
	  + std::to_string(it->first) };
      _graph->drawSection(std::make_pair(first, 1), firstP, ColorToDraw::RED,
			  display);
      display = false;
      posy += 5;
      if (it == _players.begin()) {
	drawFinal(firstP);
	return ;
      }
      --it;
      ++idx;
    }
    PointDraw	point{ posx, posy, height, width };
    drawFinal(point);
  }

  void	scoreDisplayer::enter() {
    _graph->stop();
  }

  void	scoreDisplayer::broadCmd(KeyEvent e, char c) {
    if (c == '\n' || e == KeyEvent::QUIT)
      enter();
  }

  void	scoreDisplayer::run() {
    update();
    _graph->loop([this](){ return this->update(); },
		 [this](KeyEvent e, char c){ return this->broadCmd(e, c); });
  }
}
