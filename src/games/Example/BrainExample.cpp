#include "src/games/Example/BrainExample.h"

/** \brief Init function that should be filled
  */
void BrainExample::init()
{
    // nMaxPlayers = 42;
    // gameName = "Awesome Game";
}

/** \brief What to do when game receives data from clients
  * \param dat : data received
  *
  * Here are only sent data. It is up to you to develop how your will encode
  * and decode your datas. Use sendToAll(QByteArray dat) to send back data to 
  * Game.
  */
void BrainExample::processReceive(QByteArray dat)
{
    
}
/** \brief Encode data of the last game state
  */
QByteArray BrainExample::getLastData()
{
}
