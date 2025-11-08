# 💊 PharmaChain: FDA-Compliant Drug Traceability System

### 🚀 Overview
PharmaChain is an IoT-based drug traceability and monitoring system designed to ensure the safety, integrity, and compliance of pharmaceutical shipments.  
It integrates **ESP32**, **AWS IoT Core**, and **Node-RED Dashboard** to simulate real-time tracking of temperature, humidity, location, and seal status — aligning with **FDA-compliant logistics standards**.

---

## 🧠 System Architecture


A[ESP32 Sensor Node<br/>DHT11 + GPS (Simulated)] --> B[AWS IoT Core<br/>MQTT over TLS]
B --> C[Node-RED Flow<br/>JSON Parsing & Processing]
C --> D[FlowFuse Dashboard<br/>FDA Traceability Visualization]
D --> E[Real-Time Monitoring Interface<br/>For Alerts & Visualization]

## AWS IoT Core Setup:
Create an AWS IoT Thing & Certificates (one-time)
Open AWS Console → IoT Core → Manage → Things → Create thing → create a single thing (name e.g., pharmachain-device1).
In Security → Certificates → Create certificate → Download the device certificate and private key and the AmazonRootCA1.pem.
Activate the certificate.
Attach policy to certificate (policy must allow iot:Connect, iot:Publish, iot:Subscribe, iot:Receive — for dev you can use Action * on Resource *).
(Optional) Attach certificate to your Thing.

## Get your Endpoint URL:
AWS Console > IoT Core > Settings > copy the Device data endpoint
It looks like: a1b2c3d4e5f6g7-ats.iot.<region>.amazonaws.com

