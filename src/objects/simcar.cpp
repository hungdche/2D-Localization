#include "simcar.h"

SimCar::SimCar(state s, velocity vel, velocity w, dimension dim, SDL_Texture * texture) 
    : estimated{s}, _vel{vel}, _w{w}, _dim{dim}, _texture{texture} { 
        Parser parser("../configs/config.yaml");
        alg = std::make_unique<EKF>(s, parser.Q, parser.R);
    }

SimCar::~SimCar() {
    SDL_DestroyTexture(_texture);
}

void SimCar::feedIMU(control c, state s) {
    predicted = alg->predict(c);
    measured = s;
    alg->update(s);

    estimated = alg->dumpPose();
}



