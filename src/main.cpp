/*
    License for Butano:

    Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
    zlib License, see LICENSE file.
*/

// Butano libraries
#include "bn_core.h" // Core libraries.
#include "bn_log.h"
#include "bn_sram.h"
#include "bn_music.h"
#include "bn_music_actions.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_sram.h"
#include "bn_math.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_random.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_ptr.h"
#include "common_info.h"
#include "common_variable_8x8_sprite_font.h"

#include "bn_sprite_items_paddle.h"
#include "bn_sprite_items_ball.h"
#include "bn_regular_bg_items_bg.h"

#include "bn_core.h" 
#include "bn_log.h"
#include "bn_sram.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_math.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_random.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_paddle.h"
#include "bn_sprite_items_ball.h"
#include "bn_regular_bg_items_bg.h"
#include "common_variable_8x8_sprite_font.h"

#include "bn_core.h"
#include "bn_log.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_sprite_items_paddle.h"
#include "bn_sprite_items_ball.h"
#include "bn_regular_bg_items_bg.h"
#include "bn_sprite_text_generator.h"
#include "common_variable_8x8_sprite_font.h"

int main()
{
    bn::core::init();

    // Background music
    bn::music_items::amayadori.play(1);

    // Create background
    bn::regular_bg_ptr bg = bn::regular_bg_items::bg.create_bg(0, 0);

    // Create paddle sprite
    bn::sprite_ptr paddle = bn::sprite_items::paddle.create_sprite(0, 30); // Paddle at bottom
    //paddle.set_scale(1.5); // Enlarge paddle
    paddle.set_z_order(1); // Ensure paddle is on top layer

    // Create ball sprite
    bn::sprite_ptr ball = bn::sprite_items::ball.create_sprite(0, -30); // Ball starts at center

    // Text generator for score display
    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    bn::vector<bn::sprite_ptr, 16> text_sprites;

    // Initial score and text
    int score = 0;
    text_generator.generate(-6 * 16, -68, "(Press A to start)", text_sprites);

    // Ball movement variables
    int delta_x = 0;
    int delta_y = 0;

    bn::random random; // Random number generator

    while (true)
    {
        // Paddle movement
        if (bn::keypad::left_held() && paddle.x() > -100)
        {
            paddle.set_x(paddle.x() - 2); // Move left
        }
        else if (bn::keypad::right_held() && paddle.x() < 100)
        {
            paddle.set_x(paddle.x() + 2); // Move right
        }

        // Start ball movement
        if (bn::keypad::a_pressed() && delta_x == 0 && delta_y == 0)
        {
            text_sprites.clear();
            bn::string<32> txt_score = "Score: " + bn::to_string<32>(score);
            text_generator.generate(-6 * 16, -68, txt_score, text_sprites);

            // Set random initial ball motion
            while (delta_x == 0 || delta_y == 0)
            {
                delta_x = (random.get_int() % 5) - 2;
                delta_y = (random.get_int() % 5) + 2;
            }
        }

        // Update ball position
        ball.set_x(ball.x() + delta_x);
        ball.set_y(ball.y() + delta_y);

        // Handle ball collisions
        if (ball.x() < -112 || ball.x() > 112)
        {
            delta_x = -delta_x; // Reverse horizontal direction
        }
        if (ball.y() < -64)
        {
            delta_y = -delta_y; // Reverse vertical direction
        }
        if (ball.y() > 50 && delta_y > 0 && bn::abs(ball.x() - paddle.x()) < 32)
        {
            delta_y = -delta_y; // Bounce off paddle
            score++;
            text_sprites.clear();
            bn::string<32> txt_score = "Score: " + bn::to_string<32>(score);
            text_generator.generate(-6 * 16, -68, txt_score, text_sprites);
        }
        if (ball.y() > 72)
        {
            ball.set_position(0, 0); // Reset ball position
            delta_x = 0;
            delta_y = 0;
            score--;
            text_sprites.clear();
            bn::string<32> txt_score = "Score: " + bn::to_string<32>(score) + " (Press A)";
            text_generator.generate(-6 * 16, -68, txt_score, text_sprites);
        }

        bn::core::update(); // Update screen
    }
}
