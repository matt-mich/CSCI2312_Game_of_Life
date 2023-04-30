#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <SDL2/SDL.h>
// Info for Sleep function:
// https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program
using namespace std::this_thread;     // sleep_for, sleep_until
using std::chrono::system_clock;

const int WIDTH = 50;
const int HEIGHT = 20;
const float init_probability_alive = 0.5;

// W => Windows
// M => Mac
// L => Linux
const char SYSTEM = 'W';

struct Cell{
    int sum = 0;
    bool alive = false;
    bool will_switch = false;
};

void clear_console(){
    switch(SYSTEM){
        case 'W':
            system("cls");
            break;
        case 'M':
        case 'L':
            std::cout << "\x1B[2J\x1B[H";
            break;
        default:
            break;
    }
}

bool is_init_alive(){
    // Get a random value between 0 and 1
    double r = ((double) rand() / (RAND_MAX));
    if (r < init_probability_alive){
        return true;
    }
    // There's no else statement here, will this still work?
    return false;
}

void init_game(Cell cell[HEIGHT][WIDTH]){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            bool is_alive = false;
            if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                is_alive = is_init_alive();
            }
            cell[i][j].alive = is_alive;
        }
    }
}

#define WINDOW_WIDTH 200
#define WINDOW_HEIGHT 100

void drawToScreen(SDL_Renderer* renderer,int x, int y, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
    SDL_RenderDrawPoint(renderer, x, y);
}

void readEvents(SDL_Event& event, int& running){
    SDL_PollEvent(&event);
    switch( event.type ){
        case SDL_KEYDOWN:
            printf( "Key press detected\n" );
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_ESCAPE){
                running = 0;
            }
            printf( "Key release detected\n" );
            break;
        case SDL_QUIT:
            running = 0;
            break;
        default:
            break;
    }
}


int main(int argv, char** args) {
    // Initialize SDL objects
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Color white = {255,255,255,255};
    SDL_Color BG = {0,0,0,255};

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT,  0, &window, &renderer);

    int running = 1;
    while (running) {
        SDL_RenderClear(renderer);
        readEvents(event,running);
        drawToScreen(renderer,10,10,white);

        SDL_SetRenderDrawColor(renderer, BG.r,BG.g,BG.b,BG.a);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    std::cout << "game start, type a number" << std::endl;
    int temp_seed;
    char temp;
    std::cin >> temp_seed;
    srand(temp_seed);
    unsigned int round_count = 1;
    Cell cell[HEIGHT][WIDTH];
    init_game(cell);
    bool game_running = true;
    // Allow user to exit after x rounds
    int round_pause = 200;
    while(game_running){
        // Draw the grid
        bool all_dead = true;
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                if(cell[i][j].alive){
                    all_dead = false;
                }
                if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                    int sum = 0;
                    for(int k = -1; k <= 1; k++) {
                        for (int m = -1; m <= 1; m++) {
                            // Check to make sure [i][j] isn't self
                            if(k == 0 && m ==0){
                                continue;
                            }
                            if(cell[i+k][j+m].alive){
                                sum += 1;
                            }
                        }
                    }
                    cell[i][j].sum = sum;
                    if(cell[i][j].alive){
                        if(sum > 3 || sum < 2){
                            cell[i][j].will_switch = true;
                        }
                    }else{
                        if(sum == 3){
                            cell[i][j].will_switch = true;
                        }
                    }
                }
            }
        }

        std::string curr_line;
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                if(cell[i][j].alive){
                    curr_line += "0 ";
//                    std::cout << cell[i][j].sum << " ";
                }else{
                    curr_line += ". ";
                }
                if (cell[i][j].will_switch){
                    cell[i][j].alive = !cell[i][j].alive;
                    cell[i][j].will_switch = false;
                }
            }
            curr_line += '\n';
        }

        clear_console();
//        sleep_until(system_clock::now() + 50ms);
        std::cout << curr_line << std::endl;

        sleep_until(system_clock::now() + std::chrono::milliseconds(50));
        if(all_dead){
            std::cout << "All life has been exterminated. Good job, hero." << std::endl;
            printf("It survived for %d rounds. Continue? Y/N",round_count);
            std::cin >> temp;
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.");
                game_running = false;
            }else{
                init_game(cell);
            }
        }
        if(round_count % round_pause == 0){
            printf("Paused at %d rounds. Enter n to escape, or anything else to continue.",round_count);
            std::cin >> temp;
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.\n");
                game_running = false;
            }
            if(temp == 'R' || temp == 'r'){
                init_game(cell);
            }
        }

        round_count++;
//        std::cout << rand() << std::endl;
    }
    return 0;
}
