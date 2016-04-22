--Run this by luvit.
local net = require "net"
local json = require "json"
local callbacks = {}
local server = net.createServer(function(client)
client:on("data",function(data)
    local js = json.decode(data) or {}
    (callbacks[js.cmd] or p)(js)
  end)
  print("Connected!")
  
end)
callbacks["metadata"] = function(js)
print("recived metadata:","\n  client:\t",js.client,"\n  version:\t",js.ver,"\n  name:\t",js.name)

end

server:listen(8000, "127.0.0.1")
