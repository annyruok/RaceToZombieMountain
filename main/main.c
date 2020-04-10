//
//  main.c
//  assignment1
//
//  Created by Anny Yin on 26/3/18.
//  Copyright © 2018 Anny Yin. All rights reserved.
//
/*
#include </Users/Home/Documents/Bruh/sem 1 2018/cab202/ASSIGNMENT/assignment1/ZDK/cab202_graphics.h>
#include </Users/Home/Documents/Bruh/sem 1 2018/cab202/ASSIGNMENT/assignment1/ZDK/cab202_sprites.h>
#include </Users/Home/Documents/Bruh/sem 1 2018/cab202/ASSIGNMENT/assignment1/ZDK/cab202_timers.h>
*/

#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

/*-----------------------------------------------------------------------------------------------------------------*/

/*

 "   _______  "
 "  /       \ "
 " }_________{"
 " ||___ ___||"
 " ||\_/ \_/||"
 " ||       ||"
 " }\_______/{"
 "  \_______/ "

 "   ____  "
 "  /    \ "
 " O______O"
 " ||[][]||"
 " O\____/O"
 "  \____/ "

 "  .--""""--.  " // 14 wide
 "  |===  ===|\ " // 8 down
 "  |~~~~~~~~| }"
 "  |  FUEL  |/ "
 "  |        |  "
 "  |        |  "
 "  |        |  "
 "__|________|__"

   \_/___\_/
   /       \
  |         |
 _|_________|_
  ||___ ___||
  ||\_/ \_/||
  ||       ||
  |\_______/|
  \         /
   \_______/

   \_/____\_/
   /        \
  }          {
 _|__________|_
  ||\______/||
  ||___  ___||
  ||\_/  \_/||
  ||        ||
  }\________/{
  \          /
   \________/

   ________
  /========\
 _|________|_
 |          |
 |          |
 |          |
 |          |
 |          |
 |__________|
 _____||_____
 |          |
 |          |
 |          |
 |          |
 |          |
 |          |
 |__________|


  []-----[]
      =
      =
 []-------[]
 |         |
 |         |
 |--- O ___|
 |         |
 |         |
 []-------[]
 */


sprite_id fuel_station;
sprite_id driver;
double speed = 0; // shouldnt be too hard to make local but we'll seeeee/i'm too scared to do it rn
double dy;
double initial_time;
sprite_id active_objects[100];



char * usercar(void) {
    char * car =
    "  ____  " // 8 wide
    " /*||*\\ "  /// 6 down
    "O______O"
    "||[][]||"
    "O\\____/O"
    " \\____/ ";

    return car;
}

char * scenery_img(int imgId) {
    char * image;
    if (imgId == 0) { // tree
        image =
        "  #####  "    /// 9 wide
        "#########"      //// 6 down
        "##\\###/##"
        " ##\\|/## "
        "   }|{   "
        "   /|\\   ";
    }

    else if (imgId == 1) { // pond
        image =
        "     ____    "  /// 13 wide
        "  __/ ~  \\_  " /// 5 down
        " / ~    ~ __)"
        "(__  ~  _/   "
        "    \\__/     ";
    }

    else { // house
        image =
        "  ____[]_  " // 11 wide
        " /^_^-^_^\\ " // 5 down
        "/^_^_^_^_^\\"
        " | ___   | "
        " |_|_|___| ";
    }

    return image;
}

char * obstacle_img(int imgId) {
    char * image;
    if (imgId == 0) {
        image =
        "(((((|))))" // 10 wide
        "|        |" // 5 down
        "| X    X |"
        "|     _  |"
        "\\________/";

    }
    else {
        image =
        "    _   " // 8 wide
        "   /_\\  " /// 3 down
        " _/___\\_"
        "        ";
    }

    return image;
}

char * fuel_station_img(void) {
    char * image =
    ".-\"\"\"\"-.  " // 10 wide
    "|==  ==|\\ " // 6 down
    "|~~~~~~| }"
    "| FUEL |/ "
    "|      |  "
    "|______|  ";

    return image;
}

int total_obstacles(int increment) {
    static int amount = 0; //// amount on screen

    if (increment == 100) {
        amount = 0;
    }
    else {
        amount += increment;
    }

    return amount;
}

int scenery_random_x(int width) {
    int x_cord = rand() % (2 * (screen_width() / 4 - 1 - width)) + 1;

    if (x_cord > screen_width() / 4 - width - 1) {
        x_cord += screen_width() / 2 + width + 1;
    }

    return x_cord;
}

sprite_id create_scenery(int y) {
    sprite_id scenery;
    int imgId = rand() % 3;

    if (imgId == 0) {
        scenery = sprite_create(scenery_random_x(9), y, 9, 6, scenery_img(imgId));
    }
    else if (imgId == 1) {
        scenery = sprite_create(scenery_random_x(13), y, 13, 5, scenery_img(imgId));
    }
    else {
        scenery = sprite_create(scenery_random_x(11), y, 11, 5, scenery_img(imgId));
    }

    return scenery;
}

sprite_id create_obstacle(int y) {
    sprite_id obstacle;
    int imgId = rand() % 2;
    int x;

    if (imgId == 0) {
        x = rand() % ((screen_width() / 2) - 10) + screen_width() / 4 + 1;
        obstacle = sprite_create(x, y, 10, 5, obstacle_img(imgId));
    }
    else {
        x = rand() % ((screen_width() / 2) - 8) + screen_width() / 4 + 1;
        obstacle = sprite_create(x, y, 8, 4, obstacle_img(imgId));
    }

    return obstacle;

}

int random_y(int height, bool setting_up) {
    int y;

    if (setting_up) {
        y = (rand() % (screen_height() - height) + height) * - 1;
    }
    else {
        y = (rand() % (screen_height() / 5) + height) * - 1;
    }
    return y;
}

sprite_id pick_object(bool setting_up) {
    int pick = rand() % 2;
    int height;
    int y;
    sprite_id object;

    if (pick == 1) { // scenery
        height = 6;
        y = random_y(height, setting_up);
        object = create_scenery(y);
    }
    else { // obstacles
        height = 5;
        y = random_y(height, setting_up);
        object = create_obstacle(y);
    }

    // total_obstacles(1);
    return object;
}


bool space_occupied(sprite_id first, sprite_id second) {
    // sprites[]
    int first_left = round(sprite_x(first));;
    int first_right = first_left + sprite_width(first) - 1;;
    int first_top = round(sprite_y(first));;
    int first_bottom = first_left + sprite_height(first) - 1;;

    // sprite ur checking
    int second_left = round(sprite_x(second));
    int second_right = second_left + sprite_width(second) - 1;
    int second_top = round(sprite_y(second));
    int second_bottom = second_top + sprite_height(second) - 1;

    // collision comparisons
    if (first_top > second_bottom) return false;
    if (second_top > first_bottom) return false;
    if (first_left > second_right) return false;
    if (second_left > first_right) return false;

    return true;

}

bool test_overlap(void) {
    for (int i = 0; i < total_obstacles(0); i++) {
        for (int j = 0; j < total_obstacles(0); j++) {
            if (i != j) {
                bool overlap_present = space_occupied(active_objects[i], active_objects[j]);
                if (overlap_present) {
                    return true;
                }
            }
        }
    }
    return false;
}

void remove_overlap(bool setting_up) {
    for (int i = 0; i < total_obstacles(0); i++) {
        for (int j = 0; j < total_obstacles(0); j++) {
            if (i != j) {
                bool occupied = space_occupied(active_objects[i], active_objects[j]);
                while (occupied) {
                    active_objects[i] = pick_object(setting_up);
                    occupied = space_occupied(active_objects[i], active_objects[j]);
                }
            }
        }
    }
}

void replace_sprite(void) {
    for (int i = 0; i < total_obstacles(0); i++) {
        if (sprite_y(active_objects[i]) > screen_height()) {
            sprite_destroy(active_objects[i]);
            active_objects[i] = pick_object(false);
        }
    }

    bool overlap_present = test_overlap();
    while (overlap_present) {
        remove_overlap(false);
        overlap_present = test_overlap();
    }
}

char * get_sprite_image(int width) {
    char * image;
    if (width == 9) {
        image =
        "  #####  "    /// 9 wide
        "#########"      //// 6 down
        "##\\###/##"
        " ##\\|/## "
        "   }|{   "
        "   /|\\   ";
    }
    else if (width == 13) {
        image =
        "     ____    "  /// 13 wide
        "  __/ ~  \\_  " /// 5 down
        " / ~    ~ __)"
        "(__  ~  _/   "
        "    \\__/     ";
    }
    else if (width == 11) {
        image =
        "  ____[]_  " // 11 wide
        " /^_^-^_^\\ " // 5 down
        "/^_^_^_^_^\\"
        " | ___   | "
        " |_|_|___| ";;
    }
    else if (width == 10) {
        image =
        "(((((|))))" // 10 wide
        "|        |" // 5 down
        "| X    X |"
        "|     _  |"
        "\\________/";
    }
    else { /// width == 8
        image =
        "    _   " // 8 wide
        "   /_\\  " /// 3 down
        " _/___\\_"
        "        ";
    }

    return image;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*=================================================================================================================*/
/*=================================================================================================================*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

// bool game_over = false;
bool game_running(int status) {
    static bool running = true;

    if (status == 1) {
        running = !running;
    }

    return running;
}

//
bool game_won(int status) {
    static bool won = false;

    if (status == 100) {
        won = false;
    }
    else if (status == 1) {
        won = !won;
    }

    return won;
}

// double offroad_left;
double offroad_left(void) {
    static double left;
    left = screen_width() / 4;

    return left;
}

// double offroad_right;
double offroad_right(void) {
    static double right;
    right = 3 * screen_width() / 4;

    return right;
}

// double fuel = 100;
double fuel(double df) { // delta f, chance in fuel
    static double fuel_remaining = 100;

    if (df == 100) {
        fuel_remaining = 100; // resetting fuel
    }
    else {
        fuel_remaining += df;
    }

    if (fuel_remaining < 0) {
        fuel_remaining = 0;
    }

    return fuel_remaining;
}


// double distance;
double distance_travelled(int update) {
    static double distance = 0;

    if (update == 1) {
        distance += dy * 0.1;
    }
    else if (update == 100) {
        distance = 0;
    }

    return distance;
}

int car_damage(int update) {
    static int condition = 100;

    if (update == 1) {
        if (speed < 3) {
            condition -= 10;
        }
        else if (speed < 5) {
            condition -= 20;
        }
        else if (speed < 7) {
            condition -= 30;
        }
        else if (speed < 9) {
            condition -= 40;
        }
        else {
            condition -= 50;
        }
    }
    else if (update == 100) {
        condition = 100;
    }

    return condition;
}

/////////////--------------------------------------------///////////////////

//// zombiescenery
void step_object( sprite_id object ) {

    sprite_turn_to(object, 0, dy);
    sprite_step(object);
    sprite_draw(object);
}

bool pixel_collision(sprite_id obstacle, int x_overlap, int y_overlap, int i_start, int i_end, int j_start, int j_end, bool left_side, char * image) {

    for (int i = i_start; i < i_end; i++) {
        for (int j = j_start; j < j_end; j++) {
            int car_start = i * sprite_width(driver);
            int car_element = car_start + j;
            char car_char = usercar()[car_element];
            if (car_char != ' ') {

                int obstacle_row;
                if (i_start == 0) {
                    obstacle_row = i + y_overlap;
                }
                else {
                    obstacle_row = i - y_overlap;
                }

                int obstacle_element;
                if (left_side) {
                    obstacle_element = obstacle_row * sprite_width(obstacle) + (sprite_width(obstacle) - x_overlap) + j;
                }
                else {
                    obstacle_element = obstacle_row * sprite_width(obstacle) + (x_overlap - sprite_width(driver)) + j;
                }

                char obstacle_char = image[obstacle_element];
                if (obstacle_char != ' ') {
                    return true;

                }
            }
        }
    }
    return false;
}


int driver_left(void) {
    int left = round(sprite_x(driver));
    return left;
}

int driver_right(void) {
    int right = driver_left() + sprite_width(driver) - 1;
    return right;
}

int driver_top(void) {
    int top = round(sprite_y(driver));
    return top;
}

int driver_bottom(void) {
    int bottom = driver_top() + sprite_height(driver) - 1;
    return bottom;
}

int obstacle_left(sprite_id obstacle) {
    int left = round(sprite_x(obstacle));
    return left;
}

int obstacle_right(sprite_id obstacle) {
    int right = obstacle_left(obstacle) + sprite_width(obstacle) - 1;
    return right;
}

int obstacle_top(sprite_id obstacle) {
    int top = round(sprite_y(obstacle));
    return top;
}

int obstacle_bottom(sprite_id obstacle) {
    int bottom = obstacle_top(obstacle) + sprite_height(obstacle) - 1;
    return bottom;
}


/// zombiescenery
bool collided(sprite_id driver, sprite_id obstacle, char * object_image) {
    int y_overlap;
    if (driver_top() <= obstacle_bottom(obstacle) || driver_bottom() >= obstacle_top(obstacle)) {
        if (driver_left() <= obstacle_right(obstacle) && driver_left() >= obstacle_left(obstacle)) {
            int x_overlap = obstacle_right(obstacle) - driver_left() + 1;

            if (driver_top() < obstacle_top(obstacle)) {
                y_overlap = obstacle_top(obstacle) - driver_top();

                return pixel_collision(obstacle, x_overlap, y_overlap, y_overlap, sprite_height(driver), 0, x_overlap, true, object_image);

            }

            else {
                y_overlap = driver_top() - obstacle_top(obstacle);
                return pixel_collision(obstacle, x_overlap, y_overlap, 0, sprite_height(obstacle) - y_overlap, 0, x_overlap, true, object_image);

            }

        }


        else if (driver_right() >= obstacle_left(obstacle) && driver_right() <= obstacle_right(obstacle)) {
            int x_overlap = driver_right() - obstacle_left(obstacle) + 1;

            if (driver_top() < obstacle_top(obstacle)) {
                y_overlap = obstacle_top(obstacle) - driver_top();
                return pixel_collision(obstacle, x_overlap, y_overlap, y_overlap, sprite_height(driver), sprite_width(driver) - x_overlap, sprite_width(driver), false, object_image);

            }

            else {
                y_overlap = driver_top() - obstacle_top(obstacle);
                return pixel_collision(obstacle, x_overlap, y_overlap, 0, sprite_height(obstacle) - y_overlap, sprite_width(driver) - x_overlap, sprite_width(driver), false, object_image);
            }

        }


    }

    return false;

}

void relocate_car(void) {

    int x = (rand() % (screen_width() - sprite_width(driver) - 2)) + 1;
    int y = sprite_y(driver);
    sprite_move_to(driver, x, y);

    bool overlap = false;
    int station_overlap = space_occupied(fuel_station, driver); /// error bc the functin isn't defined yet

    for (int i = 0; i < total_obstacles(0); i++) {
        overlap = space_occupied(driver, active_objects[i]);
        while (overlap || station_overlap ) {
            x = (rand() % (screen_width() - sprite_width(driver) - 2)) + 1;
            sprite_move_to(driver, x, y);

            station_overlap = space_occupied(fuel_station, driver);
            overlap = space_occupied(driver, active_objects[i]);
        }
    }

    speed = 0;
    fuel(100);
}



/////////////--------------------------------------------///////////////////

bool offroad(void) {
    if (sprite_x(driver) < offroad_left() || sprite_x(driver) + sprite_width(driver) > offroad_right() ) {
        return true;
    }
    return false;
}

void change_speed( int key ) {
    // bring speed down if car goes offroad
    if (offroad()) {
        if (speed > 3) {
            speed = 3;
        }
    }
    // speed up
    if (key == 'i' || key == 'I') {
        if (offroad() && speed < 3) {
            speed += 1;
        }
        if (!offroad() && speed < 10) {
            speed += 1;
        }
    }
    // slow down speed
    if ((key == 'k' || key == 'K') && speed > 0) {
        speed -= 1;
    }

    dy = 0.025 * speed;
}

int fuel_station_visible(void) {
    if (sprite_y(fuel_station) < screen_width() && sprite_y(fuel_station) > 0) {
        return sprite_x(fuel_station);
    }

    return 0;
}

void update_fuel(void) {
    double change_in_fuel = -0.15 * dy;
    fuel(change_in_fuel);
}

double station_x(void) {
    int orientation = rand() % 2;
    double fuel_x;

    if (orientation == 1) {
        fuel_x = offroad_left() - 10; // 19 - 9
    }
    else {
        fuel_x = offroad_right() + 1;
    }

    return fuel_x;
}

void station_to_top(void) {
    sprite_move_to(fuel_station, station_x(), -1 * sprite_height(fuel_station));
}


bool fuel_timer(void) {
    static time_t counter = 0;
    counter += 10;

    if (counter == 3000) {
        counter = 0;
        return true;
    }

    return false;
}

void refuel(void) {
    if (speed == 0) {
        if (fuel_timer()) {
            fuel(100);
        }
    }
}

void refuel_car_left(void) {
    int fuel_left = round(sprite_x(fuel_station));
    int fuel_right = fuel_left + sprite_width(fuel_station) - 1;
    int fuel_top = round(sprite_y(fuel_station));
    // int fuel_bottom = fuel_top + sprite_height(fuel_station) - 1;

    int car_left = round(sprite_x(driver));
    // int car_right = car_left + sprite_width(driver) - 1;
    int car_top = round(sprite_y(driver));
    // int car_bottom = car_top + sprite_height(driver) - 1;

    /// if station on left
    if (((car_left - 2 == fuel_right) || (car_left - 1 == fuel_right)) && (car_top == fuel_top)) {
        refuel();
    }
}



void refuel_car_right(void) {

    int fuel_left = round(sprite_x(fuel_station));
    // int fuel_right = fuel_left + sprite_width(fuel_station) - 1;
    int fuel_top = round(sprite_y(fuel_station));
    // int fuel_bottom = fuel_top + sprite_height(fuel_station) - 1;

    int car_left = round(sprite_x(driver));
    int car_right = car_left + sprite_width(driver) - 1;
    int car_top = round(sprite_y(driver));
    // int car_bottom = car_top + sprite_height(driver) - 1;

    if (((car_right == fuel_left - 2) || (car_right == fuel_left - 1)) && (car_top == fuel_top)) {
        refuel();
    }

}



void check_station(void) {
    static bool on_screen = false;

    if (on_screen) {
        if (sprite_x(fuel_station) < screen_width() / 2) {
            refuel_car_left();
        }
        else {
            refuel_car_right();
        }

        // check for when station goes offscreen,,,,
        if (sprite_y(fuel_station) >= screen_height()) {
            on_screen = false;
        }

    }

    else {
        int chance_station_appears = 100 - fuel(0);
        double d_modulus_number = -0.4814 * fuel(0) * fuel(0) + 102.25 * fuel(0) - 335.5; /// equation formed using trial and error and excel (shoutout excel)
        int modulus_number = (int)d_modulus_number;
        int percent = rand() % modulus_number;

        if (percent < chance_station_appears) {
            // make station show up
            station_to_top();
            on_screen = true;
        }
    }

}

void step_station(void) {
    sprite_turn_to(fuel_station, 0, dy);
    sprite_step(fuel_station);
    sprite_draw(fuel_station);
}

void step_driver( int key ) {
    if (( key == 'j' || key == 'J') && speed > 0) {
        if ( sprite_x( driver ) > 1 ) {
            sprite_move( driver, -1, 0 );
        }
    }
    else if (( key == 'l' || key == 'L') && speed > 0) {
        if ( sprite_x( driver ) < screen_width() - sprite_width(driver) - 2 ) {
            sprite_move( driver, +1, 0 );
        }
    }
}

double get_elapsed_time(void) {
    double current_time = get_current_time();
    double elapsed_time = current_time - initial_time;
    elapsed_time = round(elapsed_time * 100) / 100;

    return elapsed_time;
}

void draw_offroad_sections( void ) {
    // draw road & off road section
    draw_line(offroad_left(), 3, offroad_left(), screen_height() - 2, '|'); // 2 for the dashboard
    draw_line(offroad_right(), 3, offroad_right(), screen_height() - 2, '|');
}

int pick_number_obstacles(void) {
    if (screen_width() < 90) {
        return 5;
    }
    else if (screen_width() < 120) {
        return 6;
    }
    else if (screen_width() < 160) {
        return 7;
    }
    else if (screen_width() < 200) {
        return 8;
    }
    else {
        return 10;
    }
}

void draw_borders(void) {
    draw_line(0, 0, screen_width() - 1, 0, '*'); // top
    draw_line(screen_width() - 1, 0, screen_width() - 1 , screen_height() - 1, '*'); // right
    draw_line(screen_width() - 1, screen_height() - 1, 0, screen_height() - 1, '*'); // bottom
    draw_line(0, screen_height() - 1, 0, 0, '*'); // left
}

void check_for_game_over(void) {
    // check collision
    for (int i = 0; i < total_obstacles(0); i++) {
        bool collision = collided(driver, active_objects[i], get_sprite_image(sprite_width(active_objects[i])));
        if (collision) {
            car_damage(1);
            if (car_damage(0) < 1) {
                game_running(1);
            }
            else {
                relocate_car();
            }
        }
    }

    // check for collision with fuel station
    bool fuel_collision = collided(driver, fuel_station, fuel_station_img());
    if (fuel_collision) {
        game_running(1);
    }

    // check fuel
    if (fuel(0) < 1) {
        game_running(1);
    }
}

void draw_dashboard(void) {
    // draw borders
    draw_borders();

    // draw dashboard
    draw_line(0, 2, screen_width() - 1, 2, '*'); // draw line x1 = 0 to x2 = 0 will just have a single -
    draw_line(1, 1, screen_width() - 1 - 1, 1, ' '); /// keep gaps eerrryrwhere

    // draw time
    draw_formatted(2, 1, "Time: %.02fs", get_elapsed_time()); // 14 characters + whatever the time is, 4 characters min

    // draw distance travelled
    draw_formatted(screen_width() / 4, 1, "Distance: %.fkm", distance_travelled(0)); // DISTANCE TRAVELLED // 20 + distance chars

    // draw speed
    draw_formatted(screen_width() / 2, 1, "Speed: %.f", speed); /// speeeeeeeeeeed // 15 characters + speed

    // draw car condition
    draw_formatted(2 * screen_width() / 3, 1, "Health: %d", car_damage(0));

    // draw fuel left
    draw_formatted(screen_width() - 9 - 2, 1, "Fuel: %.f", fuel(0)); // fewel // 11 charactes + fuel (3 max)

}

/*-----------------------------------------------------------------------------------------------------------------*/

void splash_screen(void) {

    draw_borders();


    char * instructions[20] = {
    "                      Just Your Average Racecar Game                     ",
    "                    Created by: Annette Yin (n9632590)                   ",
    "                                                                         ",
    "YOU ARE RUNNING AWAY FROM ZOMBIES IN A CAR THAT LOOKS SUSPICIOUSLY LIKE A",
    "BUNCH OF TEXT. BUT IT'S NOT. I SWEAR.                                    ",
    "                                                                         ",
    "YOU WILL DIE IF:                                                         ",
    " - you hit too many things (if you hit a fuel station you will die       ",
    "   immediately so don't do that)                                         ",
    " - you run out of fuel (park next to a fuel tank for 3 seconds to refuel)",
    "                                                                         ",
    "YOU WILL NOT DIE IF:                                                     ",
    " - you drive lots and lots and reach zombie mountain (don't ask me why.  ",
    "   i don't know. it's just a rule. accept it and move on.)               ",
    "                                                                         ",
    "                                CONTROLS                                 ",
    "                              i to speed up                              ",
    "          j to move left     k to slow down      l to move right         ",
    "                                                                         ",
    "                        PRESS ANY KEY TO PLAY                            "};


    for (int i = 0; i < 20; i++) {
        draw_string((screen_width() - 73)/ 2, (screen_height() - 20) / 2 + i, instructions[i]);
    }

    show_screen();
    wait_char();

}

void game_over(void) {

    if (game_won(0)) {
        char * message[9] = {
            "     *    /\\              _             *     /\\              ",
            "         /  \\        .   ((      YOU WON !   /  \\     *       ",
            "  .     /    \\   /\\       `                 /\\/\\/\\            ",
            "       /~~~~~~\\ /  \\                 *     /      \\           ",
            "      /  /\\    /\\/\\/\\                     /   /\\   \\          ",
            "     /  /  \\  /      \\   *        .      /   /~~\\   \\   .     ",
            "*   /  /    \\/ /\\     \\                 /\\  /    \\   \\        ",
            "   /  /      \\/  \\/\\   \\               /  \\/      \\   \\       ",
            "__/__/_______/___/__\\___\\_____________/____\\_______\\___\\______",
            // "                                                              "
        };

        for (int i = 0; i < 9; i++) {
            draw_string((screen_width() - 64) / 2, (screen_height() - 9) / 2 + i, message[i]);
        }
        draw_formatted((screen_width() - 64) / 2, screen_height() / 2, "You wasted: %.02f seconds of your life.", get_elapsed_time());
        draw_formatted((screen_width() - 64) / 2 + 8, screen_height() / 2 + 1, "You travelled: %.fkm", distance_travelled(0));
        draw_formatted(screen_width() / 2 + 4, screen_height() / 2 + 2, "Play again? Y/N");
        draw_string(screen_width() - 71, screen_height() - 2, "It's fine, I've wasted what feels like my entire life making this game");
    }

    else {

        char * message[9] = {
            "     *    /\\              _             *     /\\              ",
            "         /  \\        .   ((     YOU LOST !   /  \\     *       ",
            "  .     /    \\   /\\       `                 /\\/\\/\\            ",
            "       /~~~~~~\\ /  \\                  *    /      \\           ",
            "      /  /\\    /\\/\\/\\                     /   /\\   \\          ",
            "     /  /  \\  /      \\   *        .      /   /~~\\   \\   .     ",
            "*   /  /    \\/ /\\     \\                 /\\  /    \\   \\        ",
            "   /  /      \\/  \\/\\   \\               /  \\/      \\   \\       ",
            "__/__/_______/___/__\\___\\_____________/____\\_______\\___\\______",
            // "                                                              "
        };

        for (int i = 0; i < 9; i++) {
            draw_string((screen_width() - 64) / 2, (screen_height() - 9) / 2 + i, message[i]);
        }
        draw_formatted((screen_width() - 64) / 2, screen_height() / 2, "You wasted: %.02f seconds of your life.", get_elapsed_time());
        draw_formatted((screen_width() - 64) / 2 + 8, screen_height() / 2 + 1, "You travelled: %.fkm", distance_travelled(0));
        draw_formatted(screen_width() / 2 + 4, screen_height() / 2 + 2, "Play again? Y/N");
        draw_string(screen_width() - 71, screen_height() - 2, "It's fine, I've wasted what feels like my entire life making this game");

    }

    draw_borders();
    show_screen();

}

void victory_screen() {
    bool obstacles_visible = true;
    while (obstacles_visible || (sprite_y(fuel_station) < screen_height() && sprite_y(fuel_station) > 0)) {
        draw_offroad_sections();

        int count = 0;

        int key = get_char();
        step_driver(key);
        change_speed(key);

        for (int i = 0; i < total_obstacles(0); i++) {
            if (sprite_y(active_objects[i]) < screen_height()) {
                step_object(active_objects[i]);
            }
            else {
                count++;
            }
        }

        if (count == total_obstacles(0)) {
            obstacles_visible = false;
        }

        check_for_game_over();


        sprite_draw(driver);

        // update dashboard
        distance_travelled(1);
        update_fuel();

        /// fuel stuff would go here i guess...?
        step_station();

        draw_dashboard();

        show_screen();
        timer_pause(10);
        clear_screen();
    }

    /*

     "     *    /\              _             *     /\              "
     "         /  \        .   ((                  /  \     *       "
     "  .     /    \   /\       `                 /\/\/\            "
     "       /~~~~~~\ /  \                 *     /      \           "
     "      /  /\    /\/\/\                     /   /\   \          "
     "     /  /  \  /      \   *        .      /   /~~\   \   .     "
     "*   /  /    \/ /\     \                 /\  /    \   \         "
     "   /  /      \/  \/\   \               /  \/      \   \       "
     "__/__/_______/___/__\___\_____________/____\_______\___\______"

     */

    char* finish_img = "(This line represents zombie mountain)";

    sprite_id finish = sprite_create(screen_width() - 40, -2, 38, 1, finish_img);
    sprite_turn_to(finish, 0, dy);
    bool animation_done = false;
    int line_position_y;


    while (!animation_done) {
        draw_offroad_sections();

        sprite_hide(fuel_station);

        line_position_y = sprite_y(finish) + 2;
        draw_line(0, line_position_y, screen_width(), line_position_y, '_');

        step_object(finish);
        if (sprite_y(finish) + 1 >= sprite_y(driver)) {
            animation_done = true;
        }

        check_for_game_over();

        sprite_draw(driver);

        // update dashboard
        distance_travelled(1);
        update_fuel();

        /// fuel stuff would go here i guess...?
        step_station();
        draw_dashboard();

        show_screen();
        timer_pause(10);
        clear_screen();



    }
}


void reset_everything(void) {
    clear_screen();
    speed = 0;
    car_damage(100);
    distance_travelled(100);
    fuel(100);
    total_obstacles(100);
    game_won(100);
}

void setup_game( void ) {
    // setup screen
    setup_screen();

    // reset errythang
    reset_everything();

    //get width and height
    double width = screen_width();
    double height = screen_height();

    // seed time
    time_t now = time(NULL);
    srand(now);

    // set time of game start (used for timer)
    initial_time = get_current_time();

    // create driver sprite
    driver = sprite_create(width / 2 - (3), height - (height / 4), 8, 6, usercar()); // 3 bc half of width of driver
    if (height <= 4 * sprite_height(driver)) {
        sprite_move_to(driver, width/2 - 3, height - sprite_height(driver) - 1);
    }

    // create fuel sprite
    fuel_station = sprite_create(0, screen_height(), 10, 6, fuel_station_img()); // start off screen

    // create objects
    int number_of_obstacles = pick_number_obstacles();
    for (int i = 0; i < number_of_obstacles; i++) {
        active_objects[i] = pick_object(true);
        total_obstacles(1);
    }

    bool overlap_present = test_overlap();
    while (overlap_present) {
        remove_overlap(true);
        overlap_present = test_overlap();
    }

    sprite_show(fuel_station);
    show_screen();

}


void process( void ) {
    draw_offroad_sections();

    if (distance_travelled(0) >= 2000) {
        game_won(1);
        game_running(1);
    }

    // check for key press
    int key = get_char();
    step_driver(key);
    change_speed(key);

    // replenish sprites
    replace_sprite();
    for (int i = 0; i < total_obstacles(0); i++) {
        step_object(active_objects[i]);
    }

    check_for_game_over();

    // draw car
    sprite_draw(driver);

    // update dashboard
    distance_travelled(1);
    update_fuel();

    /// fuel stuff would go here i guess...?
    check_station();
    step_station();


    draw_dashboard();

    show_screen();
    clear_screen();


}

int main ( void ) {
    bool program_running = true;
    bool running_game = game_running(0);


    while (program_running) {
        setup_game();
        splash_screen();


        while ( running_game ) {
            process();
            running_game = game_running(0);
            timer_pause(10);
        }

        if (game_won(0) == true) {
            victory_screen();
        }

        game_over();


        int key = wait_char();
        while(!(key == 'y' || key == 'Y'||key == 'n' || key == 'N' )){
            key = wait_char();

        }
        if (key == 'y' || key == 'Y') {
            clear_screen();
            running_game = game_running(1);
            program_running = true;
        }
        else if (key == 'n' || key == 'N' ) {
            program_running = false;
        }




    }

}
