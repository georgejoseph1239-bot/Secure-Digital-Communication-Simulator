const WebSocket = require("ws");

const PORT = 8080;

const server = new WebSocket.Server({
    port: PORT
});

const clients = new Set();

console.log("=================================");
console.log("   SecureLink Chat Server");
console.log("=================================");
console.log(`Server running on port ${PORT}`);
console.log("Waiting for users...\n");


server.on("connection", function(socket)
{
    clients.add(socket);

    console.log(
        `Client connected. Total clients: ${clients.size}`
    );


    socket.send(JSON.stringify({
        type: "system",
        message: "Connected to SecureLink server"
    }));


    socket.on("message", function(data)
    {
        console.log(
            "Message received:",
            data.toString()
        );


        /*
         * Relay the message to every other
         * connected client.
         */

        for (const client of clients)
        {
            if (
                client !== socket &&
                client.readyState === WebSocket.OPEN
            )
            {
                client.send(data.toString());
            }
        }
    });


    socket.on("close", function()
    {
        clients.delete(socket);

        console.log(
            `Client disconnected. Total clients: ${clients.size}`
        );
    });


    socket.on("error", function(error)
    {
        console.log(
            "WebSocket error:",
            error.message
        );
    });
});


server.on("error", function(error)
{
    console.log(
        "Server error:",
        error.message
    );
});