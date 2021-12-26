const http = require("http");
const net = require("net");
const url = require("url");
const fs = require("fs");
const socketIo = require("socket.io");
const sys = require(process.binding("natives").util ? "util" : "sys");
let server;

const tcpGuests = [];
const chatGuests = [];

(server = http.createServer(function (req, res) {
  // your normal server code
  var path = url.parse(req.url).pathname;
  switch (path) {
    case "/React_page/public/index.html":
      fs.readFile(__dirname + path, function (err, data) {
        if (err) return send404(res);
        res.writeHead(200, {
          "Content-Type": path == "json.js" ? "text/javascript" : "text/html",
        });
        res.write(data, "utf8");
        res.end();
      });
      break;

    default:
      send404(res);
  }
})),
  (send404 = function (res) {
    res.writeHead(404);
    res.write("404");
    res.end();
  });

server.listen(8090, () => console.log("Server is listening"));

// socket.io, I choose you
// simplest chat application evar
const io = socketIo(server, {
    cors: {
      origin: "*",
    },
  }),
  buffer = [];

// io.on("connection", function (client) {
//   console.log("Frontend listening!");
//   chatGuests.push(client);

//   client.on("message", function (message) {
//     var msg = { message: [client.sessionId, message] };
//     //send msg to tcp connections
//     for (g in tcpGuests) {
//       tcpGuests[g].write(message);
//     }
//   });

//   //raczej dowywalenia
//   client.on("disconnect", function () {
//     client.broadcast.send({ announcement: client.sessionId + " disconnected" });
//   });
// });



//tcp socket server
var tcpServer = net.createServer(function (socket) {
  console.log("Arduino server running on port 1337");
  console.log("Web server running on http://localhost:8090");
});
var toSend=[];
var i=0;
tcpServer.on("connection", function (socket) {
  socket.write("connected to the tcp server\r\n");
  //console.log("num of connections on port 1337: " + tcpServer.connections);

  tcpGuests.push(socket);
    socket.on("data", function (data) {
      console.log("received on tcp socket:" + data);
      toSend[i]=data.toString();
      i=i+1;
      if(i>2) i=0;
      ///toSend = data.toString();
      console.log(toSend[0]);
      console.log(toSend[1]);
      console.log(toSend[2]);
      io.emit("tempK", toSend[0]);
      io.emit("tempE", toSend[1]);
      io.emit("tempA", toSend[2]);
    });
      
});

io.on("connection", (socket) => {
  console.log("Client connected");
  chatGuests.push(socket);
  socket.on("message", function (message) {    
    for (g in tcpGuests) {
      tcpGuests[g].write(message);
      console.log(message);
    }
  });
  

});

tcpServer.listen(1337);
