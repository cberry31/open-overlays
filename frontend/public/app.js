// Connect to the server using WebSocket
const ws = new WebSocket('ws://192.168.1.49:8080');

const messageDiv = document.getElementById('message');
const gearDiv = document.getElementById('gear');

ws.onopen = () => {
    console.log('Connected to server');
};

ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    if (data.gear !== undefined) {
        gearDiv.textContent = `${data.gear}`;
    }
};

ws.onerror = (err) => {
    messageDiv.textContent = 'Error connecting to server.';
    console.error('WebSocket error:', err);
};

ws.onclose = () => {
    messageDiv.textContent = 'Connection closed.';
};
