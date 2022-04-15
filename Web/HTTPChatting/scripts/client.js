const socket = new WebSocket("ws://localhost:38000/");

socket.onopen = (event) => {
   socket.send("Hello I am client!");
};


socket.onmessage = (event) => {
   console.log(event.data);

   var parent = document.getElementById("parent");
   var chat = document.createElement("div");
   chat.innerHTML = event.data;
   chat.setAttribute("id", "chat");
   chat.style.backgroundColor = "yellow";
   parent.appendChild(chat);
}

// var obj = document.getElementById("parent");
// var newDIV = document.createElement("div");

// newDIV.innerHTML = "새로 생성된 DIV";
// newDIV.setAttribute("id", "myDiv");
// newDIV.style.backgroundColor = "yellow";
// obj.appendChild(newDIV);