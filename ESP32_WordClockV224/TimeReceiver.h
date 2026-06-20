// ============================================================================
// TimeReceiver.h  -  BLE TimeReceiver client (NimBLE-Arduino v2.x)
//
// Reworked so that:
//   * BLE callbacks ONLY set flags (no allocation, no deleteClient, no rescan)
//   * Every deleteClient / scan->stop / rescan runs in loop() context, driven
//     by CheckTimeReceiverClient()
//   * Both callback objects are single STATIC instances passed with `false`
//     (NimBLE never frees them) -> no per-attempt callback-object leak
//
// Connects to a TimeSender peripheral advertising TIMESENDER_DEVICE_NAME.
//
// Requires (already declared in the main sketch):
//   TIMESENDER_DEVICE_NAME, TIMERECEIVER_MIN_RSSI, SERVICE_UUID,
//   CHARACTERISTIC_UUID_TX, CHARACTERISTIC_UUID_RX, CONNECT_TIMEOUT_MS,
//   pClient, pRemoteTX, pRemoteRX, TRfoundAddr, TRhaveFoundAddr,
//   TRconnectRequested, TRConnecting, TRConnected, TRconnectStartms,
//   LastTimeReceiverScan, TimeReceiverRunning, TRConnectedSince,
//   Tekstprintln(), Tekstprintlnf(), ReworkInputString()
// ============================================================================

// ----------------------------------------------------------------------------
// One extra flag (add this with the other TR statics in the main sketch, OR
// leave it here if this header is included after those declarations).
// ----------------------------------------------------------------------------
static volatile bool TRneedsCleanup = false;                                    // Set by onDisconnect, handled in loop()

// Forward declarations (definitions are further down in the sketch)
static void Connect_TimeReceiver(void);
void StartTimeReceiverScan(void);

//--------------------------------------------                                  //
// BLE TimeReceiver RX notify callback
// Runs in the NimBLE host task. Keep it short. ReworkInputString() is heavy
// but is the existing behaviour; it is not BLE-cleanup, so it stays.
//--------------------------------------------
static void TimeReceiverNotifyCB(NimBLERemoteCharacteristic*, uint8_t* data, size_t len, bool notify)
{
 static char buf[128];
 size_t n = (len < sizeof(buf) - 1) ? len : sizeof(buf) - 1;
 memcpy(buf, data, n);
 buf[n] = 0;
 ReworkInputString(String(buf));
 TRConnectedSince = millis();
}

//--------------------------------------------                                  //
// BLE TimeReceiver client callbacks
// onDisconnect ONLY sets flags. All teardown is deferred to loop context.
//--------------------------------------------
class TimeReceiverClientCB : public NimBLEClientCallbacks
{
 void onConnect(NimBLEClient* client) override
   {
    TRConnected      = true;
    TRConnecting     = false;
    TRConnectedSince = millis();
    Tekstprintlnf("TimeReceiver connected to: %s", TIMESENDER_DEVICE_NAME);
   }

 void onDisconnect(NimBLEClient* client, int reason) override
   {
    Tekstprintlnf("TimeReceiver disconnected (reason: %d)", reason);
    TRConnected         = false;
    TRConnecting        = false;
    TRhaveFoundAddr     = false;
    TRconnectRequested  = false;
    TimeReceiverRunning = false;
    pRemoteTX           = nullptr;                                              // Just null our cached pointers - safe in callback
    pRemoteRX           = nullptr;
    TRneedsCleanup      = true;                                                 // Defer deleteClient + rescan to loop context
   }
};

//--------------------------------------------                                  //
// BLE TimeReceiver scan callbacks
//--------------------------------------------
class TimeReceiverScanCB : public NimBLEScanCallbacks
{
 void onResult(const NimBLEAdvertisedDevice* dev) override
   {
    if (TRConnecting) return;
    if (!dev->haveName()) return;
    if (dev->getName().find(TIMESENDER_DEVICE_NAME) == std::string::npos) return;
    if (dev->getRSSI() <= TIMERECEIVER_MIN_RSSI) return;
    if (!dev->isConnectable()) return;
    if (!dev->isAdvertisingService(NimBLEUUID(SERVICE_UUID))) return;
    TRfoundAddr        = dev->getAddress();
    TRhaveFoundAddr    = true;
    TRconnectRequested = true;
    NimBLEDevice::getScan()->stop();
   }
};

//--------------------------------------------                                  //
// Single STATIC callback instances - defined AFTER the classes so the
// compiler can size them. Passed to NimBLE with `false` => never freed.
//--------------------------------------------
static TimeReceiverClientCB trClientCB;
static TimeReceiverScanCB   trScanCB;

//--------------------------------------------                                  //
// BLE TimeReceiver Send message to peer (writes to remote RX characteristic)
//--------------------------------------------
void SendMessageTimeReceiver(const char* msg)
{
 if (!TRConnected || !msg || !pRemoteRX || !pClient) return;
 if (!pClient->isConnected()) { TRConnected = false; return; }
 size_t len = strlen(msg);
 pRemoteRX->writeValue((uint8_t*)msg, len, false);
}

//--------------------------------------------                                  //
// BLE TimeReceiver Start scan (uses STATIC scan callback - no `new`)
//--------------------------------------------
void StartTimeReceiverScan(void)
{
 NimBLEScan* scan = NimBLEDevice::getScan();
 if (TimeReceiverRunning && !scan->isScanning())                                // Flag stale - scan isn't actually running
   {
    TimeReceiverRunning = false;
    Tekstprintln("Flag reset - scan not running");
   }
 if (TimeReceiverRunning) { Tekstprintln("Scan already running - skipping"); return; }
 if (scan->isScanning())  { scan->stop(); delay(100); }
 TimeReceiverRunning = true;
 scan->setActiveScan(true);
 scan->setInterval(160);
 scan->setWindow(30);
 scan->setScanCallbacks(&trScanCB, false);                                      // Static instance, do NOT delete
 if (!scan->start(0, false, false))
   {
    Tekstprintln("ERROR: Failed to start TimeReceiver scan");
    TimeReceiverRunning = false;
   }
 else
   {
    LastTimeReceiverScan = millis();
    Tekstprintln("TimeReceiver scan started");
   }
}

//--------------------------------------------                                  //
// BLE TimeReceiver Stop (loop context only)
//--------------------------------------------
void StopTimeReceiver(void)
{
 NimBLEDevice::getScan()->stop();
 if (pClient)
   {
    if (pClient->isConnected()) pClient->disconnect();
    NimBLEDevice::deleteClient(pClient);                                        // Loop context => safe
    pClient = nullptr;
   }
 TRConnected        = false;
 TRConnecting       = false;
 pRemoteTX          = nullptr;
 pRemoteRX          = nullptr;
 TRhaveFoundAddr    = false;
 TRconnectRequested = false;
 TRneedsCleanup     = false;
}

//--------------------------------------------                                  //
// BLE TimeReceiver Connect (loop context only - all deleteClient here is safe)
//--------------------------------------------
static void Connect_TimeReceiver(void)
{
 if (!TRhaveFoundAddr || TRConnected || TRConnecting) return;
 const int           MAX_CONNECT_ATTEMPTS    = 3;
 const unsigned long POST_DISCONNECT_DELAY_MS = 150;

 for (int attempt = 1; attempt <= MAX_CONNECT_ATTEMPTS; ++attempt)
    {
     NimBLEDevice::getScan()->stop();
     delay(100);
     pClient = NimBLEDevice::createClient();
     pClient->setClientCallbacks(&trClientCB, false);                           // Static instance, do NOT delete
     TRConnecting     = true;
     TRconnectStartms = millis();

     if (!pClient->connect(TRfoundAddr))
        {
         NimBLEDevice::deleteClient(pClient); pClient = nullptr;
         TRConnecting = false; delay(POST_DISCONNECT_DELAY_MS); continue;
        }
     NimBLERemoteService* svc = pClient->getService(SERVICE_UUID);
     if (!svc)
        {
         pClient->disconnect();
         NimBLEDevice::deleteClient(pClient); pClient = nullptr;
         TRConnecting = false; delay(POST_DISCONNECT_DELAY_MS); continue;
        }
     pRemoteTX = svc->getCharacteristic(CHARACTERISTIC_UUID_TX);
     pRemoteRX = svc->getCharacteristic(CHARACTERISTIC_UUID_RX);
     if (!pRemoteTX || !pRemoteRX)
        {
         pClient->disconnect();
         NimBLEDevice::deleteClient(pClient); pClient = nullptr;
         TRConnecting = false; delay(POST_DISCONNECT_DELAY_MS); continue;
        }
     if (!pRemoteTX->subscribe(true, TimeReceiverNotifyCB, true))
        {
         pClient->disconnect();
         NimBLEDevice::deleteClient(pClient); pClient = nullptr;
         TRConnecting = false; delay(POST_DISCONNECT_DELAY_MS); continue;
        }
     TRConnectedSince   = millis();
     TRhaveFoundAddr    = false;
     TRconnectRequested = false;
     TRConnecting       = false;
     return;                                                                    // Connected; onConnect callback sets TRConnected
    }

 if (pClient) { NimBLEDevice::deleteClient(pClient); pClient = nullptr; }
 TRhaveFoundAddr    = false;
 TRconnectRequested = false;
 Tekstprintln("TimeReceiver connection failed - restarting scan");
 StartTimeReceiverScan();
}

//--------------------------------------------                                  //
// BLE TimeReceiver Reconnect (loop context only)
//--------------------------------------------
void ReconnectTimeReceiver(void)
{
 if (pClient)
   {
    if (pClient->isConnected()) pClient->disconnect();
    NimBLEDevice::deleteClient(pClient);
    pClient = nullptr;
   }
 TRConnected        = false;
 TRConnecting       = false;
 TRneedsCleanup     = false;                                                    // Clear pending cleanup; doing it now
 TRhaveFoundAddr    = true;                                                     // Keep the found address
 TRconnectRequested = true;
 delay(200);
 Connect_TimeReceiver();
}

//--------------------------------------------                                  //
// BLE TimeReceiver Check client - the single loop-context driver.
// Handles deferred cleanup, connect requests, timeouts and periodic rescan.
//--------------------------------------------
void CheckTimeReceiverClient(void)
{
 unsigned long now = millis();
 NimBLEScan* scan = NimBLEDevice::getScan();

 if (TRneedsCleanup)                                                            // Deferred teardown from onDisconnect (now in loop context)
   {
    TRneedsCleanup = false;
    if (pClient)
      {
       NimBLEDevice::deleteClient(pClient);
       pClient = nullptr;
      }
    if (scan->isScanning()) { scan->stop(); delay(100); }
    delay(200);
    StartTimeReceiverScan();
    return;                                                                     // Resume from clean state next pass
   }

 if (!scan->isScanning() && TimeReceiverRunning)                                // Flag stale - allow rescanning
   {
    TimeReceiverRunning = false;
    Tekstprintln("TimeReceiverRunning flag reset - ready for rescan");
   }

 if (TRconnectRequested && TRhaveFoundAddr && !TRConnected && !TRConnecting)    // Honour a connect request from the scan callback
   {
    TRconnectRequested = false;
    Connect_TimeReceiver();
   }

 if (TRConnecting && (now - TRconnectStartms > CONNECT_TIMEOUT_MS))             // Connection attempt timed out
   {
    if (pClient)
      {
       if (pClient->isConnected()) pClient->disconnect();
       NimBLEDevice::deleteClient(pClient); pClient = nullptr;
      }
    TRConnecting        = false;
    TRhaveFoundAddr     = false;
    TRconnectRequested  = false;
    TimeReceiverRunning = false;
    Tekstprintln("Connect timeout - resetting for new scan");
    StartTimeReceiverScan();                                                    // Use helper so TimeReceiverRunning stays consistent
   }

 if (!TRConnected && !scan->isScanning())                                       // Periodic rescan safety net
   {
    if (now - LastTimeReceiverScan > 3000)
      {
       TimeReceiverRunning = false;
       StartTimeReceiverScan();
       Tekstprintlnf("Periodic rescan initiated (%.1f s since last scan)", (now - LastTimeReceiverScan) / 1000.0);
      }
   }
}