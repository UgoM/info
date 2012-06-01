#include "src/games/Example/GameExample.h"


/** \brief What to do when game receives data from server
  * \param dat : data received
  *
  * Here are only sent data. It is up to you to develop how your will encode
  * and decode your datas. Use sendToAll(QByteArray dat) to send back data to 
  * Brain.
  */
void GameExample::processReceive(QByteArray block)
{
}

