#include <platform/pc/SDLPlatform.hpp>
#include <chrono>

bool SDLPlatform::init(int w, int h) {
    width = w;
    height = h;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("ICQ Console",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB565,   // IMPORTANT (matches framebuffer)
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );

    return true;
}

uint64_t SDLPlatform::time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void SDLPlatform::present(const uint16_t* fb) {
    SDL_UpdateTexture(texture, nullptr, fb, width * sizeof(uint16_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

Input SDLPlatform::pollInput() {
    Input in;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            in.quit = true;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    in.up    = keys[SDL_SCANCODE_UP];
    in.down  = keys[SDL_SCANCODE_DOWN];
    in.left  = keys[SDL_SCANCODE_LEFT];
    in.right = keys[SDL_SCANCODE_RIGHT];
    in.x     = keys[SDL_SCANCODE_X];

    return in;
}

uint32_t SDLPlatform::getRandomNumber()
{
    std::uniform_int_distribution<uint32_t> dist(
        0, std::numeric_limits<uint32_t>::max()
    );
    return dist(rng);
}

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <mutex>

void broadcastLanGame(uint32_t gameId,
                                   uint16_t wsPort)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcastEnable = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
               &broadcastEnable, sizeof(broadcastEnable));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777); // discovery port
    addr.sin_addr.s_addr = INADDR_BROADCAST;

    LanBroadcastPacket pkt{};
    pkt.magic[0] = 'I';
    pkt.magic[1] = 'C';
    pkt.magic[2] = 'Q';
    pkt.magic[3] = 'G';

    pkt.gameId = gameId;
    pkt.wsPort = wsPort;


    sendto(sock,
           &pkt,
           sizeof(pkt),
           0,
           (sockaddr*)&addr,
           sizeof(addr));

    close(sock);
}

void SDLPlatform::startAdvertising(GAME_NAME game)
{
    if(advertising.load()){
        return;
    }
    advertising.store(true);
    
    advertisingThread = std::thread([this,game]() {
        while (advertising.load()) {

            broadcastLanGame(
                (uint32_t)game,
                8899
            );
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

}

void SDLPlatform::stopAdvertising()
{
    advertising.store(false);
    if(advertisingThread.joinable()) {
        advertisingThread.join();
    }
}

bool SDLPlatform::isAdvertising() const
{
    return advertising.load();
}

void SDLPlatform::startDiscovery()
{
    if (discovering.load()) return;

    discovering.store(true);

    discoveryThread = std::thread([this]() {

        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        timeval tv{};
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(7777);
        addr.sin_addr.s_addr = INADDR_ANY;

        bind(sock, (sockaddr*)&addr, sizeof(addr));

        while (discovering.load()) {

            sockaddr_in sender{};
            socklen_t senderLen = sizeof(sender);

            LanBroadcastPacket pkt{};

            int len = recvfrom(sock,
                               &pkt,
                               sizeof(pkt),
                               0,
                               (sockaddr*)&sender,
                               &senderLen);

            if (len == sizeof(pkt)) {

                // Validate magic
                if (pkt.magic[0] == 'I' &&
                    pkt.magic[1] == 'C' &&
                    pkt.magic[2] == 'Q' &&
                    pkt.magic[3] == 'G') {

                    std::lock_guard<std::mutex> lock(discoveredMutex);
                    discoveryDevice d = {
                        .adress = sender.sin_addr.s_addr,
                        .game= (GAME_NAME)pkt.gameId
                    };
                    discoveredDevices[d.adress]=(GAME_NAME)pkt.gameId;
                }
            }
        }

        close(sock);
    });
}


void SDLPlatform::stopDiscovery()
{
    discovering.store(false);

    if (discoveryThread.joinable()) {
        discoveryThread.join();
    }
}

bool SDLPlatform::isDiscovering() const
{
    return discovering.load();
}

void SDLPlatform::pollDiscovered(std::vector<device_id>& devicesOut)
{
    std::lock_guard<std::mutex> lock(discoveredMutex);

    devicesOut.clear();

    for (const auto& [id, _] : discoveredDevices) {
        devicesOut.push_back(id);
    }
}