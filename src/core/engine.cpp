#include <iostream>
#include <string>
#include <SDL.h>
#include <cmath>

#include "core/engine.h"
dimension ScreenDim;

Engine::Engine(const char *name) {
    _window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
    onResizeCanvas();
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    _background = drawGrid(ScreenDim, &SOFT_BLACK, &DARK_GREY, 40);
    car = std::unique_ptr<Vehicle>(GenerateCar({20, 20}));
    scar = std::unique_ptr<SimCar>(GenerateSimCar());
}

Engine::~Engine() {
    SDL_DestroyTexture(_background);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

SDL_Texture *Engine::drawGrid(dimension d, const SDL_Color *bc, const SDL_Color *gc, int size) {
    SDL_Texture *grid = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, d.w, d.h);
    SDL_SetRenderTarget(_renderer, grid);

    // background
    SDL_SetRenderDrawColor(_renderer, bc->r, bc->g, bc->b, bc->a);
    SDL_RenderClear(_renderer);

    // grid
    SDL_SetRenderDrawColor(_renderer, gc->r, gc->g, gc->b, gc->a);
    for (int i = 0; i < d.w; i += size)
        SDL_RenderDrawLine(_renderer, i, 0, i, d.h); // verticle
    for (int i = 0; i < d.h; i += size)
        SDL_RenderDrawLine(_renderer, 0, i, d.w, i); // horizontal

    SDL_SetRenderTarget(_renderer, NULL);
    return grid;
}

SDL_Texture *Engine::createTexture(dimension d, const SDL_Color *color, const SDL_Color *borderColor) {
    SDL_Texture *texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, d.w, d.h);
    SDL_SetRenderTarget(_renderer, texture);

    if (borderColor) {
        SDL_SetRenderDrawColor(_renderer, borderColor->r, borderColor->g, borderColor->b, borderColor->a);
        SDL_RenderClear(_renderer);

        SDL_SetRenderDrawColor(_renderer, color->r, color->g, color->b, color->a);
        SDL_Rect border = {1, 1, d.w - 2, d.h - 2};
        SDL_RenderFillRect(_renderer, &border);
    } else {
        SDL_SetRenderDrawColor(_renderer, color->r, color->g, color->b, color->a);
        SDL_RenderClear(_renderer);
    }

    SDL_SetRenderTarget(_renderer, NULL);
    return texture;
}

void Engine::GenerateObstacle() {
    srand(time(NULL));
    float x_cell = (rand() % ScreenDim.w + 1);
    float y_cell = (rand() % ScreenDim.h + 1);

    int width = (rand() % 20 + 10) * 10;
    width = std::fmin(width, ScreenDim.w - x_cell);
    int height = (rand() % 20 + 10) * 10;
    height = std::fmin(height, ScreenDim.w - y_cell);

    SDL_Texture *obstacleTexture = createTexture({width, height}, &DARK_GREY, &SOFT_WHITE);
    std::unique_ptr<Obstacle> ob = std::unique_ptr<Obstacle>(new Obstacle({x_cell, y_cell}, {width, height}, obstacleTexture));
    obstacles.push_back(std::move(ob));
}

Vehicle *Engine::GenerateCar(dimension d) {
    SDL_Texture *carTexture = createTexture(d, &SOFT_WHITE);
    Vehicle *car = new Vehicle({static_cast<float>(ScreenDim.w / 2), static_cast<float>(ScreenDim.h / 2)}, d, carTexture);
    return car;
}

SimCar *Engine::GenerateSimCar() {
    SDL_Texture *carTexture = createTexture(car->getDim(), &LIGHT_BLUE, &NEON_BLUE);
    SimCar *scar = new SimCar({static_cast<float>(ScreenDim.w / 2), static_cast<float>(ScreenDim.h / 2)}, car->getDim(), carTexture);
    return scar;
}

void Engine::run() {
    renderCar();
}

void Engine::onResizeCanvas() {
    SDL_GetWindowSize(_window, &ScreenDim.w, &ScreenDim.h);
    position upleft = {0, 0};
    position upright = {(float)ScreenDim.w, 0};
    position downleft = {0, (float)ScreenDim.h};
    position downright = {(float)ScreenDim.w, (float)ScreenDim.h};
    line up = {upleft, upright};
    line down = {downleft, downright};
    line right = {upright, downright};
    line left = {upleft, downleft};
    walls = {up, down, left, right};
}

void Engine::render(Object *obj, SDL_Texture *texture) {
    SDL_Rect src = {0, 0, obj->getDim().w, obj->getDim().h};
    SDL_FRect dst = {obj->getPos().x, obj->getPos().y, (float)obj->getDim().w, (float)obj->getDim().h};
    if (texture)
        SDL_SetRenderTarget(_renderer, texture);

    if (obj->getAngle() != 0) {
        float x_center = (obj->getDim().w / 2);
        float y_center = (obj->getDim().h / 2);
        SDL_FPoint center = {x_center, y_center};
        SDL_RenderCopyExF(_renderer, obj->getTexture(), &src, &dst, obj->getAngle(), &center, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopyF(_renderer, obj->getTexture(), &src, &dst);
    }

    if (texture)
        SDL_SetRenderTarget(_renderer, NULL);
}

void Engine::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case 13:
            GenerateObstacle();
            break;
        case SDLK_l:
            sim_view = !sim_view;
        }
    }
    if (!sim_view)
        for (auto &ob : obstacles) {
            ob->handleEvent(e);
            ob->drag();
        }
    car->handleEvent(e);
}

void Engine::renderCar() {
    // refresh the canvas
    onResizeCanvas();
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _background, NULL, NULL);

    // draw obstacles
    if (!sim_view)
        for (auto &ob : obstacles)
            render(ob.get());

    // handle car movement
    position prev_pos = car->getPos();
    angle prev_ang = car->getAngle();

    // check collisions
    float x_offset = car->getOffset() * getX(car->getAngle());
    float y_offset = car->getOffset() * getY(car->getAngle());

    SDL_FRect car_collider = {car->getPos().x + x_offset, car->getPos().y+ y_offset, (float)car->getDim().w, (float)car->getDim().h};

    for (auto &ob : obstacles) {
        SDL_FRect ob_collider = {ob->getPos().x, ob->getPos().y, (float)ob->getDim().w, (float)ob->getDim().h};
        SDL_FRect temp;
        if (SDL_IntersectFRect(&car_collider, &ob_collider, &temp)) {
            car->getPos().x -= x_offset;
            car->getPos().y -= y_offset;
            car->getVel() = -car->getVel()/10;
            
        }
    }

    // update last time
    Uint32 pos_t = SDL_GetTicks();
    car->move(pos_t);

    // feed to simcar
    position post_pos = car->getPos();
    angle post_ang = car->getAngle();
    float dt = (pos_t - car->getLastTs()) / 1000.0f;

    velocity vel = isInDir(prev_pos, post_pos, prev_ang) * distance(post_pos, prev_pos) / dt;
    angle yaw = (post_ang - prev_ang) / dt;
    state s = {vel, yaw, pos_t};

    // update simcar and car
    scar->updateState(s);
    car->setLastUpdate(pos_t);

    // draw raydar rays
    SDL_SetRenderDrawColor(_renderer, 0, SOFT_WHITE.g, 0, 10);
    std::vector<Ray *> rayDars = car->getRayDar()->rays;
    for (auto &ray : rayDars) {
        position intersect = {-1, -1};
        position temp;
        float smallest = 10000.0f;

        for (auto &ob : obstacles) {
            std::vector<line> lines = ob->getWalls();
            for (line l : lines) {
                if (!ray->isIntersecting(l, temp))
                    continue;
                if (distance(temp, ray->_origin) < smallest) {
                    smallest = distance(temp, ray->_origin);
                    intersect = temp;
                }
            }
        }

        for (line l : walls) {
            if (!ray->isIntersecting(l, temp))
                continue;
            if (distance(temp, ray->_origin) < smallest) {
                smallest = distance(temp, ray->_origin);
                intersect = temp;
            }
        }

        if (!sim_view) {
            if (intersect.x == -1)
                intersect = ray->getDir();
            position rayep;
            for (float radius = 15; radius < distance(intersect, ray->_origin); radius *= 2) {
                rayep = rayEndpoint(ray->_origin, ray->_rot, radius);
                SDL_RenderDrawPointF(_renderer, rayep.x, rayep.y);
            }
        } else {
            if (intersect.x != -1) {
                position rayep;
                for (float radius = 15; radius < distance(intersect, ray->_origin); radius *= 2) {
                    rayep = rayEndpoint(ray->_origin, ray->_rot, radius);
                    SDL_RenderDrawPointF(_renderer, rayep.x, rayep.y);
                }
            }
        }
        if (intersect.x != -1) {
            SDL_FRect rect = {static_cast<float>(intersect.x - 1.5), static_cast<float>(intersect.y - 1.5), 3, 3};
            SDL_RenderFillRectF(_renderer, &rect);
        }
    }

    // draw camera rays
    SDL_SetRenderDrawColor(_renderer, SOFT_WHITE.r, SOFT_WHITE.g, SOFT_WHITE.b, SOFT_WHITE.a);
    std::vector<Ray *> camRays = car->getCamera()->rays;
    for (int i = 0; i < camRays.size(); i++) {
        position intersect = {-1, -1};
        position temp;
        float smallest = 10000.0f;

        for (auto &ob : obstacles) {
            std::vector<line> lines = ob->getWalls();
            for (line l : lines) {
                if (!camRays[i]->isIntersecting(l, temp))
                    continue;
                if (distance(temp, camRays[i]->_origin) < smallest) {
                    smallest = distance(temp, camRays[i]->_origin);
                    intersect = temp;
                }
            }
        }

        for (line l : walls) {
            if (!camRays[i]->isIntersecting(l, temp))
                continue;
            if (distance(temp, camRays[i]->_origin) < smallest)
            {
                smallest = distance(temp, camRays[i]->_origin);
                intersect = temp;
            }
        }
        SDL_FRect rect = {static_cast<float>(intersect.x - 2.5), static_cast<float>(intersect.y - 2.5), 5, 5};
        SDL_RenderFillRectF(_renderer, &rect);

        if ((i == 0 || i == camRays.size() - 1) && !sim_view)
            SDL_RenderDrawLineF(_renderer, camRays[i]->_origin.x, camRays[i]->_origin.y, intersect.x, intersect.y);
    }

    // draw car
    render(car.get());
    // if (sim_view)
        render(scar.get());

    // display
    SDL_RenderPresent(_renderer);
}
