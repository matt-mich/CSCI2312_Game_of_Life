// These are header files for the function.
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

// Info for Sleep function:
// https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program
// This sleep function adds a timed delay to the program.
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

// These are constants that will be defined in the program.
const int WIDTH = 50;
const int HEIGHT = 20;
const float init_probability_alive = 0.5;

// W => Windows
// M => Mac
// L => Linux
const char SYSTEM = 'W';

//This shows that each cell has a boolean value for if it is alive or not, a boolean value if it will switch to being
//alive or dead the next round, and a sum as an integer for the amount of neighboring alive cells.
struct Cell{
    int sum = 0;
    bool alive = false;
    bool will_switch = false;
};

// This function clears the console screen.
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

// This function generates a random boolean value based on the initial probability of the cell being alive.
bool is_init_alive(){
    // Get a random value between 0 and 1
    double r = ((double) rand() / (RAND_MAX));
    if (r < init_probability_alive){
        return true;
    }
    // There's no else statement here, will this still work?
    // If the initial probability is less than random value, then the cell will not be alive.
    return false;
}

// This function will initialize the game grid with cells that can be or may not be alive.
void init_game(Cell cell[HEIGHT][WIDTH]){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            bool is_alive = false;
            //This function will cause the program to ignore the edges of the grid and randomly determine if a cell is
            //alive
            if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                is_alive = is_init_alive();
            }
            cell[i][j].alive = is_alive;
        }
    }
}

// This is the main function for the program.
int main() {
    //A user types in a seed value that initializes a random number generator.
    std::cout << "game start, type a number" << std::endl;
    int temp_seed;
    char temp;
    std::cin >> temp_seed;
    srand(temp_seed);
    unsigned int round_count = 1;
    Cell cell[HEIGHT][WIDTH];
    // This initializes the game grid.
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
                    // Calculates for the sum of the live neighbors and checks for the cell neighbors.
                    for(int k = -1; k <= 1; k++) {
                        for (int m = -1; m <= 1; m++) {
                            // Check to make sure [i][j] isn't self
                            // Skips the cell itself.
                            if(k == 0 && m ==0){
                                continue;
                            }
                            //Checks if a cell is alive, and it is it adds one to the sum.
                            if(cell[i+k][j+m].alive){
                                sum += 1;
                            }
                        }
                    }
                    //This code will set the sum variable for the current cell to the value of sum calculated in lines
                    //107 and 108.
                    cell[i][j].sum = sum;
                    /*
                     * This code checks if the current cell is alive. It checks to see if the sum of the neighboring
                     *cells that are alive is less than 2 and greater than 3. The cell will switch to dead if this is
                     *true. The program will check if the sum is exactly 3 if the cell is dead, and if ture will
                     *switch to alive.
                    */
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

        /*
         * This code will represent a dead cell with . and a live cell with 0. If the cell is switched in the next
         * iteration, the code resets the will_switch variable and changes alive variable to false.
         */
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
                    //If the cell is alive it will now be dead.
                    cell[i][j].alive = !cell[i][j].alive;
                    cell[i][j].will_switch = false;
                }
            }
            //Creates new line in visual representation of the grid being generated.
            curr_line += '\n';
        }

        //This code will clear the console and print the current state of teh grid, and will wait 50 milliseconds
        //before proceeding.
        clear_console();
//        sleep_until(system_clock::now() + 50ms);
        std::cout << curr_line << std::endl;

        //This code will check to see if all cells on the grid are dead. Then, it prompts to user to continue or quit
        //the game. If the user continues a new game will be created, and if the user quits the game ends.
        sleep_until(system_clock::now() + 50ms);
        if(all_dead){
            std::cout << "All life has been exterminated. Good job, hero." << std::endl;
            printf("It survived for %d rounds. Continue? Y/N",round_count);
            std::cin >> temp;
            //If the user enters N or n the program writes a message and stops.
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.");
                game_running = false;
            // Causes tbe game to restart.
            }else{
                init_game(cell);
            }
        }
        //This code will check to see if a certain number of rounds has elapsed. If this has happened, then the program
        //will prompt the user to continue or quit the game. If the user quits then the game will end.
        if(round_count % round_pause == 0){
            printf("Paused at %d rounds. Enter n to escape, or anything else to continue.",round_count);
            std::cin >> temp;
            //This if statement end the game if the user inputs N or n.
            if(temp == 'N' || temp == 'n'){
                printf("Good call. See ya.\n");
                game_running = false;
            }
            // If the user inputs R or r the game will restart.
            if(temp == 'R' || temp == 'r'){
                //Calls init_game function.
                init_game(cell);
            }
        }

        round_count++;
//        std::cout << rand() << std::endl;
    }
    return 0;
}
