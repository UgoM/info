#include "src/games/Example/BrainExample.h"

/** \brief Init function that should be filled
  */
void BrainExample::init()
{
    // nMaxPlayers = 42;
    // name = "Awesome Game";
}

/** \brief What to do when game receives data from server
  * \param dat : data received
  *
  * Here are only sent data. It is up to you to develop how your will encode
  * and decode your datas. Use send(QByteArray dat) to send back data to Brain.
  */
void BrainExample::processReceive(QByteArray dat)
{
    
}
/** \brief Encode data of the last game state
  */
QByteArray BrainExample::getLastData()
{
}
