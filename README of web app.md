# Aether: IoT Air Purifier Web Dashboard

## Project Overview
Aether is a modern, responsive frontend web application designed to monitor live environmental data. Built with Vite, this dashboard interfaces seamlessly with an ESP32 IoT node via the ThingSpeak REST API. It visualizes real-time air quality, temperature, and humidity metrics while providing a virtual dashboard of the physical hardware's state.

## Key Features
* **Live Data Streaming:** Automatically polls the ThingSpeak cloud every 15 seconds to fetch the latest sensor readings.
* **Dynamic Hardware Mirroring:** Features a virtual hardware dashboard that visually mirrors the physical ESP32's LED indicators (Good/Moderate/Poor) and active buzzer state.
* **Hazard Notifications:** Implements a custom, on-screen alert system that triggers a slide-down warning banner when hazardous air quality (high PPM) is detected.
* **Interactive UI:** Includes custom CSS animations, a pulsating connection badge, and a dynamic SVG meter that visually fills based on the live air quality data.
* **Modern Tooling:** Packaged, built, and served using Vite for optimized, lightning-fast performance.

## Tech Stack
* **Frontend:** HTML5, CSS3 (Custom animations & responsive grid)
* **Logic:** Vanilla JavaScript (ES6 Modules)
* **Build Tool:** Vite
* **Backend/API:** ThingSpeak Cloud

## Setup & Local Installation

### Prerequisites
* Node.js installed on your machine.

### Installation Steps
1. Clone this repository to your local machine.
2. Open the project folder in your terminal.
3. Install the required dependencies:
   ```bash
   npm install
