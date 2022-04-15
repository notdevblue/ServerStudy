const { WebSocketServer } = require("ws");
const express = require('express');
const app = express();
const router = express.Router();
const ws = new WebSocketServer({ port: 38000 }, () => {
   console.log("Server running on port 38000");
});

app.use(router);

router.get("/", (req, res) => {
   res.send("Hello!");
});

ws.on("connection", (client) => {
   console.log("Client connected");

   client.on("message", (data) => {
      console.log(data.toString());
      setInterval(() => {
         client.send("Hello I am server!");
      }, 1000);
   });
});

ws.on("close", (client) => {
   console.log("Byebye");
});