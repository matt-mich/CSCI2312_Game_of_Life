// import all the libraries
#include <iostream> //import input output library
#include <chrono> // import time releated library
#include <thread> // to manage the threads relete functionality
#include <string> // to manipulate strings

// Info for Sleep function:
// https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

const int WIDTH = 50; // initialize the width of the grid
const int HEIGHT = 20; // initialize the height of the grid
const float init_probability_alive = 0.5; // initialize the probablity of a cell to be alive

// W => Windows
// M => Mac
// L => Linux
const char SYSTEM = 'W';

// initialize cell data type
struct Cell{
    int sum = 0;
    bool alive = false;
    bool will_switch = false;
};

// this program is used to clear the console
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

//This function initializes the game by setting the alive status of each cell in the grid
//It takes a 2D array of type Cell as input/print the grid with alive cell and every other grid element
void init_game(Cell cell[HEIGHT][WIDTH]){
    //Loop through each row of the grid
    for(int i = 0; i < HEIGHT; i++){
        //Loop through each column of the grid
        for(int j = 0; j < WIDTH; j++){
            //Initialize the status of the cell to false
            bool is_alive = false;
            //If the cell is not on the border of the grid,
            //use the is_init_alive() function to determine if it should be alive or dead
            if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                is_alive = is_init_alive();
            }
            //Update the status of the cell with the determined status
            cell[i][j].alive = is_alive;
        }
    }
}

int main() {
    //Print a message to the console to indicate that the game has started
    std::cout << "game start, type a number" << std::endl;
    //Declare a integer temp_seed variable and a temp character variable
    int temp_seed;
    char temp;
    //Take input from the user and seed the random number generator
    std::cin >> temp_seed;
    srand(temp_seed);
    //Initialize a counter for the number of rounds and a 2D array of type Cell to store the cells in the grid
    unsigned int round_count = 1;
    Cell cell[HEIGHT][WIDTH];
    //Initialize the game by setting the alive status of each cell in the grid
    init_game(cell);
    //Set a boolean variable to indicate whether the game is still running
    bool game_running = true;
    // Allow user to exit after x rounds
    int round_pause = 200;
    while(game_running){
        // Draw the grid
        //Initialize a boolean variable to keep track of whether all cells are dead

        bool all_dead = true;
        //Loop through each row and column of the grid
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                //If the cell is alive, set all_dead to false
                if(cell[i][j].alive){
                    all_dead = false;
                }
                //If the cell is not on the border of the grid,
                //calculate the sum of the number of alive neighbors using a nested loop
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
                    //Update the sum of the cell
                    cell[i][j].sum = sum;
                    //If the cell is alive, set its will_switch value based on the sum of its neighbors
                    if(cell[i][j].alive){
                        if(sum > 3 || sum < 2){
                            cell[i][j].will_switch = true;
                        }
                    }
                        //If the cell is dead, set its will_switch value based on the sum of its neighbors
                    else{
                        if(sum == 3){
                            cell[i][j].will_switch = true;
                        }
                    }
                }
            }
        }
        //Draw the grid by looping through each row and column of the grid
        std::string curr_line;
        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < WIDTH; j++){
                //If the cell is alive, add a "0" to the current line string
                if(cell[i][j].alive){
                    curr_line += "0 ";
//                    std::cout << cell[i][j].sum << " ";
                }else{
                    //If the cell is dead, add a "." to the current line string
                    curr_line += ". ";
                }
                //If the cell will switch, switch its alive status and reset its will_switch value

                if (cell[i][j].will_switch){
                    cell[i][j].alive = !cell[i][j].alive;
                    cell[i][j].will_switch = false;
                }
            }
            curr_line += '\n';
        }

        //clears the console before each iteration of the game loop so that the previous state of the game board is not displayed.
        clear_console();
//        sleep_until(system_clock::now() + 50ms);
        std::cout << curr_line << std::endl;
        //waits for 50 milliseconds before the next iteration of the game loop to control the speed of the game.
        sleep_until(system_clock::now() + 50ms);
        //checks if all the cells on the game board are dead. If they are, it prompts the user to continue playing or exit the game.
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
        //pauses the game after a certain number of rounds specified by round_pause. It prompts the user to continue playing, reset the game board, or exit the game.
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
        // increments the round count for each iteration of the game loop.
        round_count++;
        // std::cout << rand() << std::endl;
    }
    return 0;
}
