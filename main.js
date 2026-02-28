import { thingSpeakConfig } from "./thingspeak-config.js";

// DOM Elements
const elements = {
    airPpm: document.getElementById('air-ppm'),
    airStatus: document.getElementById('air-status'),
    airMeter: document.getElementById('air-meter'),
    airCard: document.getElementById('air-card'),
    tempVal: document.getElementById('temp-val'),
    humVal: document.getElementById('hum-val'),
    ledGreen: document.getElementById('led-green'),
    ledYellow: document.getElementById('led-yellow'),
    ledRed: document.getElementById('led-red'),
    buzzerBox: document.getElementById('buzzer-box'),
    buzzerStatus: document.getElementById('buzzer-status'),
    notificationBox: document.getElementById('notification-box'),
    lastUpdate: document.createElement('p') // To show last update time
};

// Add last update time to footer or header
document.querySelector('header').appendChild(elements.lastUpdate);
elements.lastUpdate.style.fontSize = '0.7rem';
elements.lastUpdate.style.color = 'var(--text-secondary)';

// Configuration
const METER_CIRCUMFRENCE = 283;
let lastAirStatus = "";

function showNotification(title, message) {
    if (elements.notificationBox.children.length > 0) return; // Prevent spam

    const alert = document.createElement('div');
    alert.className = 'alert-box';
    alert.innerHTML = `
        <div class="alert-icon">⚠️</div>
        <div class="alert-content">
            <h4>${title}</h4>
            <p>${message}</p>
        </div>
    `;
    elements.notificationBox.appendChild(alert);

    // Remove after 5 seconds
    setTimeout(() => {
        alert.style.opacity = '0';
        alert.style.transition = 'opacity 0.5s ease';
        setTimeout(() => alert.remove(), 500);
    }, 5000);
}
function updateUI(data) {
    const air = parseFloat(data.field3);
    const temp = parseFloat(data.field1);
    const hum = parseFloat(data.field2);

    if (isNaN(air) || isNaN(temp) || isNaN(hum)) return;

    // Update Values
    elements.airPpm.textContent = Math.round(air);
    elements.tempVal.textContent = temp.toFixed(1);
    elements.humVal.textContent = Math.round(hum);
    elements.lastUpdate.textContent = `Last sync: ${new Date(data.created_at).toLocaleTimeString()}`;

    // Air Meter SVG
    const progress = Math.min(air / 1000, 1);
    const offset = METER_CIRCUMFRENCE - (progress * METER_CIRCUMFRENCE);
    elements.airMeter.style.strokeDashoffset = offset;

    // Logic Sync
    let status = "Initializing...";
    let statusClass = "status-good";

    elements.ledGreen.classList.remove('active');
    elements.ledYellow.classList.remove('active');
    elements.ledRed.classList.remove('active');
    elements.buzzerBox.classList.remove('buzzer-active');

    if (air < 300) {
        status = "Good";
        statusClass = "status-good";
        elements.ledGreen.classList.add('active');
        elements.buzzerStatus.textContent = "Quiet";
        elements.airMeter.style.stroke = "var(--accent-green)";
    }
    else if (air < 600) {
        status = "Moderate";
        statusClass = "status-moderate";
        elements.ledYellow.classList.add('active');
        elements.buzzerStatus.textContent = "Quiet";
        elements.airMeter.style.stroke = "var(--accent-yellow)";
    }
    else {
        status = "Poor";
        statusClass = "status-poor";
        elements.ledRed.classList.add('active');
        elements.buzzerBox.classList.add('buzzer-active');
        elements.buzzerStatus.textContent = "Alerting!";
        elements.airMeter.style.stroke = "var(--accent-red)";

        // Trigger Notification
        if (lastAirStatus !== "Poor") {
            showNotification("Hazardous Air Quality!", "Air quality is poor. Purifier and alarm activated.");
        }
    }

    lastAirStatus = status;
    elements.airStatus.textContent = status;
    elements.airStatus.className = `air-status ${statusClass}`;
}

async function fetchThingSpeakData() {
    const url = `https://api.thingspeak.com/channels/${thingSpeakConfig.channelId}/feeds.json?api_key=${thingSpeakConfig.readApiKey}&results=1`;

    try {
        const response = await fetch(url);
        const data = await response.json();
        if (data.feeds && data.feeds.length > 0) {
            updateUI(data.feeds[0]);
        }
    } catch (error) {
        console.error("ThingSpeak Fetch Error:", error);
    }
}

// Start polling
fetchThingSpeakData();
setInterval(fetchThingSpeakData, thingSpeakConfig.pollInterval);

console.log("Aether Connected to ThingSpeak.");
