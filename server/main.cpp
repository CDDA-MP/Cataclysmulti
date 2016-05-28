#include <uv.h>
#include <json11.hpp>

#include "network.h"

int main(int argc, char** argv) {
    Network::bind("0.0.0.0", 8000);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}