local net = require "net"
local json = require "json"
local callbacks = {}
local server = net.createServer(function(client)
client:write('{"cmd":"colormap","map":{"custom":[1,3,0],"another":[3,2,0]}},') --Send the custom color.
client:write('{"cmd":"msgbox","str":"Hello,world!\\nColor <color_red>Test</color>!"},')
client:write('{"cmd":"msgbox","str":"This is the <color_custom>second</color> message box!\\nThis line will display with <color_another>another custom color</color>!"},')

client:on("data",function(data)
    local js = json.decode(data) or {}
    (callbacks[js.cmd] or p)(client,js)
  end)
  print("Connected!")
  
end)
callbacks["metadata"] = function(client,js)
print("recived metadata:","\n  client:\t",js.client,"\n  version:\t",js.ver,"\n  name:\t",js.name)

end

server:listen(8000, "127.0.0.1")