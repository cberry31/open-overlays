// Connect to the server using WebSocket
const ws = new WebSocket('ws://192.168.1.49:8080');

const messageDiv = document.getElementById('message');

ws.onopen = () => {
    console.log('Connected to server');
};

ws.onmessage = (event) => {
    messageDiv.textContent = event.data;
};

ws.onerror = (err) => {
    messageDiv.textContent = 'Error connecting to server.';
    console.error('WebSocket error:', err);
};

ws.onclose = () => {
    messageDiv.textContent = 'Connection closed.';
};
