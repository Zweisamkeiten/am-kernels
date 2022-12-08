#include <am.h>
#include <klib.h>
#include <klib-macros.h>

// 每秒绘制帧数
#define FPS            30
// 每秒周期数
#define CPS             5
#define CHAR_W          8
#define CHAR_H         16
// 屏幕上最多128个字符'
#define NCHAR         128
#define COL_WHITE    0xeeeeee
#define COL_RED      0xff0033
#define COL_GREEN    0x00cc33
#define COL_PURPLE   0x2a0a29

enum { WHITE = 0, RED, GREEN, PURPLE };
struct character {
  char ch;
  int x, y, v, t;
} chars[NCHAR];

int screen_w, screen_h, hit, miss, wrong;
uint32_t texture[3][26][CHAR_W * CHAR_H], blank[CHAR_W * CHAR_H];

int min(int a, int b) {
  return (a < b) ? a : b;
}

int randint(int l, int r) {
  return l + (rand() & 0x7fffffff) % (r - l + 1);
}

void new_char() {
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    // 128 屏幕字符数组中有空闲位置, 则生成一个新字符
    if (!c->ch) {
      c->ch = 'A' + randint(0, 25);
      // 横坐标随机
      c->x = randint(0, screen_w - CHAR_W);
      // 纵坐标0, 从顶上往下掉
      c->y = 0;
      // 掉落速度随机
      c->v = (screen_h - CHAR_H + 1) / randint(FPS * 3 / 2, FPS * 2);
      c->t = 0;
      return;
    }
  }
}

void game_logic_update(int frame) {
  // 每6帧生成一个新字符
  if (frame % (FPS / CPS) == 0) new_char();
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    if (c->ch) {
      // 已经掉到底部
      if (c->t > 0) {
        // 自减30帧后消失
        if (--c->t == 0) {
          c->ch = '\0';
        }
      } else { // 未掉到底部
        // 纵坐标增加一个单位
        c->y += c->v;
        if (c->y < 0) {
          // 被击中的, 因为速度为负, 已经上升到屏幕顶部外
          // 清空
          c->ch = '\0';
        }
        // 落到底部, 需要消失
        if (c->y + CHAR_H >= screen_h) {
          miss++;
          c->v = 0;
          // 纵坐标标记在底部
          c->y = screen_h - CHAR_H;
          // 保持30帧不消失
          c->t = FPS;
        }
      }
    }
  }
}

void render() {
  static int x[NCHAR], y[NCHAR], n = 0;

  for (int i = 0; i < n; i++) {
    // 把128个字符 位置用blank填充
    io_write(AM_GPU_FBDRAW, x[i], y[i], blank, CHAR_W, CHAR_H, false);
  }

  n = 0;
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    if (c->ch) {
      // 更新有字符的, 位置.
      x[n] = c->x; y[n] = c->y; n++;
      // 更新颜色
      int col = (c->v > 0) ? WHITE : (c->v < 0 ? GREEN : RED);
      // 写入FBDRAW 显存
      io_write(AM_GPU_FBDRAW, c->x, c->y, texture[col][c->ch - 'A'], CHAR_W, CHAR_H, false);
    }
  }
  // 刷新屏幕 sync
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
  for (int i = 0; i < 40; i++) putch('\b');
  // 更新游戏数据
  printf("Hit: %d; Miss: %d; Wrong: %d", hit, miss, wrong);
}

void check_hit(char ch) {
  int m = -1;
  for (int i = 0; i < LENGTH(chars); i++) {
    struct character *c = &chars[i];
    // 按键与屏幕上的某个字符匹配, 且该字符正在掉落
    // m < 0 判断第一次匹配, || 要找到y坐标最大的匹配
    if (ch == c->ch && c->v > 0 && (m < 0 || c->y > chars[m].y)) {
      m = i;
    }
  }
  // 没匹配到, 则错误按键
  if (m == -1) {
    wrong++;
  } else {
    // 击中
    hit++;
    // 则速度变为负, 从下降状态到上升状态
    chars[m].v = -(screen_h - CHAR_H + 1) / (FPS);
  }
}


void video_init() {
  // 设置屏幕宽高
  screen_w = io_read(AM_GPU_CONFIG).width;
  screen_h = io_read(AM_GPU_CONFIG).height;

  extern char font[];
  // blank 是一个字符的像素点阵 8 * 16, 为每个像素都初始化为紫色
  for (int i = 0; i < CHAR_W * CHAR_H; i++)
    blank[i] = COL_PURPLE;

  // 将屏幕用紫色的blank填满, 边缘如果不够CHAR_W, CHAR_H, 裁剪后填满
  for (int x = 0; x < screen_w; x += CHAR_W)
    for (int y = 0; y < screen_h; y += CHAR_H) {
      io_write(AM_GPU_FBDRAW, x, y, blank, min(CHAR_W, screen_w - x), min(CHAR_H, screen_h - y), false);
    }

  // 这里每个字符高为16个点，宽为8个点。
  // 因此单个字符可以用16个8bit数来表示，
  // 每个8bit数代表字符的一行，
  // 对应的点为 1 时显示有颜色，
  // 为 0 时显示空颜色。
  //
  // 为26个字母的制作字符点阵
  for (int ch = 0; ch < 26; ch++) {
    // 赋值c指针到font数组每行的开头, 表示第几个字母
    char *c = &font[CHAR_H * ch];
    for (int i = 0, y = 0; y < CHAR_H; y++)
      for (int x = 0; x < CHAR_W; x++, i++) {
        // 按行优先的顺序, 每个8bit数, 分别制作白绿红三种字模
        int t = (c[y] >> (CHAR_W - x - 1)) & 1;
        texture[WHITE][ch][i] = t ? COL_WHITE : COL_PURPLE;
        texture[GREEN][ch][i] = t ? COL_GREEN : COL_PURPLE;
        texture[RED  ][ch][i] = t ? COL_RED   : COL_PURPLE;
      }
  }
}

char lut[256] = {
  [AM_KEY_A] = 'A', [AM_KEY_B] = 'B', [AM_KEY_C] = 'C', [AM_KEY_D] = 'D',
  [AM_KEY_E] = 'E', [AM_KEY_F] = 'F', [AM_KEY_G] = 'G', [AM_KEY_H] = 'H',
  [AM_KEY_I] = 'I', [AM_KEY_J] = 'J', [AM_KEY_K] = 'K', [AM_KEY_L] = 'L',
  [AM_KEY_M] = 'M', [AM_KEY_N] = 'N', [AM_KEY_O] = 'O', [AM_KEY_P] = 'P',
  [AM_KEY_Q] = 'Q', [AM_KEY_R] = 'R', [AM_KEY_S] = 'S', [AM_KEY_T] = 'T',
  [AM_KEY_U] = 'U', [AM_KEY_V] = 'V', [AM_KEY_W] = 'W', [AM_KEY_X] = 'X',
  [AM_KEY_Y] = 'Y', [AM_KEY_Z] = 'Z',
};

int main() {
  // 初始化ioe, am/src/platform/nemu/ioe.c  gpu_init, audio_init, timer_init
  ioe_init();
  // 初始化 video
  video_init();

  panic_on(!io_read(AM_TIMER_CONFIG).present, "requires timer");
  panic_on(!io_read(AM_INPUT_CONFIG).present, "requires keyboard");

  printf("Type 'ESC' to exit\n");

  int current = 0, rendered = 0;
  while (1) {
    // 每秒只绘制30帧, 程序启动后现在这一秒结束总共要绘制多少帧
    int frames = io_read(AM_TIMER_UPTIME).us / (1000000 / FPS);

    // 当前秒内逐帧进行逻辑更新
    for (; current < frames; current++) {
      game_logic_update(current);
    }

    while (1) {
      // 读取键码
      AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
      // 没有按键 break
      if (ev.keycode == AM_KEY_NONE) break;
      // 退出
      if (ev.keydown && ev.keycode == AM_KEY_ESCAPE) halt(0);
      // 有按键, 且在26个键码内, 则检查是否有击中
      if (ev.keydown && lut[ev.keycode]) {
        check_hit(lut[ev.keycode]);
      }
    };

    // 如果当前已经判断过的帧还未绘制
    if (current > rendered) {
      // 重新绘制
      render();
      rendered = current;
    }
  }
}
