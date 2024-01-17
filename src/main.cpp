enum GameState {
    initialize,
    running,
    pause,
    terminate,
};

auto game_state = GameState::initialize;

int main() {
    while(game_state != GameState::terminate) {
        switch(game_state) {
            case initialize:
                break;
            case running:
                break;
            case pause:
                break;            
        }
    }
}