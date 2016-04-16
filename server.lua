--This script designed for testing Cataclysmulti
--Run this by luvit.
local net = require ("net")

local server = net.createServer(function(client)
  client:on("data",function(data)
    print(data..'\n')
  end)
  print("Connected!")
  client:write('{"cmd":"printVersion","version":"1.0"}')
end)

server:listen(8000, "127.0.0.1")
