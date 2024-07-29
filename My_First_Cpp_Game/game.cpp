#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#include <cmath>

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y;
float ball_sum_speed = fabs(ball_dp_x) + fabs(ball_dp_y) + 10;
float ball_half_size = 1;

int player_1_score, player_2_score;

internal void
simulate_player(float* p, float* dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
	GM_CHALLANGE_RATE,
	GM_MAX_SCORE,
	GM_END_GAME,
};

Gamemode current_gamemode;
int hot_button = 0;
bool enemy_is_ai;
int hard_rate = 0;
int boink_right = 0;
int boink_left = 0;
int max_rate = 0;

internal void
simulate_game(Input* input, float dt) {
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);
	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0xff5500);

	if (current_gamemode == GM_MENU) {

		if (pressed(BUTTON_LEFT)) {
			if (hot_button == 1)
			{
				hot_button = 0;
			}
		}
		else if (pressed(BUTTON_RIGHT))
		{
			if (hot_button == 0)
			{
				hot_button = 1;
			}
		}

		if (pressed(BUTTON_ENTER)) {
			if (hot_button == 0)
			{
				current_gamemode = GM_CHALLANGE_RATE;
			}
			else {
				current_gamemode = GM_MAX_SCORE;
			}
			enemy_is_ai = hot_button ? 0 : 1;
			hot_button = 0;
		}

		if (hot_button == 0) {
			draw_text("JKJ", 15, 0, 1, 0xcccccc);
			draw_text("QKJ", -40, 0, 1, 0xff0000);
		}
		else {
			draw_text("JKJ", 15, 0, 1, 0xff0000);
			draw_text("QKJ", -40, 0, 1, 0xcccccc);
		}

		draw_text("CHOOSE THE GAME", -31, 30, 1, 0x00aaff);
	}
	else if (current_gamemode == GM_CHALLANGE_RATE) {

		if (pressed(BUTTON_UP)) {
			if (hot_button == 2)
			{
				hot_button = 1;
			}
			else if (hot_button == 1)
			{
				hot_button = 0;
			}
			else if (hot_button == 0)
			{
				hot_button = 2;
			}
		}
		else if (pressed(BUTTON_DOWN))
		{
			if (hot_button == 0)
			{
				hot_button = 1;
			}
			else if (hot_button == 1)
			{
				hot_button = 2;
			}
			else if (hot_button == 2)
			{
				hot_button = 0;
			}
		}

		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_MAX_SCORE;
			hard_rate = hot_button;
			hot_button = 0;
		}

		if (hot_button == 0) {
			draw_text("EASY", -20, 7, 1, 0xff0000);
			draw_text("NORMAL", -20, 0, 1, 0xcccccc);
			draw_text("HARD", -20, -7, 1, 0xcccccc);
		}
		else if (hot_button == 1)
		{
			draw_text("EASY", -20, 7, 1, 0xcccccc);
			draw_text("NORMAL", -20, 0, 1, 0xff0000);
			draw_text("HARD", -20, -7, 1, 0xcccccc);
		}
		else
		{
			draw_text("EASY", -20, 7, 1, 0xcccccc);
			draw_text("NORMAL", -20, 0, 1, 0xcccccc);
			draw_text("HARD", -20, -7, 1, 0xff0000);
		}

		draw_text("CHOOSE THE DIFFICULTY", -43, 30, 1, 0x00aaff);
	}
	else if (current_gamemode == GM_MAX_SCORE) {

		if (pressed(BUTTON_UP)) {
			if (hot_button == 4)
			{
				hot_button = 3;
			}
			else if (hot_button == 3)
			{
				hot_button = 2;
			}
			else if (hot_button == 2)
			{
				hot_button = 1;
			}
			else if (hot_button == 1)
			{
				hot_button = 0;
			}
			else if (hot_button == 0)
			{
				hot_button = 4;
			}
		}
		else if (pressed(BUTTON_DOWN))
		{
			if (hot_button == 0)
			{
				hot_button = 1;
			}
			else if (hot_button == 1)
			{
				hot_button = 2;
			}
			else if (hot_button == 2)
			{
				hot_button = 3;
			}
			else if (hot_button == 3)
			{
				hot_button = 4;
			}
			else if (hot_button == 4)
			{
				hot_button = 0;
			}
		}

		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
			max_rate = hot_button;
			hot_button = 0;
		}

		if (hot_button == 0) {
			draw_text("NO LIMITS", -20, 14, 1, 0xff0000);
			draw_text("10", -20, 7, 1, 0xcccccc);
			draw_text("25", -20, 0, 1, 0xcccccc);
			draw_text("50", -20, -7, 1, 0xcccccc);
			draw_text("100", -20, -14, 1, 0xcccccc);
		}
		else if (hot_button == 1) {
			draw_text("NO LIMITS", -20, 14, 1, 0xcccccc);
			draw_text("10", -20, 7, 1, 0xff0000);
			draw_text("25", -20, 0, 1, 0xcccccc);
			draw_text("50", -20, -7, 1, 0xcccccc);
			draw_text("100", -20, -14, 1, 0xcccccc);
		}
		else if (hot_button == 2) {
			draw_text("NO LIMITS", -20, 14, 1, 0xcccccc);
			draw_text("10", -20, 7, 1, 0xcccccc);
			draw_text("25", -20, 0, 1, 0xff0000);
			draw_text("50", -20, -7, 1, 0xcccccc);
			draw_text("100", -20, -14, 1, 0xcccccc);
		}
		else if (hot_button == 3) {
			draw_text("NO LIMITS", -20, 14, 1, 0xcccccc);
			draw_text("10", -20, 7, 1, 0xcccccc);
			draw_text("25", -20, 0, 1, 0xcccccc);
			draw_text("50", -20, -7, 1, 0xff0000);
			draw_text("100", -20, -14, 1, 0xcccccc);
		}
		else if (hot_button == 4) {
			draw_text("NO LIMITS", -20, 14, 1, 0xcccccc);
			draw_text("10", -20, 7, 1, 0xcccccc);
			draw_text("25", -20, 0, 1, 0xcccccc);
			draw_text("50", -20, -7, 1, 0xcccccc);
			draw_text("100", -20, -14, 1, 0xff0000);
		}

		draw_text("MAX SCORE", -19, 30, 1, 0x00aaff);
	}
	else if (current_gamemode == GM_GAMEPLAY) {
		switch (max_rate)
		{
		case 0: {} break;
		case 1: {
			if (player_1_score == 10 || player_2_score == 10) {
				current_gamemode = GM_END_GAME;
			}
		} break;
		case 2: {
			if (player_1_score == 25 || player_2_score == 25) {
				current_gamemode = GM_END_GAME;
			}
		} break;
		case 3: {
			if (player_1_score == 50 || player_2_score == 50) {
				current_gamemode = GM_END_GAME;
			}
		} break;
		case 4: {
			if (player_1_score == 100 || player_2_score == 100) {
				current_gamemode = GM_END_GAME;
			}
		} break;
		}

		float player_1_ddp = 0.f;
		if (is_down(BUTTON_UP)) player_1_ddp += 2000;
		if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;

		float player_2_ddp = 0.f;
		if (!enemy_is_ai) {
			if (is_down(BUTTON_W)) player_2_ddp += 2000;
			if (is_down(BUTTON_S)) player_2_ddp -= 2000;
		}
		else {
			player_2_ddp = (ball_p_y - player_2_p) * 100;
			switch (hard_rate)
			{
			case 0: {
				if (player_2_ddp > 1000) player_2_ddp = 1000;
				if (player_2_ddp < -1000) player_2_ddp = -1000;
			}break;
			case 1: {
				if (player_2_ddp > 1300) player_2_ddp = 1300;
				if (player_2_ddp < -1300) player_2_ddp = -1300;
			}break;
			case 2: {
				if (player_2_ddp > 2000) player_2_ddp = 2000;
				if (player_2_ddp < -2000) player_2_ddp = -2000;
			}break;

			}
		}

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

		// Simulate ball
		{
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;

			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
				if (boink_right == boink_left) boink_right++;
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_sum_speed = 100 + (boink_right * 20) + (boink_left * 20);
				if (ball_sum_speed * .7 > fabs(player_1_dp)) {
					ball_dp_y = player_1_dp;
				}
				else {
					if (player_1_dp < 0) {
						ball_dp_y = ball_sum_speed * .7 * -1;
					}
					else {
						ball_dp_y = ball_sum_speed * .7;
					}
				}
				ball_dp_x = ball_sum_speed * (-1) + fabs(ball_dp_y);
			}
			else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
				if (boink_left == boink_right - 1) boink_left++;
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_sum_speed = 100 + (boink_right * 20) + (boink_left * 20);
				if (ball_sum_speed * .7 > fabs(player_2_dp)) {
					ball_dp_y = player_2_dp;
				}
				else {
					if (player_2_dp < 0) {
						ball_dp_y = ball_sum_speed * .7 * -1;
					}
					else {
						ball_dp_y = ball_sum_speed * .7;
					}
				}
				ball_dp_x = ball_sum_speed - fabs(ball_dp_y);
			}
			else if (ball_p_y + ball_half_size > arena_half_size_y) {
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			else if (ball_p_y - ball_half_size < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}
			else if (ball_p_x + ball_half_size > arena_half_size_x) {
				ball_p_x = 40;
				ball_p_y = 0;
				ball_dp_x = -100.f;
				ball_dp_y = 0.f;
				ball_sum_speed = fabs(ball_dp_x) + fabs(ball_dp_y);
				player_1_score++;
				boink_left = 0;
				boink_right = 0;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x) {
				ball_p_x = -40;
				ball_p_y = 0;
				ball_dp_x = 100.f;
				ball_dp_y = 0.f;
				ball_sum_speed = fabs(ball_dp_x) + fabs(ball_dp_y);
				player_2_score++;
				boink_left = 0;
				boink_right = 0;
			}

		}

		draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
		draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);

		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);

		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000);
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000);

		}
	else {

		if (pressed(BUTTON_LEFT)) {
			if (hot_button == 1)
			{
				hot_button = 0;
			}
		}
		else if (pressed(BUTTON_RIGHT))
		{
			if (hot_button == 0)
			{
				hot_button = 1;
			}
		}

		if (pressed(BUTTON_ENTER)) {
			if (hot_button == 0)
			{
				current_gamemode = GM_GAMEPLAY;
			}
			else {
				current_gamemode = GM_MENU;
			}
			player_1_score = 0;
			player_2_score = 0;
			hot_button = 0;
			player_1_p = 0;
			player_1_dp = 0;
			player_2_p = 0;
			player_2_dp = 0;
			ball_p_x = 0;
		}

		if (hot_button == 0) {
			draw_text("AGAIN", -40, 0, 1, 0xff0000);
			draw_text("MENU", 20, 0, 1, 0xcccccc);
		}
		else {
			draw_text("AGAIN", -40, 0, 1, 0xcccccc);
			draw_text("MENU", 20, 0, 1, 0xff0000);
		}

		if (!enemy_is_ai) {
			if (player_1_score > player_2_score) {
				draw_text("1 PLAYER WON", -26, 30, 1, 0x00aaff);
			}
			else {
				draw_text("2 PLAYER WON", -26, 30, 1, 0x00aaff);
			}
		}
		else {
			if (player_1_score > player_2_score) {
				draw_text("THE BOT WON", -22, 30, 1, 0x00aaff);
			}
			else {
				draw_text("THE PLAYER WON", -28, 30, 1, 0x00aaff);
			}
		}
	}

}