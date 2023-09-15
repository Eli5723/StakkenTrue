#pragma once


class Player {

};

enum class PlayerState {
    AFK,
    Playing,
    Winner,
    Loser,
};

class Lobby {
    public:
        void applyInput();

        void addPlayer(Player player);
        void removePlayer(Player player); 

        void startGame();
        void endGame();

        void tick();

        void seria
};