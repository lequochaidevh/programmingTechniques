
#include "main.h"

static struct termios old_termios, new_termios;

void init_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
    fflush(stdout);
}

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    printf("terminal restored\n\n");
}

static char keystate[256] = { 0 };

void process_input() {
    char c;
    for (int i = 0; i < 256; i++) {
        keystate[i] = 0;
    }

    while (read(STDIN_FILENO, &c, 1) > 0) {
        // printf("\ninput: %c", c);
        unsigned char uc = (unsigned char)c;
        keystate[uc] = 1;
    }
}

int is_key_pressed(char key) {
    return keystate[(unsigned char)key];
}

char** init_picture() {
    char** picture = malloc(sizeof(char*) * Y_PIXELS);
    for (int i = 0; i < Y_PIXELS; i++) {
        picture[i] = malloc(sizeof(char) * X_PIXELS);
    }
    return picture;
}

char*** init_blocks() {
    char*** blocks = malloc(sizeof(char**) * Z_BLOCKS);
    for (int i = 0; i < Z_BLOCKS; i++) {
        blocks[i] = malloc(sizeof(char*) * Y_BLOCKS);
        for (int j = 0; j < Y_BLOCKS; j++) {
            blocks[i][j] = malloc(sizeof(char) * X_BLOCKS);
            for (int k = 0; k < X_BLOCKS; k++) {
                blocks[i][j][k] = ' ';
            }
        }
    }
    return blocks;
}

typedef struct Vector {
    float x;
    float y;
    float z;
} vect;
typedef struct Vector2 {
    float psi;
    float phi;
} vect2;


typedef struct Vector_vector2 {
    vect pos;
    vect2 view;
} player_pos_view;

player_pos_view init_posview() {
    player_pos_view posview;
    posview.pos.x = 5;
    posview.pos.y = 5;
    posview.pos.z = 4 + EYE_HEIGHT;
    posview.view.phi = 0;
    posview.view.psi = 0;
    return posview;
}

// Convert spherical angles (phi, psi) to a 3D direction vector
vect angles_to_vect(vect2 angles) {
    vect res;
    res.x = cos(angles.psi) * cos(angles.phi);
    res.y = cos(angles.psi) * sin(angles.phi);
    res.z = sin(angles.psi);
    return res;
}

// Add two vectors component-wise
vect vect_add(vect v1, vect v2) {
    vect res;
    res.x = v1.x + v2.x;
    res.y = v1.y + v2.y;
    res.z = v1.z + v2.z;
    return res;
}

// Scale a vector by a scalar s
vect vect_scale(float s, vect v) {
    vect res = { s * v.x, s * v.y, s * v.z };
    return res;
}

// Subtract vector v2 from vector v1
vect vect_sub(vect v1, vect v2) {
    vect v3 = vect_scale(-1, v2);  // Negate v2
    return vect_add(v1, v3);       // Add v1 + (-v2)
}

// Normalize the vector v to have length 1 (modifies v in place)
void vect_normalize(vect* v) {
    float len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}

// Initialize a 2D array of direction vectors for each pixel given a center view angle
vect** init_directions(vect2 view) {
    // Compute direction vectors at the edges of the vertical field of view
    view.psi -= VIEW_HEIGHT / 2.0;    // Move down half viewport height
    vect screen_down = angles_to_vect(view);
    view.psi += VIEW_HEIGHT;          // Move up full viewport height
    vect screen_up = angles_to_vect(view);
    view.psi -= VIEW_HEIGHT / 2.0;    // Reset vertical angle to center

    // Compute direction vectors at the edges of the horizontal field of view
    view.phi -= VIEW_WIDTH / 2.0;     // Move left half viewport width
    vect screen_left = angles_to_vect(view);
    view.phi += VIEW_WIDTH;           // Move right full viewport width
    vect screen_right = angles_to_vect(view);
    view.phi -= VIEW_WIDTH / 2.0;     // Reset horizontal angle to center

    // Calculate midpoints for vertical and horizontal edges
    vect screen_mid_vert = vect_scale(0.5, vect_add(screen_up, screen_down));
    vect screen_mid_hor = vect_scale(0.5, vect_add(screen_left, screen_right));

    // Calculate vectors from midpoints to edges
    vect mid_to_left = vect_sub(screen_left, screen_mid_hor);
    vect mid_to_up = vect_sub(screen_up, screen_mid_vert);

    // Allocate 2D array of vectors for directions: Y_PIXELS rows, X_PIXELS columns
    vect** dir = malloc(sizeof(vect*) * Y_PIXELS);
    for (int i = 0; i < Y_PIXELS; i++) {
        dir[i] = malloc(sizeof(vect) * X_PIXELS);
    }

    // For each pixel, calculate the corresponding direction vector
    for (int y_pix = 0; y_pix < Y_PIXELS; y_pix++) {
        for (int x_pix = 0; x_pix < X_PIXELS; x_pix++) {
            // Start from midpoint plus offset vectors
            vect tmp = vect_add(vect_add(screen_mid_hor, mid_to_left), mid_to_up);

            // Subtract scaled horizontal offset depending on pixel position
            tmp = vect_sub(tmp, vect_scale(((float)x_pix / (X_PIXELS - 1)) * 2, mid_to_left));

            // Subtract scaled vertical offset depending on pixel position
            tmp = vect_sub(tmp, vect_scale(((float)y_pix / (Y_PIXELS - 1)) * 2, mid_to_up));

            // Normalize the resulting vector to unit length
            vect_normalize(&tmp);

            // Assign the direction vector to the array for this pixel
            dir[y_pix][x_pix] = tmp;
        }
    }
    return dir;
}

// Check if a vector position lies outside the bounding box defined by X_BLOCKS, Y_BLOCKS, Z_BLOCKS
int ray_outside(vect pos) {
    if (pos.x >= X_BLOCKS || pos.y >= Y_BLOCKS || pos.z >= Z_BLOCKS
        || pos.x < 0 || pos.y < 0 || pos.z < 0) {
        return 1;  // Outside bounds
    }
    return 0;      // Inside bounds
}

// Check if the position is near the border of a block on at least two axes
int on_block_border(vect pos) {
    int cnt = 0;
    // Check distance to nearest integer coordinate on x axis
    if (fabsf(pos.x - roundf(pos.x)) < BLOCK_BORDER_SIZE) {
        cnt++;
    }
    // Check distance to nearest integer coordinate on y axis
    if (fabsf(pos.y - roundf(pos.y)) < BLOCK_BORDER_SIZE) {
        cnt++;
    }
    // Check distance to nearest integer coordinate on z axis
    if (fabsf(pos.z - roundf(pos.z)) < BLOCK_BORDER_SIZE) {
        cnt++;
    }
    // Return true if close to at least two block borders (edges or corners)
    if (cnt >= 2) {
        return 1;
    }
    return 0;
}

// Return the smaller of two floats
float min(float a, float b) {
    if (a < b)
        return a;
    return b;
}

// Perform ray tracing from pos in direction dir through the 3D blocks array
char raytrace(vect pos, vect dir, char*** blocks) {
    float eps = 0.01;  // Small epsilon to avoid precision issues
    while (!ray_outside(pos)) {
        // Get block character at current integer position
        char c = blocks[(int)pos.z][(int)pos.y][(int)pos.x];
        if (c != ' ') {  // If block is not empty
            if (on_block_border(pos)) {
                return '-';  // If position near block border, return special char
            }
            else {
                return c;    // Otherwise, return block character (hit)
            }
        }

        float dist = 2;  // Max distance to next voxel boundary along ray
        // Calculate distance to next block boundary on x axis based on ray direction
        if (dir.x > eps) {
            dist = min(dist, ((int)(pos.x + 1) - pos.x) / dir.x);
        }
        else if (dir.x < -eps) {
            dist = min(dist, ((int)pos.x - pos.x) / dir.x);
        }
        // Calculate distance to next block boundary on y axis
        if (dir.y > eps) {
            dist = min(dist, ((int)(pos.y + 1) - pos.y) / dir.y);
        }
        else if (dir.y < -eps) {
            dist = min(dist, ((int)pos.y - pos.y) / dir.y);
        }
        // Calculate distance to next block boundary on z axis
        if (dir.z > eps) {
            dist = min(dist, ((int)(pos.z + 1) - pos.z) / dir.z);
        }
        else if (dir.z < -eps) {
            dist = min(dist, ((int)pos.z - pos.z) / dir.z);
        }
        // Move position along ray direction by distance plus epsilon
        pos = vect_add(pos, vect_scale(dist + eps, dir));
    }
    return ' ';  // Return empty space if ray goes outside bounds without hitting anything
}

// Generate the ASCII picture by ray tracing all pixels from player position/view
char** get_picture(char** picture, player_pos_view posview, char*** blocks) {
    vect** directions = init_directions(posview.view);  // Initialize ray directions for all pixels
    for (int y = 0; y < Y_PIXELS; y++) {
        for (int x = 0; x < X_PIXELS; x++) {
            // Ray trace from player position along pixel direction and get block char
            picture[y][x] = raytrace(posview.pos, directions[y][x], blocks);
        }
    }
}

// Render the ASCII picture to the console, with coloring for character 'o'
void draw_ascii(char** picture) {
    fflush(stdout);
    printf("\033[0;0H"); // ANSI escape code to move cursor to top-left of screen
    for (int i = 0; i < Y_PIXELS; i++) {
        int current_color = 0;
        for (int j = 0; j < X_PIXELS; j++) {
            // Change color to green if character is 'o' and color not already green
            if (picture[i][j] == 'o' && current_color != 32) {
                printf("\x1B[32m"); // Set terminal text color to green
                current_color = 32;
            }
            // Reset color if current char not 'o' and color is green
            else if (picture[i][j] != 'o' && current_color != 0) {
                printf("\x1B[0m");  // Reset terminal text color
                current_color = 0;
            }
            printf("%c", picture[i][j]);
        }
        printf("\x1B[0m\n"); // Reset color and print newline at end of each row
    }
}


int main() {
    printf("\n\n");
    printf("\x1B[32m"); // Set terminal text color to green
    printf("-------------------------------------------------------------------\n");
    printf("Minecraft                    ---                    version 1.0.0\n");
    printf("License                      ---                    MIT\n");
    printf("Author                       ---                    devh\n");
    printf("Code language                ---                    c17\n");
    printf("-------------------------------------------------------------------\n");
    printf("\x1B[0m");  // Reset terminal text color
    sleep(5);
    init_terminal();
    // Init obj by char pixel
    char** picture = init_picture();
    char*** blocks = init_blocks();
    for (int x = 0; x < X_BLOCKS; x++) {
        for (int y = 0; y < Y_BLOCKS; y++) {
            for (int z = 0; z < 4; z++) {
                blocks[z][y][x] = '@';
            }
        }
    }

    player_pos_view posview = init_posview();

    while (1) {
        process_input();
        if (is_key_pressed('q')) {
            break;
        }
        
        usleep(20000); //20ms

        get_picture(picture, posview, blocks);
        
        draw_ascii(picture);
    }
    restore_terminal();
 
    return 0;
}