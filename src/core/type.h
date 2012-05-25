#pragma once

namespace ClientType {
    enum EClientType {
        NONE,
        OBSERVER,
        PLAYER
    };
}

namespace Message {
    enum EMessage {
        NONE,
        
        // if Game wants to become a player
        I_WANT_TO_PLAY,
        OK_YOU_CAN_PLAY,
        NO_YOU_CANT_PLAY,
        // if Game wants to be an obs
        I_AM_AN_OBS_NOW,

        // Udp BroadCast
        UDP_ASK_FOR_SERVER,
        ANSWER_UDP_ASK_FOR_SERVER,

        // Server's welcome
        HELLO_FROM_SERVER,

        // Ask for the game list of the server
        ASK_LIST_GAMES,
        END_GAME_LIST
    };
}

/** \brief data types send across the network
  */
namespace DataType {
	enum EDataType {
		MESSAGE = 1,
        LISTOFSERVERS,
        GAMEDATA,
        NCONNECTED
	};
}
