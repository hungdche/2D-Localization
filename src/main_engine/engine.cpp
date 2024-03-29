#include <iostream>
#include <string>
#include <cmath>

#include "engine.h"

#pragma region init
dimension ScreenDim;
Engine::Engine(const char *name) {
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_EVENTS)) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
    if (TTF_Init() < 0) {
        std::cout << TTF_GetError() << std::endl;
        exit(0);
    }
    _window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
    onResizeCanvas();
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    _background = drawGrid(ScreenDim, &SOFT_BLACK, &DARK_GREY, 40);
    _font = TTF_OpenFont("../font/UbuntuMono-R.ttf", 17);
    
    car = std::unique_ptr<Vehicle>(GenerateCar({20, 20}));
    scar = std::unique_ptr<SimCar>(GenerateSimCar());
    _stat_surface = updateStats();
    _speed_surface = showSpeed();
    lastUpdate = SDL_GetTicks();
}

Engine::~Engine() {
    TTF_CloseFont(_font);
    SDL_DestroyTexture(_background);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    TTF_Quit();
    SDL_Quit();
}

void Engine::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case 13:
            GenerateObstacle();
            break;
        case SDLK_n:
            scar = std::unique_ptr<SimCar>(GenerateSimCar());
            scar->getPos().x = car->getPos().x;
            scar->getPos().y = car->getPos().y;
            break;
        case SDLK_l:
            sim_view = !sim_view;
            break;
        }
    }
    if (!sim_view)
        for (auto &ob : obstacles) {
            ob->handleEvent(e);
            ob->drag();
        }
    car->handleEvent(e);
}
#pragma endregion 

#pragma region init_obj
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
    state s = {car->getPos(), car->getAngle()};
    SDL_Texture *carTexture = createTexture(car->getDim(), &LIGHT_BLUE, &NEON_BLUE);
    SimCar *scar = new SimCar(s, car->getVel(), car->getW(), car->getDim(), carTexture);
    return scar;
}
#pragma endregion 

#pragma region init_graphics
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

void Engine::onResizeCanvas() {
    SDL_GetWindowSize(_window, &ScreenDim.w, &ScreenDim.h);
    position upleft = {0, 0};
    position upright = {(float)ScreenDim.w-1, 0};
    position downleft = {0, (float)ScreenDim.h-1};
    position downright = {(float)ScreenDim.w-1, (float)ScreenDim.h-1};
    line up = {upleft, upright};
    line down = {downleft, downright};
    line right = {upright, downright};
    line left = {upleft, downleft};
    walls = {up, down, left, right};
}
#pragma endregion 

#pragma region screen_update_stats
SDL_Surface * Engine::updateStats() {
    // current stat
    std::string current = "True  " + std::to_string(car->getPos().x);
    current += " " + std::to_string(car->getPos().y);
    current += " " + std::to_string(car->getAngle());

    // estimated stat
    current += "\nEsti. " + std::to_string(scar->getPos().x);
    current += " " + std::to_string(scar->getPos().y);
    current += " " + std::to_string(scar->getAngle());

    current += "\n";
    // estimated stat
    state predicted = scar->getPredState();
    current += "\nPred. " + std::to_string(predicted._pos.x);
    current += " " + std::to_string(predicted._pos.y);
    current += " " + std::to_string(predicted._rot);

    // estimated stat
    state measured = scar->getMeasState();
    current += "\nMeas. " + std::to_string(measured._pos.x);
    current += " " + std::to_string(measured._pos.y);
    current += " " + std::to_string(measured._rot);


    SDL_Surface* text_surf = TTF_RenderText_Blended_Wrapped(_font, current.c_str(), WHITE, 400);
    if (!_font) std::cout << TTF_GetError();
    if (!text_surf) std::cout << "NULL" << std::endl;
    return text_surf;
}

SDL_Surface * Engine::showSpeed() {
    // current stat
    std::string current = "Speed " + std::to_string(car->getVel());
    current += " Accel: " + std::to_string(car->getAccel());

    SDL_Surface* text_surf = TTF_RenderText_Blended_Wrapped(_font, current.c_str(), WHITE, 400);
    if (!_font) std::cout << TTF_GetError();
    if (!text_surf) std::cout << "NULL" << std::endl;
    return text_surf;
}
#pragma endregion 

#pragma region render_sensors
void Engine::drawRaydars() {
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
            for (float radius = 15; radius < distance(intersect, ray->_origin); radius *= 1.2) {
                rayep = rayEndpoint(ray->_origin, ray->_rot, radius);
                SDL_RenderDrawPointF(_renderer, rayep.x, rayep.y);
            }
        } else {
            if (intersect.x != -1) {
                position rayep;
                for (float radius = 15; radius < distance(intersect, ray->_origin); radius *= 1.2) {
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
}

void Engine::drawCamRays() {
    // draw camera rays
    SDL_SetRenderDrawColor(_renderer, SOFT_WHITE.r, SOFT_WHITE.g, SOFT_WHITE.b, SOFT_WHITE.a);
    // SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    std::vector<SDL_Vertex> verts;
    verts.push_back({{car->getCenter().x, car->getCenter().y}, SOFT_WHITE} );
    // verts.push_back({{0, 0}, {255,255,255,0}} );
    // verts.push_back({{300, 300}, {255,255,255,0}} );
    // verts.push_back({{car->getCenter().x, car->getCenter().y}, SOFT_WHITE} );
    // verts.push_back({{600, 600}, {255,255,255,0}} );
    // verts.push_back({{900, 900}, {255,255,255,0}} );
    // verts.push_back({{0, 0}, {255,255,255,0}} );
    
    std::vector<Ray *> camRays = car->getCamera()->rays;
    
    position prev = {-1, -1};
    bool prev_direction = -1;
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

        if (i == 0 || i == camRays.size() - 1)
            SDL_RenderDrawLineF(_renderer, camRays[i]->_origin.x, camRays[i]->_origin.y, intersect.x, intersect.y);
        // if (prev.x == -1) {
        //     prev = intersect;
        //     verts.push_back({{intersect.x, intersect.y}, {255,255,255,0}} );
        //     continue;
        // }

        // if (prev_direction == -1) {
        //     prev_direction = prev.direction(intersect);
        //     prev = intersect;
        //     continue;
        // }

        // if (prev.direction(intersect) != prev_direction) {
        //     prev_direction = !prev_direction;
        //     prev = intersect;

        //     verts.push_back({{intersect.x, intersect.y}, {255,255,255,0}} );
        //     verts.push_back({{car->getCenter().x, car->getCenter().y}, SOFT_WHITE} );
        //     verts.push_back({{intersect.x, intersect.y}, {255,255,255,0}} );
        //     continue;
        // }
        // prev = intersect;
        // if (i == camRays.size() - 1) {
        //     verts.push_back({{intersect.x, intersect.y}, {255,255,255,0}} );
        // }
        // else {
        //     // if (distance(temp, camRays[i]->_origin) > 50) {
        //     //     intersect = rayEndpoint(camRays[i]->_origin, camRays[i]->_rot, 50);
        //     // }
        //     // SDL_RenderDrawLineF(_renderer, camRays[i]->_origin.x, camRays[i]->_origin.y, intersect.x, intersect.y);
        // }
        // if (prev.x == -1 || )
    }
    // SDL_RenderGeometry(_renderer, NULL, verts.data(), verts.size() - verts.size() % 3, NULL, 0);
}
#pragma endregion 

#pragma region render_objects
void Engine::renderObstacles() {
    // draw obstacles
    if (!sim_view)
        for (auto &ob : obstacles)
            render(ob.get());
}

void Engine::renderCar() {

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
    
    control s = {vel, yaw, dt};

    // car->getIMU()->estimatePose()

    // update simcar and car
    scar->feedIMU(s, car->getIMU()->estimatePose());
    car->setLastUpdate(pos_t);

    // render rays
    drawCamRays();
    drawRaydars();

    // draw car
    render(car.get());
    render(scar.get());
}
#pragma endregion

#pragma region render
void Engine::render(Object *obj, SDL_Texture *texture) {
    SDL_Rect src = {0, 0, obj->getDim().w, obj->getDim().h};
    SDL_FRect dst = {obj->getPos().x, obj->getPos().y, (float)obj->getDim().w, (float)obj->getDim().h};
    if (texture)
        SDL_SetRenderTarget(_renderer, texture);

    if (obj->getAngle() != 0) {
        float x_center = (obj->getDim().w / 2);
        float y_center = (obj->getDim().h / 2);
        SDL_FPoint center = {x_center, y_center};
        SDL_RenderCopyExF(_renderer, obj->getTexture(), &src, &dst, toDeg(obj->getAngle()), &center, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopyF(_renderer, obj->getTexture(), &src, &dst);
    }

    if (texture)
        SDL_SetRenderTarget(_renderer, NULL);
}
#pragma endregion

void Engine::run() {
    // refresh the canvas
    onResizeCanvas();
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _background, NULL, NULL);
    
    // render objects
    renderObstacles();
    renderCar();

    if (car->getLastTs() - lastUpdate > 100) {
        lastUpdate = car->getLastTs();
        SDL_FreeSurface(_stat_surface);
        _stat_surface = updateStats();
        SDL_FreeSurface(_speed_surface);
        _speed_surface = showSpeed();
    }

    // show stats
    SDL_Texture * _stat_texture = SDL_CreateTextureFromSurface(_renderer, _stat_surface);
    SDL_Rect dest;
    dest.x = 10;
    dest.y = ScreenDim.h - (_stat_surface->h) - 10;
    dest.w = _stat_surface->w;
    dest.h = _stat_surface->h;
    SDL_RenderCopy(_renderer, _stat_texture, NULL, &dest);
    SDL_DestroyTexture(_stat_texture);

    // show speed
    SDL_Texture * _speed_texture = SDL_CreateTextureFromSurface(_renderer, _speed_surface);
    dest.x = 10;
    dest.y = 10;
    dest.w = _speed_surface->w;
    dest.h = _speed_surface->h;
    SDL_RenderCopy(_renderer, _speed_texture, NULL, &dest);
    SDL_DestroyTexture(_speed_texture);

    // display
    SDL_RenderPresent(_renderer);
}
