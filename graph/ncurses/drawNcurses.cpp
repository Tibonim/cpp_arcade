#include "GraphNcurses.hh"

namespace	arcade {
  void		GraphNcurses::drawEmpty(uint16_t, uint16_t) const {
    return ;
  }

  void		GraphNcurses::drawBlock(uint16_t row, uint16_t col) const {
    mvwaddch(_map, row, col, '#');
  }

  void		GraphNcurses::drawObstacle(uint16_t row, uint16_t col) const {
    mvwaddch(_map, row, col, '#');
  }

  void		GraphNcurses::drawEvilDude(uint16_t row, uint16_t col) const {
    mvwaddch(_map, row, col, '*');
  }

  void		GraphNcurses::drawEvilShoot(uint16_t row, uint16_t col) const {
    mvwaddch(_map, row, col, '|');
  }

  void		GraphNcurses::drawMyShoot(uint16_t row, uint16_t col) const {
    mvwaddch(_map, row, col, '\'');
  }

  void		GraphNcurses::drawPowerup(uint16_t row, uint16_t col) const {
    init_color(COLOR_BLUE, 300, 400, 500);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    wattron(_map, COLOR_PAIR(2) | A_BLINK | A_BOLD);
    mvwaddch(_map, row, col, 'O');
    wattroff(_map, COLOR_PAIR(2) | A_BLINK | A_BOLD);
  }

  void		GraphNcurses::drawOther(uint16_t row, uint16_t col) const {
    mvwaddch(_map, row, col, '0');
  }
}
