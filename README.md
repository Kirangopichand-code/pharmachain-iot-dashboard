# 💊 PharmaChain: FDA-Compliant Drug Traceability System

### 🚀 Overview
PharmaChain is an IoT-based drug traceability and monitoring system designed to ensure the safety, integrity, and compliance of pharmaceutical shipments.  
It integrates **ESP32**, **AWS IoT Core**, and **Node-RED Dashboard** to simulate real-time tracking of temperature, humidity, location, and seal status — aligning with **FDA-compliant logistics standards**.

## 🧠 System Architecture

flowchart LR
A[ESP32 Sensor Node<br/>DHT11 + GPS (Simulated)] --> B[AWS IoT Core<br/>MQTT over TLS]
B --> C[Node-RED Flow<br/>JSON Parsing & Processing]
C --> D[FlowFuse Dashboard<br/>FDA Traceability Visualization]
D --> E[Real-Time Monitoring Interface<br/>For Alerts & Visualization]

## Import the .json file into your Flow:

In Node-RED, click on the ☰ Menu (top-right corner)
Select Import > Upload a file
Choose the file:
Click Import > Deploy
or run this in cmd (node_red_flows/pharmachain_dashboard.json)
