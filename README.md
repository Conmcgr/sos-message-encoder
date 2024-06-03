# SOS Message Encoder

## Overview
The SOS Message Encoder is a tool designed to encode and decode messages using a custom format. It leverages a series of transformations to convert plain text messages into a packed binary format suitable for transmission or storage, and can similarly decode such messages back into readable text.

The table below gives an overview of how messages can be encoded:


<img width="422" alt="Screenshot 2024-06-02 at 9 39 57 PM" src="https://github.com/Conmcgr/sos-message-encoder/assets/42752783/d999d948-5fed-4361-89d6-3f8b7f1e079f">

## Features
- **Encode Messages:** Convert plain text into a special encoded format.
- **Decode Messages:** Revert the special encoded format back into plain text.
- **CLI Support:** Command line interface for easy interaction.

## Getting Started

### Prerequisites
- GCC compiler
- Standard C library

## To Compile and Use the Project
Compiling: run "make all"
Usage: run ./myprogram

## Architecture
Main Components:

cli.c/h: Handles the command line interface interactions.
message_encode.c/h: Contains the core logic for encoding and decoding messages.
main.c: Entry point of the application which sets up the environment and initiates the CLI.
Data Flow:

The user inputs a message through the CLI.
message_encode.c processes the input to either encode or decode the message.
The result is output to the user through the CLI.
