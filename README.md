# Simple Server
![License](https://img.shields.io/github/license/LonerLena/SimpleServer?style=for-the-badge) ![Issues](https://img.shields.io/github/issues-raw/lonerlena/simpleserver?style=for-the-badge)

## Why
At first I was working on a simple chat server as a project. Eventually I wanted several features and I kinda felt like it could be of use for others.

## Setup
Include the server header file, and compile your project with the server file included.

## Usage
Start server: `serverStart(PORT);`
Accept client: `serverAccept();`
Close client connection: `serverClose(CLIENT);`
Send message to client: `serverSend(CLIENT, MESSAGE);`
etc.