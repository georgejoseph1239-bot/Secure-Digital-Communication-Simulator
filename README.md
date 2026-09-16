# Secure Digital Communication Simulator

A modular **ECE project** that demonstrates how a digital message can be protected, converted into digital data, modulated, transmitted through a noisy channel, analyzed, and recovered at the receiver.

The project combines **C-based communication-system simulation** with a modern browser-based **SecureLink** demonstration interface.

## 🌐 Live Demo

**SecureLink Website:**  
https://georgejoseph1239-bot.github.io/Secure-Digital-Communication-Simulator/web/

**GitHub Repository:**  
https://github.com/georgejoseph1239-bot/Secure-Digital-Communication-Simulator

---

## 📌 Project Overview

The simulator follows a simplified digital communication chain:

```text
Message
   ↓
Encryption
   ↓
Binary Conversion
   ↓
BPSK Modulation
   ↓
AWGN Noise Channel
   ↓
FFT / Signal Analysis
   ↓
BPSK Demodulation
   ↓
BER & SNR Analysis
   ↓
Authentication / Integrity Verification
   ↓
Decryption
   ↓
Original Message
```

The goal is to connect important **Digital Communication**, **Signal Processing**, **Programming**, and **Cybersecurity** concepts in one project.

---

## ✨ Main Features

### 🔐 Secure Encryption

The C simulator uses authenticated encryption through **libsodium**.

- XSalsa20 stream encryption
- Poly1305 authentication
- Random nonce generation
- Authentication and integrity verification
- Tampered ciphertext can be rejected

> The C encryption path is intended as a secure cryptographic implementation for the project. Password-to-key derivation is kept simple for demonstration and can be upgraded to a dedicated password KDF for a production system.

### 📡 BPSK Modulation

The encrypted data is converted into bits and transmitted using **Binary Phase Shift Keying (BPSK)**.

- `1` → 0° phase
- `0` → 180° phase
- Carrier-based modulation
- Correlation-based demodulation

### 🌫️ AWGN Channel

The simulator models transmission through an **Additive White Gaussian Noise (AWGN)** channel.

Different noise levels can be tested to observe their effect on communication quality.

### 📊 SNR and BER Analysis

The project calculates:

- Signal power
- Noise power
- SNR in dB
- Bit errors
- Bit Error Rate (BER)

It also performs an **SNR vs BER analysis** over multiple noise levels.

### 📈 FFT Signal Analysis

A small FFT-style frequency-domain analysis is included to examine the transmitted BPSK signal.

### 🌐 SecureLink Web Interface

The `web/` directory contains a browser-based demonstration interface.

The website includes:

- Secure communication dashboard
- Sender and friend/session names
- Password-protected demo session
- Message input
- Real AES-256-GCM browser encryption
- Random initialization vector
- Real ciphertext display
- Full-screen transmission animation
- Authentication verification
- Decryption demonstration
- Transmission log
- Modern responsive UI

The web demo is designed as a **standalone demonstration**, so it does not require the C simulator to be running.

---

## 🛠️ Technologies Used

### C / Communication Simulation

- C
- GCC
- Libsodium
- Math library
- BPSK
- AWGN
- SNR
- BER
- FFT

### Web

- HTML5
- CSS3
- JavaScript
- Web Crypto API
- AES-256-GCM

### Development

- Visual Studio Code
- Git
- GitHub
- GitHub Pages
- Node.js / WebSocket server components

---

## 📁 Project Structure

```text
Secure-Digital-Communication-Simulator/
│
├── include/
│   ├── ber.h
│   ├── channel.h
│   ├── decoding.h
│   ├── demodulation.h
│   ├── encryption.h
│   ├── fft.h
│   ├── modulation.h
│   ├── security.h
│   ├── snr.h
│   └── snr_ber.h
│
├── src/
│   ├── main.c
│   ├── security.c
│   ├── modulation.c
│   ├── channel.c
│   ├── demodulation.c
│   ├── decoding.c
│   ├── ber.c
│   ├── snr.c
│   ├── snr_ber.c
│   └── fft.c
│
├── web/
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── server/
│   ├── server.js
│   └── package.json
│
├── tests/
├── docs/
└── README.md
```

The modular structure makes it possible to improve one part of the system without rewriting the complete project.

---

## 🚀 Running the C Simulator

### Requirements

Install:

- GCC
- Libsodium

### Compile

From the project root:

```bash
gcc -Iinclude src/main.c src/security.c src/modulation.c src/channel.c src/demodulation.c src/decoding.c src/ber.c src/snr.c src/snr_ber.c src/fft.c -o simulator -lsodium -lm
```

### Run

```bash
./simulator
```

The simulator will request a message and security password and then perform the communication process.

---

## 🌐 Running the Website Locally

The website is located in:

```text
web/
```

You can open:

```text
web/index.html
```

directly in a browser.

The live GitHub Pages version is also available at:

https://georgejoseph1239-bot.github.io/Secure-Digital-Communication-Simulator/web/

---

## 🔄 Example Communication Process

Suppose the sender enters:

```text
Hello Friend
```

The system performs:

```text
Hello Friend
      ↓
AES-256-GCM / Authenticated Encryption
      ↓
Ciphertext
      ↓
Binary Data
      ↓
BPSK
      ↓
AWGN Channel
      ↓
Received Signal
      ↓
BPSK Demodulation
      ↓
Recovered Ciphertext
      ↓
Authentication Verification
      ↓
Decryption
      ↓
Hello Friend
```

The receiver should obtain the same original message when authentication and transmission recovery succeed.

---

## 🧩 Modular Design

One of the main goals of this project is to keep the system **easy to extend**.

Each major function is separated into its own module:

```text
Encryption
Modulation
Channel
Demodulation
Decoding
BER
SNR
FFT
Security
Web Interface
```

This means new functionality can be added without replacing the complete communication chain.

### Possible future additions

The project can be extended by adding individual modules such as:

- QPSK modulation
- QAM modulation
- FSK modulation
- PSK variants
- Channel coding
- Error-correction codes
- Interleaving
- Digital filters
- Larger FFT implementation
- Eye-diagram analysis
- Adaptive noise/channel models
- Improved password-based key derivation
- User authentication
- Message history
- Database storage
- Real-time Internet messaging
- Cloud-hosted communication server
- End-to-end encrypted browser communication

These are **optional extensions** rather than requirements for the current simulator.

---

## 🔒 Security Note

The project contains two separate demonstration paths:

### C Simulator

Uses:

```text
libsodium
   ↓
crypto_secretbox
   ↓
XSalsa20 + Poly1305
```

### Browser Demo

Uses the browser's built-in:

```text
Web Crypto API
   ↓
AES-256-GCM
```

The two implementations demonstrate secure authenticated encryption independently. They are not currently presented as a single interoperable cryptographic protocol.

For a production communication system, additional protocol design, secure key exchange, authenticated user identity, secure password-based key derivation, replay protection, and secure transport would be required.

---

## 🎓 Educational Objectives

This project demonstrates practical understanding of:

- Digital communication systems
- Binary data representation
- BPSK modulation and demodulation
- AWGN channel modeling
- Signal-to-noise ratio
- Bit error rate
- Frequency-domain analysis
- FFT concepts
- Authenticated encryption
- Data integrity
- C modular programming
- Browser cryptography
- Basic client/server communication concepts

---

## 📌 Project Status

**Current Status: Working Prototype / Educational Simulator**

Implemented:

- [x] Secure message encryption
- [x] Binary conversion
- [x] BPSK modulation
- [x] AWGN channel
- [x] BPSK demodulation
- [x] BER calculation
- [x] SNR calculation
- [x] SNR vs BER analysis
- [x] FFT signal analysis
- [x] Authentication and integrity verification
- [x] SecureLink web interface
- [x] GitHub Pages deployment

Possible future development:

- [ ] Additional modulation schemes
- [ ] Advanced channel coding
- [ ] Production-grade key derivation
- [ ] Persistent messaging
- [ ] Cloud deployment
- [ ] Full real-time multi-user communication

---

## 👨‍💻 Author

**George Joseph**

Electronics & Communication Engineering

GitHub:  
https://github.com/georgejoseph1239-bot

---

## 📄 License

This project is intended primarily for **educational, academic, and portfolio purposes**.

A formal open-source license can be added later if required.
