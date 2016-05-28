#include "protocol.h"

#include <unordered_map>
#include <json11.hpp>

#include "network.h"
#include "input.h"
#include "output.h"
#include "color.h"
#include "version.h"
#include "game.h"

namespace Network {
    namespace Protocol {
        static void msgbox(const json11::Json &json) {
            UI::Msgbox(json["str"].string_value());
        }

        static void colormap(const json11::Json &json) {
            if (!json["map"].is_object()) {
                return;
            }

            std::map<std::string, json11::Json> map = json["map"].object_items();

            for (auto & i : map) {
                json11::Json::array array;

                if (i.second.is_array() && (array = i.second.array_items()).size() == 3 && array[0].is_number() &&
                        array[1].is_number() && array[2].is_number()) {
                    addColor(i.first, (short) array[0].number_value(), (short) array[1].number_value(),
                             (chtype) array[2].number_value());
                }
            }
        }

        std::unordered_map<std::string, Network::Protocol::Callback> Callbacks {
            {"msgbox", msgbox },
            {"colormap", colormap },
        };

        void call_Callback(const json11::Json &json) {
            if (!json["cmd"].is_string()) {
                return;
            }

            Network::Protocol::Callback t = Network::Protocol::Callbacks[json["cmd"].string_value()];

            if (t) {
                t(json);
            }

        }

        namespace Out {
            void sendmeta() {
                Network::send(json11::Json::object {
                    {"cmd", "metadata" },
                    {"client", VERSION_NAME },
                    {"ver", VERSION_PREFIX + " " + VERSION_VERSION },
                    {"userid", Game::userid },
                });
            }
        }
    }
}
