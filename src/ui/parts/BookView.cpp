#include "BookView.h"

#include "Player.h"

using namespace std;

void BookView::populate(const Player *player, SpellBook::Type type)
{
  auto totalPool = player->book()->bookSpells(type);
  
  // TODO finire
}