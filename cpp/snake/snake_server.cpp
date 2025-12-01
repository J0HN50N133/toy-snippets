
#include <numeric>
class Server {
public:
    void init() {
        // 初始化服务器
    }
    void run() {
        // 运行服务器
    }
    void stop() {
        // 停止服务器
    }
};

// 一局游戏
class Game {
private:
    int _id;
};

// player
class Player {
private:
    int _id;
    int _game_id; // 正在游玩的游戏id
    int _snake_id; // 正在游玩的蛇id
};



int main(int argc, const char** argv) {
    Server server;
    server.init();
    server.run();
    return 0;
}