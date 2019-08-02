#pragma once

#include "play/Player.h"

#include <immer/vector.hpp>

#include <memory>
#include <atomic>
#include <ctime>

namespace uw
{
    class GameManager
    {
    public:
        GameManager():
            _msPerFrame(1000 / PHISICS_FRAME_PER_SECOND),
            _isRunning(true),
            _nextPlayers(nullptr),
            _nextAddPlayer(nullptr)
        {}

        ~GameManager()
        {
            stop();
        }

        void stop()
        {
            _isRunning = false;
        }

        void startSync()
        {
            while(_isRunning) {

                const auto startFrameTime = std::chrono::steady_clock::now();

                loopPhysics();

                const auto endFrameTime = std::chrono::steady_clock::now();

                const auto frameTimeInMs = (unsigned int)std::chrono::duration<double, std::milli>(endFrameTime - startFrameTime).count();

                if (frameTimeInMs < _msPerFrame)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(_msPerFrame - frameTimeInMs));
                }
            }
        }

        void setNextPlayer(std::shared_ptr<Player> newPlayer)
        {
            _nextAddPlayer.store(new Player(*newPlayer));
        }

        void setNextPlayers(immer::vector<std::shared_ptr<Player>> nextPlayers)
        {
            _nextPlayers.store(new immer::vector<std::shared_ptr<Player>>(nextPlayers.begin(), nextPlayers.end()));
        }

        immer::vector<std::shared_ptr<Player>> players()
        {
            return _players;
        }

        immer::vector<std::shared_ptr<const Singuity>> singuities()
        {
            const auto localPlayers = _players;

            std::vector<std::shared_ptr<const Singuity>> singuities;
            for (auto player : localPlayers)
            {
                auto playerSinguities = player->singuities();
                singuities.insert(singuities.end(), playerSinguities.begin(), playerSinguities.end());
            }

            return immer::vector<std::shared_ptr<const Singuity>>(singuities.begin(), singuities.end());
        }

    private:

        void loopPhysics()
        {
            const auto newPlayer = _nextAddPlayer.exchange(nullptr);
            if (newPlayer)
            {
                _players = _players.push_back(std::shared_ptr<Player>(newPlayer));
            }

            const auto allPlayers = _nextPlayers.exchange(nullptr);
            if (allPlayers)
            {
                _players = *allPlayers;
                delete allPlayers;
            }

            auto localPlayers = _players;
            std::vector<std::shared_ptr<Player>> workingPlayers(localPlayers.begin(), localPlayers.end());

            for(unsigned int workingPlayerIndex = 0; workingPlayerIndex < workingPlayers.size(); ++workingPlayerIndex)
            {
                workingPlayers[workingPlayerIndex] = std::make_shared<Player>(*workingPlayers[workingPlayerIndex]);
            }

            for (auto workingPlayer : workingPlayers)
            {
                workingPlayer->actualize();
            }

            _players = immer::vector<std::shared_ptr<Player>>(workingPlayers.begin(), workingPlayers.end());
        }

        static const unsigned int PHISICS_FRAME_PER_SECOND;

        const unsigned int _msPerFrame;
        std::atomic<Player*> _nextAddPlayer;
        std::atomic<immer::vector<std::shared_ptr<Player>>*> _nextPlayers;
        immer::vector<std::shared_ptr<Player>> _players;
        bool _isRunning;
    };
}