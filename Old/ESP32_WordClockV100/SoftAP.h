const char SoftAP_html[]  = 
R"rawliteral(
<!DOCTYPE html><html><head>
  <title>Enter WIFI SSID & password</title>
  <meta name="google" content="notranslate" />
  <meta name="viewport" content="width=device-width, initial-scale=1">  
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f9;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }
    .container {
      background-color: #fff;
      padding: 20px;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      border-radius: 10px;
      text-align: center;
      width: 300px;
    }
    h2 {
      color: #333;
    }
    label {
      display: block;
      margin: 10px 0 5px;
      color: #555;
    }
    input[type="text"],
    input[type="password"] {
      width: 100%;
      padding: 10px;
      margin: 5px 0 20px;
      border: 1px solid #ddd;
      border-radius: 5px;
      box-sizing: border-box;
    }
    input[type="submit"] {
      background-color: #4CAF50;
      color: white;
      padding: 10px 20px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
    }
    input[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Clock WiFi Configuration</h2>
    Enter SSID and password of your WiFI router.
    <form action="/" method="POST">
      <label for="ssid">SSID:</label>
      <input type="text" id="ssid" name="ssid" required>
      <label for="pass">Password:</label>
      <input type="password" id="pass" name="pass" required>
      <input type="submit" value="Submit">
    </form>
  </div>
</body>
</html>
)rawliteral";