#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  int processes = 30;
  NCursesDisplay::Display(system, processes);
}