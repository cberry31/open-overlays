// This is a debug server to host the frontend on port 9000
const http = require('http');
const fs = require('fs');
const path = require('path');

http.createServer((req, res) => {
    const filePath = path.join(__dirname, 'public', req.url === '/' ? 'dash.html' : req.url);
    
    fs.readFile(filePath, (err, content) => {
        if (err) {
            res.writeHead(404);
            res.end('File not found');
            return;
        }
        res.writeHead(200);
        res.end(content);
    });
}).listen(9000);

