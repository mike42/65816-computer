; snake.s: a simple game to test the computer
.import uart_printz
ROM_PRINT_CHAR   := $00

.segment "START"
start:                              ; entry point
    jmp __main

.segment "CODE"
;#include <curses.h>
;#include <unistd.h>
;#include <string.h>
;
;#define DIR_UP 0
;#define DIR_RIGHT 1
;#define DIR_DOWN 2
;#define DIR_LEFT 3
;#define FRAME_DELAY_US 100000
;#define SNAKE_SEGMENT_MAX 511
;
;#define X_MAX 79
;#define Y_MAX 23
;
;struct snake_segment {
;    uint8_t x;
;    uint8_t y;
;};
;
;uint16_t random_state;
;
;// game state
;uint8_t snake_dir;
;size_t snake_head_id;
;size_t snake_tail_id;
;bool game_lost;
;
;struct snake_segment snake_segments[SNAKE_SEGMENT_MAX + 1];
;
;void place_fruit();
;
;uint8_t fruit_x;
;uint8_t fruit_y;
;
;void snake_update_direction()
; TODO unported code
snake_update_direction:
;    int ch = getch();
;    while (ch != -1) {
;        if (ch == 'w' && snake_dir != DIR_DOWN) {
;            snake_dir = DIR_UP;
;        } else if (ch == 'a' && snake_dir != DIR_RIGHT) {
;            snake_dir = DIR_LEFT;
;        } else if (ch == 's' && snake_dir != DIR_UP) {
;            snake_dir = DIR_DOWN;
;        } else if (ch == 'd' && snake_dir != DIR_LEFT) {
;            snake_dir = DIR_RIGHT;
;        } else if (ch == '\n' || ch == KEY_ENTER || ch == ' ' || ch == 'q') {
;            game_lost = true;
;        }
;        ch = getch();
;    }
    rts
;
;void draw_fruit()
; TODO unported code
__draw_fruit:
;    move(fruit_y, fruit_x);
;    printw("%c", '%');
    rts

; void snake_advance()
; TODO unported code
__snake_advance:
;    struct snake_segment old_snake_head = snake_segments[snake_head_id];
;    struct snake_segment new_snake_head = old_snake_head;
;    char snake_head_char = 'x';
;    if (snake_dir == DIR_RIGHT) {
;        new_snake_head.x = old_snake_head.x == X_MAX ? 0 : old_snake_head.x + 1;
;        snake_head_char = '<';
;    } else if (snake_dir == DIR_LEFT) {
;        new_snake_head.x = old_snake_head.x == 0 ? X_MAX : old_snake_head.x - 1;
;        snake_head_char = '>';
;    } else if (snake_dir == DIR_UP) {
;        new_snake_head.y = old_snake_head.y == 0 ? Y_MAX : old_snake_head.y - 1;
;        snake_head_char = 'v';
;    } else if (snake_dir == DIR_DOWN) {
;        new_snake_head.y = old_snake_head.y == Y_MAX ? 0 : old_snake_head.y + 1;
;        snake_head_char = '^';
;    }
;    // Collision?
;    size_t i;
;    for(i = 0; i <= SNAKE_SEGMENT_MAX; i++) {
;        if(snake_segments[i].x == new_snake_head.x && snake_segments[i].y == new_snake_head.y) {
;            snake_head_char = 'X';
;            game_lost = true;
;        }
;    }
;    // Advance snake head index
;    if (snake_head_id == SNAKE_SEGMENT_MAX) {
;        snake_head_id = 0;
;    } else {
;        snake_head_id++;
;    }
;    // Replace char in old snake head
;    if (snake_head_id != snake_tail_id) {
;        move(old_snake_head.y, old_snake_head.x);
;        printw("%c", '@');
;    }
;    // Print new snake head
;    move(new_snake_head.y, new_snake_head.x);
;    printw("%c", snake_head_char);
;    // Save new snake head
;    snake_segments[snake_head_id] = new_snake_head;
;
;    struct snake_segment old_snake_tail = snake_segments[snake_tail_id];
;    if(new_snake_head.x == fruit_x && new_snake_head.y == fruit_y) {
;        // Move the fruit!
;        place_fruit();
;        draw_fruit();
;    } else {
;        // fruit can overlap with snake
;        if (old_snake_tail.x != fruit_x || old_snake_tail.y != fruit_y) {
;            // Erase old tail
;            move(old_snake_tail.y, old_snake_tail.x);
;            printw("%c", ' ');
;        }
;        old_snake_head.x = 255;
;        old_snake_tail.y = 255;
;        snake_segments[snake_tail_id] = old_snake_tail;
;        // Advance snake tail index
;        if (snake_tail_id == SNAKE_SEGMENT_MAX) {
;            snake_tail_id = 0;
;        } else {
;            snake_tail_id++;
;        }
;    }
;    move(fruit_y, fruit_x);
    rts

;void snake_frame()
__snake_frame:
    jsr snake_update_direction
    jsr __snake_advance
    rts

;#define RAND_MAX 65535
;void srand(uint16_t value)
; TODO unported code
__srand:
;    random_state = value + 1;
;    if(random_state == 0) {
;        // Random state must never be 0!
;        random_state++;
;    }
    rts

; TODO unported code
;uint16_t rand()
__rand:
;    // This is a 16-bit Xorshift https://en.wikipedia.org/wiki/Xorshift
;    // 7,9,13 triplet is from http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
;    random_state ^= random_state >> 7;
;    random_state ^= random_state << 9;
;    random_state ^= random_state >> 13;
;    return random_state - 1; // to match built-in rand.
    lda #0
    rts

; Equivalent of printw("%s", str);
;__printstr:
;    lda 3, S
;    jsr uart_printz
;    rts

;
; int main
; TODO unported code
__main:
    ; 16-bit accumulator and index registers
    .a16
    .i16
    rep #%00110000
    ; some test code
    lda #'u'
    cop ROM_PRINT_CHAR
    ; srand(42);
    pea 42
    jsr __srand

; Punch this out so we don't get surprises
; TODO unported code
;   memset(snake_segments, 255, sizeof(snake_segments));
;   initscr();
;   raw();
;   noecho();

; Print welcome screen: not having much luck yet but this works-
    lda #$4141
    sta f:$c200

    ; set data bank register to equal program bank register (for accessing constants etc).
;    phk
;    plb
;    See welcome_screen label
;    ldx a:welcome_screen
;    jsr uart_printz
;    const char* scr[13];
;    int i;
;    for(i = 0; i < 13; i++) {
;        move(5 + i, 15);
;        printw("%s", scr[i]);
;    }

; TODO unported code
;    move(0,0);
;    refresh();
;    getch();
;
; Clear screen
; TODO unported code
;    clear();
;    refresh();
;
;    // Change to nonblocking input mode
;    nodelay(stdscr, TRUE);
;
;    // Prepare game state
;    snake_dir = DIR_RIGHT;
;    game_lost = false;
;    snake_head_id = snake_tail_id = 0;
;    snake_segments[snake_head_id].x = 8;
;    snake_segments[snake_head_id].y = 8;
;    place_fruit();
;    draw_fruit();
;
;    while (!game_lost) {
;        snake_frame();
;        refresh();
;        usleep(FRAME_DELAY_US);
;    }
;    nodelay(stdscr, FALSE);
;    // Print score
;    uint16_t score = snake_head_id >= snake_tail_id ? (snake_head_id - snake_tail_id) : ((SNAKE_SEGMENT_MAX - snake_tail_id) + snake_head_id);
;    move(11, 28);
;    printw("[ Game over! Score: %u ]", score);
;
;    // Wait for space or q, then quit
;    bool complete = false;
;    while(true) {
;        char c = getch();
;        if(c == 'q' || c == ' ') {
;            break;
;        }
;    }
;    endwin();
    ; some test code
    lda #'v'
    cop ROM_PRINT_CHAR
    stp

;void place_fruit()
; TODO unported code
__place_fruit:
;    fruit_x = rand() % (X_MAX + 1);
;    fruit_y = rand() % (Y_MAX + 1);
    rts

welcome_screen: .addr welcome_screen_0, welcome_screen_1, welcome_screen_2, welcome_screen_3, welcome_screen_4, welcome_screen_5, welcome_screen_6, welcome_screen_7, welcome_screen_8, welcome_screen_9, welcome_screen_10, welcome_screen_11, welcome_screen_12
welcome_screen_0: .asciiz "    Y\n"
welcome_screen_1: .asciiz "   o^o    Welcome to\n"
welcome_screen_2: .asciiz "   \\ /       ____              _\n"
welcome_screen_3: .asciiz "   | |      / ___| _ __   __ _| | _____\n"
welcome_screen_4: .asciiz "    \\ \\     \\___ \\| '_ \\ / _` | |/ / _ \\\n"
welcome_screen_5: .asciiz "     \\ \\     ___) | | | | (_| |   <  __/\n"
welcome_screen_6: .asciiz "     | |    |____/|_| |_|\\__,_|_|\\_\\___|\n"
welcome_screen_7: .asciiz "    / /\n"
welcome_screen_8: .asciiz "   | |  ____    ____    ____    ____    ____\n"
welcome_screen_9: .asciiz "   \\ \\_/ __ \\__/ __ \\__/ __ \\__/ __ \\__/ __ \\__\n"
welcome_screen_10: .asciiz "    \\___/  \\____/  \\____/  \\____/  \\____/  \\___\\\n"
welcome_screen_11: .asciiz "\n"
welcome_screen_12: .asciiz "           [ Press any key to start ]\n"

