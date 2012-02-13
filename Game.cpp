#include "Game.h"

#include <iostream>

Game::Game()
{
	std::cout << "Constructeur Game" << std::endl;
}

Game::~Game()
{
	std::cout << "Destructeur Game" << std::endl;
}

void Game::sendTo(int idClient, QString message)
{
}

void Game::sendToAll(QString message)
{
}

void Game::processReceive(QString message)
{
}

void Game::processClick()
{
}

void Game::processKey()
{
}
