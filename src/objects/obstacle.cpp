#include <objects/obstacle.h>

Obstacle::Obstacle(position p, dimension d, SDL_Texture * texture) 
    : _pos(p), _dim(d), _texture(texture) { }

Obstacle::~Obstacle() { 
    SDL_DestroyTexture(_texture);
}

void Obstacle::handleEvent (SDL_Event & e) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_Point cursor = {mouse_x, mouse_y};
    SDL_Rect rect = {_pos.x, _pos.y, _dim.w, _dim.h};

    if (e.motion.type == SDL_MOUSEBUTTONDOWN && SDL_PointInRect(&cursor, &rect) == SDL_TRUE && SDL_PointInRect(&cursor, &rect) && !isDragged) {
        isDragged = true; 
        _prev_mouse = {(float)mouse_x, (float)mouse_y};
    }
    if (e.motion.state == SDL_RELEASED) isDragged = false;
}

void Obstacle::drag() {
    if (isDragged) {
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        
        int offset_x = mouse_x - _prev_mouse.x;
        int offset_y = mouse_y - _prev_mouse.y;

        _pos.x += offset_x;
        _pos.y += offset_y;
        _prev_mouse = {(float)mouse_x, (float)mouse_y};
    }
}