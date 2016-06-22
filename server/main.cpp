#include <uv.h>
#include <json11.hpp>

#include "network.h"
#include "map.h"

int main(int argc, char** argv) {
    Map::init();
    Network::bind("0.0.0.0", 8000);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}