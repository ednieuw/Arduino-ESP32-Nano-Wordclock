const char OTA_html[] = 
R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Word clock Update</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <style>
      body { font-family: Arial; text-align: center; background-color: #f2f2f2; margin-top: 50px; }
      h2 { color: #333; }
      form {
        background: black;  padding: 20px;  border-radius: 10px;  display: inline-block;
        box-shadow: 0 0 10px rgba(0,0,0,0.1);  max-width: 90vw;  width: 100%;  box-sizing: border-box;
      }
  input[type=file] {
		padding: 10px; width: 90vw; font-size: 16px;box-sizing: border-box;	background-color: #000000;
	}
      input[type=submit] {
        background-color: #4CAF50; color: white; border: none; padding: 12px 24px;
        font-size: 16px; cursor: pointer; border-radius: 5px;
      }
      #progressBar {
		width: 90%;background-color: #ddd;	margin-top: 20px;
		height: 30px;	border-radius: 5px;	overflow: hidden;
	}
      #bar {
        height: 100%; width: 0%; background-color: #4CAF50; text-align: center;
        line-height: 30px; color: white; transition: width 0.2s;
      }
    </style>
  </head>
  <body style="color: #FFFFFF; background-color: #000000">
<h2 style="color: #FFBB00;"> Upload Word clock Firmware (.bin)</h2>

    <form method="POST" action="/update" enctype="multipart/form-data" id="upload_form" style="width: 424px">
      <input type="file" name="update" accept=".bin"><br><br>
      <input type="submit" value="Upload">
    </form>
    <div id="progressBar"><div id="bar">0%<br></div></div>
    
    <p>&nbsp;</p>
    
<h3 style="color: #FFCC00;"> 
Wait 15 seconds after upload <br>
and the clock menu will appear.</h3>

    <script>
      const form = document.getElementById('upload_form');
      const bar = document.getElementById('bar');

      form.addEventListener('submit', function(e){
        e.preventDefault();
        const fileInput = form.querySelector('input[name="update"]');
        const file = fileInput.files[0];
        if (!file) return alert("Please select a file");

        const xhr = new XMLHttpRequest();
        xhr.open("POST", "/update", true);

        xhr.upload.onprogress = function(e) {
          if (e.lengthComputable) {
            const percent = Math.round((e.loaded / e.total) * 100);
            bar.style.width = percent + "%";
            bar.innerHTML = percent + "%";
          }
        };

        xhr.onload = function() {
          if (xhr.status === 200) {
            bar.innerHTML = "✅ Update successful. Restarting... in 10 seconds";
            setTimeout(() => location.href = "/", 7000);
          } else {
            bar.innerHTML = "❌ Error!";
          }
        };

        const formData = new FormData();
        formData.append("update", file);
        xhr.send(formData);
      });
    </script>
  </body>
</html>
)rawliteral";


const char index_html_footer[]  = 
R"rawliteral(
</span>
</body></html>
)rawliteral";

