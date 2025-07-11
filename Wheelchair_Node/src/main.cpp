#include <Arduino.h>
#include <painlessMesh.h>
#include <ArduinoJson.h>

#define MESH_PREFIX     "WheelchairMesh"
#define MESH_PASSWORD   "meshsecure"
#define MESH_PORT       5555

Scheduler userScheduler;
painlessMesh mesh;

String wheelchairID = "WC-01";  // Unique ID for this wheelchair

// This function sends the wheelchair presence
void sendBroadcastMessage() {
  StaticJsonDocument<200> doc;
  doc["wheelchairID"] = wheelchairID;

  String msg;
  serializeJson(doc, msg);

  mesh.sendBroadcast(msg);
  Serial.printf("📤 Sent presence: %s\n", msg.c_str());
}

// Create a repeating task every 3 seconds
Task sendTask(3000, TASK_FOREVER, &sendBroadcastMessage);

void setup() {
  Serial.begin(115200);

  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);

  userScheduler.addTask(sendTask);
  sendTask.enable();

  Serial.println("🦼 Wheelchair Node Started...");
}

void loop() {
  mesh.update();
}
