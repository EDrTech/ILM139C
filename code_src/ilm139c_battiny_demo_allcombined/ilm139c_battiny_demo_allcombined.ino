#include <Illumicro.h>
#include <Arduino.h>

// Constants
const int ROWS = 9;
const int COLS = 13;
const int BUTTON_PIN = PIN_PA7;

// Digit patterns for 0-9 (3x5 pixels) for trailing demo FPS display
const uint8_t digit_patterns[10][5][3] = {
  {{1,1,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,1,1}}, // 0
  {{0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}}, // 1
  {{1,1,1}, {0,0,1}, {1,1,1}, {1,0,0}, {1,1,1}}, // 2
  {{1,1,1}, {0,0,1}, {1,1,1}, {0,0,1}, {1,1,1}}, // 3
  {{1,0,1}, {1,0,1}, {1,1,1}, {0,0,1}, {0,0,1}}, // 4
  {{1,1,1}, {1,0,0}, {1,1,1}, {0,0,1}, {1,1,1}}, // 5
  {{1,1,1}, {1,0,0}, {1,1,1}, {1,0,1}, {1,1,1}}, // 6
  {{1,1,1}, {0,0,1}, {0,1,0}, {1,0,0}, {1,0,0}}, // 7
  {{1,1,1}, {1,0,1}, {1,1,1}, {1,0,1}, {1,1,1}}, // 8
  {{1,1,1}, {1,0,1}, {1,1,1}, {0,0,1}, {1,1,1}}  // 9
};

// Star structure for starry night
struct Star {
  uint8_t row;
  uint8_t col;
  float phase;
};

// Global arrays
Star stars[20];
uint8_t frameBuffer[ROWS][COLS][3];

// Demo state
int currentDemo = 0;
unsigned long demoStartTime = 0;

Illumicro matrix(0x30); // Adjust I2C address if needed

void setup() {
  pinMode(PIN_PA6, OUTPUT);
  digitalWrite(PIN_PA6, HIGH);
  delay(500);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  matrix.begin();
  matrix.setGlobalMaxCurrentPercent(5, 2550);

  randomSeed(analogRead(0));

  // Initialize stars
  for (int i = 0; i < 20; i++) {
    stars[i].row = random(0, 9);
    stars[i].col = random(0, 13);
    stars[i].phase = random(0, 628) / 100.0;
  }

  // Clear matrix
  for (uint8_t row = 0; row < ROWS; row++) {
    for (uint8_t col = 0; col < COLS; col++) {
      matrix.setLED(row, col, 0, 0);
      matrix.setLED(row, col, 1, 0);
      matrix.setLED(row, col, 2, 0);
    }
  }
}

// Time-based demo functions
void wavyDemo() {
  const uint8_t baseline = 10; // Minimum LED intensity (10/255)
  for (uint8_t step = 0; step < 200; ++step) {
    float t = step * 0.05f; // Time in seconds (50ms per step)
    float color_angle = (2.0f * PI * step) / 200.0f; // Smooth color cycle over 10s
    uint8_t base_g = (uint8_t)(127.5f * (1.0f + sinf(color_angle))); // Green
    uint8_t base_b = (uint8_t)(127.5f * (1.0f + cosf(color_angle))); // Blue
    for (uint8_t row = 0; row < ROWS; ++row) {
      for (uint8_t col = 0; col < COLS; ++col) {
        float wave = 0.5f + 0.5f * sinf(2.0f * PI * (col / (float)COLS + t * 0.5f)); // Horizontal wave, moving up
        uint8_t intensity = (uint8_t)(wave * 245.0f) + baseline; // Scale to 245 + baseline
        if (intensity > 255) intensity = 255; // Clamp
        frameBuffer[row][col][0] = (base_b * intensity) >> 8; // Blue
        frameBuffer[row][col][1] = (base_g * intensity) >> 8; // Green
        frameBuffer[row][col][2] = (intensity * baseline) >> 8; // Dim red for contrast
      }
    }
    matrix.pushFrame(frameBuffer);
    delay(5);
  }
}

void diagonalWaveDemo() {
  for (uint8_t step = 0; step < 200; step++) {
    uint8_t offset = step % 26;
    for (uint8_t row = 0; row < ROWS; row++) {
      for (uint8_t col = 0; col < COLS; col++) {
        uint8_t diag = (row + col + offset) % 26;
        uint8_t r, g, b;
        if (diag < 8) { r = 255; g = 0; b = 0; }
        else if (diag < 16) { r = 0; g = 255; b = 0; }
        else { r = 0; g = 0; b = 255; }
        frameBuffer[row][col][0] = b;
        frameBuffer[row][col][1] = g;
        frameBuffer[row][col][2] = r;
      }
    }
    matrix.pushFrame(frameBuffer);
    delay(50);
  }
}

void pulseDemo() {
  const float center_row = (ROWS - 1) / 2.0f;
  const float center_col = (COLS - 1) / 2.0f;
  const uint8_t baseline = 10; // Minimum LED intensity (10/255)
  for (uint8_t step = 0; step < 200; ++step) {
    float t = step * 0.05f; // Time in seconds (50ms per step)
    float color_angle = (2.0f * PI * step) / 200.0f; // Smooth color cycle over 10s
    uint8_t base_r = (uint8_t)(127.5f * (1.0f + sinf(color_angle)));
    uint8_t base_g = (uint8_t)(127.5f * (1.0f + sinf(color_angle + 2.0f * PI / 3.0f)));
    uint8_t base_b = (uint8_t)(127.5f * (1.0f + sinf(color_angle + 4.0f * PI / 3.0f)));
    for (uint8_t row = 0; row < ROWS; ++row) {
      for (uint8_t col = 0; col < COLS; ++col) {
        float dx = col - center_col;
        float dy = row - center_row;
        float dist = sqrtf(dx * dx + dy * dy);
        float wave = 0.0f;
        for (int i = 0; i <= (int)(t / 1.0f); ++i) {
          float drop_time = t - i * 1.0f; // Time since drop i
          if (drop_time <= 3.0f) { // 3s for full matrix coverage
            for (int j = 0; j < 2; ++j) { // Two waves per drop
              float wave_time = drop_time - 0.2f * j;
              float wave_dist = dist - 3.5f * wave_time; // Wave speed: 3.5 pixels/sec
              if (wave_dist >= -0.8f && wave_dist <= 0.8f) {
                wave += 0.3f * (1.0f + cosf(PI * wave_dist)) * expf(-wave_time * 0.8f);
              }
            }
          }
        }
        uint8_t intensity = (uint8_t)(wave * 245.0f) + baseline; // Scale to 245 + baseline
        if (intensity > 255) intensity = 255; // Clamp
        frameBuffer[row][col][0] = (base_b * intensity) >> 8; // Blue
        frameBuffer[row][col][1] = (base_g * intensity) >> 8; // Green
        frameBuffer[row][col][2] = (base_r * intensity) >> 8; // Red
      }
    }
    matrix.pushFrame(frameBuffer);
    delay(5);
  }
}

void starryNight() {
  for (int s = 0; s < 500; s++) {
    memset(frameBuffer, 0, sizeof(frameBuffer));
    for (int i = 0; i < 20; i++) {
      float brightness = 255 * (0.5 + 0.5 * sin(millis() / 1000.0 + stars[i].phase));
      uint8_t b = (uint8_t)brightness;
      frameBuffer[stars[i].row][stars[i].col][0] = b;
      frameBuffer[stars[i].row][stars[i].col][1] = b;
      frameBuffer[stars[i].row][stars[i].col][2] = b;
    }
    matrix.pushFrame(frameBuffer);
    delay(20);
  }
}

void fallingSand() {
  struct Particle {
    uint8_t row;
    uint8_t col;
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  Particle active[117];
  uint16_t active_count = 0;
  bool settled[9][13] = {0};
  uint16_t settled_count = 0;

  for (uint8_t row = 0; row < 9; row++) {
    for (uint8_t col = 0; col < 13; col++) {
      matrix.setLED(row, col, 0, 0);
      matrix.setLED(row, col, 1, 0);
      matrix.setLED(row, col, 2, 0);
    }
  }

  while (settled_count < 117) {
    if (active_count < 117) {
      uint8_t free_cols[13];
      uint8_t free_count = 0;
      for (uint8_t col = 0; col < 13; col++) {
        bool occupied = settled[0][col];
        for (uint8_t i = 0; i < active_count; i++) {
          if (active[i].row == 0 && active[i].col == col) {
            occupied = true;
            break;
          }
        }
        if (!occupied) {
          free_cols[free_count++] = col;
        }
      }
      if (free_count > 0) {
        uint8_t col = free_cols[random(0, free_count)];
        active[active_count].row = 0;
        active[active_count].col = col;
        active[active_count].r = random(0, 256);
        active[active_count].g = random(0, 63);
        active[active_count].b = random(0, 63);
        matrix.setLED(0, col, 0, active[active_count].b);
        matrix.setLED(0, col, 1, active[active_count].g);
        matrix.setLED(0, col, 2, active[active_count].r);
        active_count++;
      }
    }

    for (int i = 0; i < active_count; i++) {
      uint8_t row = active[i].row;
      uint8_t col = active[i].col;
      uint8_t r = active[i].r;
      uint8_t g = active[i].g;
      uint8_t b = active[i].b;

      if (row + 1 >= 9 || settled[row + 1][col]) {
        settled[row][col] = true;
        settled_count++;
        active[i] = active[active_count - 1];
        active_count--;
        i--;
      } else {
        bool can_move = true;
        for (int j = 0; j < active_count; j++) {
          if (j != i && active[j].row == row + 1 && active[j].col == col) {
            can_move = false;
            break;
          }
        }
        if (can_move) {
          matrix.setLED(row, col, 0, 0);
          matrix.setLED(row, col, 1, 0);
          matrix.setLED(row, col, 2, 0);
          active[i].row++;
          matrix.setLED(active[i].row, col, 0, b);
          matrix.setLED(active[i].row, col, 1, g);
          matrix.setLED(active[i].row, col, 2, r);
        }
      }
    }
    delay(50);
  }
  delay(1000); // Pause after completion
}

void loop() {
  // Time-based demos
  if (currentDemo == 0) {
    demoStartTime = millis();
    wavyDemo();
    currentDemo++;
  }
  else if (currentDemo == 1) {
    demoStartTime = millis();
    diagonalWaveDemo();
    currentDemo++;
  }
  else if (currentDemo == 2) {
    demoStartTime = millis();
    pulseDemo();
    currentDemo++;
  }
  else if (currentDemo == 3) {
    demoStartTime = millis();
    starryNight();
    currentDemo++;
  }
  else if (currentDemo == 4) {
    demoStartTime = millis();
    fallingSand();
    currentDemo++;
  }
  // Trailing demo
  else if (currentDemo == 5) {
    static float PARTICLE_SPEED = 20.0;
    static const float FADE_RATE = 1200.0;
    static bool ramp = true;
    static int particle_row = 0;
    static int particle_col = 0;
    static int state = 0;
    static unsigned long accumulated_time = 0;
    static unsigned long last_time = millis();
    static uint8_t brightness[ROWS][COLS] = {0};
    static unsigned long frame_count = 0;
    static unsigned long fps_start_time = last_time;
    static int fps = 0;

    unsigned long time_per_pixel = 1000 / PARTICLE_SPEED;
    unsigned long current_time = millis();
    unsigned long elapsed_time = current_time - last_time;
    last_time = current_time;
    accumulated_time += elapsed_time;
    frame_count++;

    if (current_time - fps_start_time >= 1000) {
      if ((PARTICLE_SPEED < 110) && (ramp == true)) {
        PARTICLE_SPEED += 10;
      } else {
        ramp = false;
      }
      if ((PARTICLE_SPEED > 20) && (ramp == false)) {
        PARTICLE_SPEED -= 10;
      } else {
        ramp = true;
      }
      fps = frame_count;
      frame_count = 0;
      fps_start_time = current_time;
    }
    

    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        if (brightness[row][col] > 0) {
          int fade_amount = (FADE_RATE * elapsed_time) / 1000;
          brightness[row][col] = (brightness[row][col] > fade_amount) ? brightness[row][col] - fade_amount : 0;
        }
      }
    }

    while (accumulated_time >= time_per_pixel) {
      brightness[particle_row][particle_col] = 255;
      if (state == 0) {
        particle_col++;
        if (particle_col >= COLS - 1) { particle_col = COLS - 1; state = 1; }
      }
      else if (state == 1) {
        particle_row++;
        if (particle_row >= ROWS - 1) { particle_row = ROWS - 1; state = 2; }
      }
      else if (state == 2) {
        particle_col--;
        if (particle_col <= 0) { particle_col = 0; state = 3; }
      }
      else if (state == 3) {
        particle_row--;
        if (particle_row <= 0) { particle_row = 0; state = 0; }
      }
      accumulated_time -= time_per_pixel;
    }

    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        if (row >= 2 && row <= 6 && col >= 2 && col <= 10) {
          int digit_index = (col - 2) / 3;
          int local_col = (col - 2) % 3;
          int local_row = row - 2;
          int digit_value;
          if (digit_index == 0) digit_value = fps / 100;
          else if (digit_index == 1) digit_value = (fps / 10) % 10;
          else digit_value = fps % 10;
          if (digit_patterns[digit_value][local_row][local_col] == 1) {
            frameBuffer[row][col][0] = 0;
            frameBuffer[row][col][1] = 45;
            frameBuffer[row][col][2] = 80;
          } else {
            frameBuffer[row][col][0] = 0;
            frameBuffer[row][col][1] = 0;
            frameBuffer[row][col][2] = 0;
          }
        } else {
          uint8_t b = brightness[row][col];
          if (row == particle_row && col == particle_col) {
            frameBuffer[row][col][0] = 55;
            frameBuffer[row][col][1] = 127;
            frameBuffer[row][col][2] = 255;
          } else {
            frameBuffer[row][col][0] = b / 10;
            frameBuffer[row][col][1] = 0;
            frameBuffer[row][col][2] = b / 10;
          }
        }
      }
    }
    matrix.pushFrame(frameBuffer);

    // Check for button press to restart
    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(50); // Debounce
      if (digitalRead(BUTTON_PIN) == LOW) {
        currentDemo = 0;
        // Reset trailing demo state
        PARTICLE_SPEED = 20.0;
        ramp = true;
        particle_row = 0;
        particle_col = 0;
        state = 0;
        accumulated_time = 0;
        last_time = millis();
        memset(brightness, 0, sizeof(brightness));
        frame_count = 0;
        fps_start_time = last_time;
        fps = 0;
      }
    }
  }
}