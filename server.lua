--Run this using luvit.
local net = require ("net")

local server = net.createServer(function(client)
  client:on("data",function(data)
    client:write(data)
  end)
  print("Connected!")
  client:write("\xffCHECK\x00\xFEARG1\x00")
end)

server:listen(8000, "127.0.0.1")
