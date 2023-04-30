#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <SDL2/SDL.h>
// Info for Sleep function:
// https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program
using namespace std::this_thread;     // sleep_for, sleep_until
using std::chrono::system_clock;

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

const int WIDTH=100;
const int HEIGHT=100;
const int SCALE_FACTOR = 10;
const int WINDOW_WIDTH = WIDTH*SCALE_FACTOR;
const int WINDOW_HEIGHT = HEIGHT*SCALE_FACTOR;

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

void event_handler(SDL_Event event, int &key_req){
    SDL_PollEvent(&event);
    switch( event.type ){
        case SDL_KEYDOWN:
            printf( "Key press detected\n" );
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    key_req = 1;
                    break;
                case SDLK_r:
                    key_req = 2;
                    break;
                case SDLK_SPACE:
                    key_req = 3;
                    break;
                default:
                    break;
            }
            printf( "Key release detected\n" );
            break;
        case SDL_QUIT:
            key_req = 1;
            break;
        default:
            break;
    }
}

void drawToScreen(SDL_Renderer* renderer,int x, int y, \
                    SDL_Color color,
                    int scale,
                    SDL_Color outline){
    SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
    for(int i = 0; i < scale;i++){
        for(int j = 0; j < scale;j++) {
            if(i == 0 | j == 0 | i == scale-1 | j == scale-1){
                SDL_SetRenderDrawColor(renderer, outline.r,outline.g,outline.b,outline.a);
            }else{
                SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a);
            }
            SDL_RenderDrawPoint(renderer, x+i, y+j);
        }
    }
}

void main_game_loop(int** cells_pointer){
    // Draw the grid

}

int main(int argv, char** args) {
    int key_reqs = 0;
    std::cout << "game start, type a number" << std::endl;
    int temp_seed;
    char temp;
//    std::cin >> temp_seed;
//    srand(temp_seed);
    unsigned int round_count = 1;
    Cell** cells_array_pointer;
    Cell cell[HEIGHT][WIDTH];

    init_game(cell);
    bool game_running = true;
    // Allow user to exit after x rounds
    int round_pause = 200;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Color white = {255,255,255,255};
    SDL_Color paused_color = {255,100,100,255};
    SDL_Color outline = {255,0,0,255};

    SDL_Color BG = {0,0,0,255};

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH,
                                WINDOW_HEIGHT,
                                0,
                                &window,
                                &renderer);
    int running = 1;
    bool paused = false;
    SDL_SetWindowPosition(window,1632,1511);
    while (running) {
        SDL_SetRenderDrawColor(renderer, BG.r,BG.g,BG.b,BG.a);
        SDL_RenderClear(renderer);

        event_handler(event,key_reqs);
        switch(key_reqs){
            case 0:
                break;
            case 1:
                running = 0;
                break;
            case 2:
                init_game(cell);
                break;
            case 3:
                paused = !paused;
            default:
                break;
        }
        key_reqs = 0;
        // Update window

        if(!paused){
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
                        drawToScreen(renderer, 10*i, 10*j,white,10,outline);
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
//        std::cout << curr_line << std::endl;

            sleep_until(system_clock::now() + std::chrono::milliseconds(1));
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
//            if(round_count % round_pause == 0){
//                printf("Paused at %d rounds. Enter n to escape, or anything else to continue.",round_count);
//                std::cin >> temp;
//                if(temp == 'N' || temp == 'n'){
//                    printf("Good call. See ya.\n");
//                    game_running = false;
//                }
//                if(temp == 'R' || temp == 'r'){
//                    init_game(cell);
//                }
//            }

            round_count++;
        }else{
            for(int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    if (cell[i][j].alive) {
                        drawToScreen(renderer, 10 * i, 10 * j, paused_color, 10, outline);
                    }
                }
            }
        }

//        std::cout << rand() << std::endl;



        SDL_RenderPresent(renderer);
    }
    int curr_x = 0;
    int curr_y = 0;
    SDL_GetWindowPosition(window,&curr_x,&curr_y);
    std::cout << curr_x << " " << curr_y;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
