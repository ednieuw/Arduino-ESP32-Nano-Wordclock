const char logData[]  = 
R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Log Viewer</title>
<style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body { font-family: 'Courier New', monospace; background: #1e1e1e; color: #d4d4d4; padding: 20px; }
    .container { max-width: 1200px; margin: 0 auto; }
    header { background: #2d2d30; padding: 20px; border-radius: 8px; margin-bottom: 20px; box-shadow: 0 2px 8px rgba(0,0,0,0.3); }
    h1 { color: #4ec9b0; font-size: 24px; margin-bottom: 10px; }
    .controls { display: flex; gap: 10px; flex-wrap: wrap; align-items: center; }
    button { background: #0e639c; color: white; border: none; padding: 10px 20px; border-radius: 4px; cursor: pointer; font-size: 14px; transition: background 0.3s; }
    button:hover { background: #1177bb; }
    button:active { background: #0d5a8f; }
    .auto-refresh { display: flex; align-items: center; gap: 8px; }
    input[type=checkbox] { width: 18px; height: 18px; cursor: pointer; }
    .status { margin-left: auto; padding: 8px 12px; background: #3c3c3c; border-radius: 4px; font-size: 12px; }
    .status.connected { color: #4ec9b0; }
    .status.error { color: #f48771; }
    .log-container { background: #1e1e1e; border: 1px solid #3c3c3c; border-radius: 8px; padding: 20px; height: calc(100vh - 200px); overflow-y: auto; box-shadow: 0 2px 8px rgba(0,0,0,0.3); }
    .log-line { padding: 2px 0; border-bottom: 1px solid #2d2d30; font-size: 13px; line-height: 0.8; display: flex; }
    .log-line:hover { background: #2d2d30; }
    .log-line:last-child { border-bottom: none; }
    .line-number { display: inline-block; width: 50px; color: #858585; user-select: none; flex-shrink: 0; }
    .log-text { color: #d4d4d4; flex: 1; word-wrap: break-word; overflow-wrap: break-word; }
    .empty-state { text-align: center; padding: 60px 20px; color: #858585; }
    ::-webkit-scrollbar { width: 12px; }
    ::-webkit-scrollbar-track { background: #1e1e1e; }
    ::-webkit-scrollbar-thumb { background: #3c3c3c; border-radius: 6px; }
    ::-webkit-scrollbar-thumb:hover { background: #4c4c4c; }
</style>

</head>
<body>
    <div class="container">
        <header>
            <div class="controls">
                 <div class="auto-refresh">
                    <input type="checkbox" id="autoRefresh" onchange="toggleAutoRefresh()" checked>
                    <label for="autoRefresh">Auto🔄</label>
                    <span class="status" id="status">Ready</span>
                </div>
            </div>
        </header>
        
        <div class="log-container" id="logContainer">
            <div class="empty-state">
                <p>No log data loaded yet.</p>
                <p style="margin-top: 10px; font-size: 12px;">Click "Refresh" to fetch the latest Tekstprint output.</p>
            </div>
        </div>
    </div>

    <script>
        let autoRefreshInterval = null;
        let lastLogContent = '';  // Track last content to avoid unnecessary updates
        
        // Fetch log from ESP32
        async function fetchLog() {
            const statusEl = document.getElementById('status');
//            statusEl.textContent = 'Loading...';
//            statusEl.className = 'status';
            
            try {
                // Replace with your ESP32 endpoint that returns the log data
                // This should return an array of log lines or plain text
                const response = await fetch('/tekstprint');
                
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                
                const data = await response.text();
                
                // Only update display if content actually changed
                if (data !== lastLogContent) {
                    displayLog(data);
                    lastLogContent = data;
                }
                
                statusEl.textContent = `🔄 ${new Date().toLocaleTimeString('nl-NL', {
                hour: '2-digit',
                minute: '2-digit',
                second: '2-digit'
                })}`;

                statusEl.className = 'status connected';
            } catch (error) {
                console.error('Error fetching log:', error);
                statusEl.textContent = `Error: ${error.message}`;
                statusEl.className = 'status error';
            }
        }
        
        // Display log lines
        function displayLog(data) {
            const container = document.getElementById('logContainer');
            
            // Split data into lines - display ALL lines from the buffer
            const lines = data.split('\n').filter(line => line !== '');
            
            if (lines.length === 0) {
                // Don't clear existing content, just update if it's the initial empty state
                if (container.querySelector('.empty-state')) {
                    container.innerHTML = '<div class="empty-state"><p>No log entries found.</p></div>';
                }
                return;
            }
            
            // Remember scroll position
            const wasAtBottom = (container.scrollHeight - container.scrollTop - container.clientHeight) < 50;
            
            // Build HTML for all lines   //line number  after line html +=       <span class="line-number">${lineNum}</span>                 
            let html = '';
            lines.forEach((line, index) => {
                const lineNum = index + 1;
                html += `<div class="log-line">
 
                <span class="log-text" style="white-space: pre">${escapeHtml(line)}</span>
                </div>`;
            });
           
            container.innerHTML = html;
            
            // Only auto-scroll if user was already at bottom
            if (wasAtBottom) {
                container.scrollTop = container.scrollHeight;
            }
        }
                
        // Toggle auto-refresh
        function toggleAutoRefresh() {
            const checkbox = document.getElementById('autoRefresh');
            
            if (checkbox.checked) {
                fetchLog(); // Fetch immediately
                autoRefreshInterval = setInterval(fetchLog, 1000); // Then every 1 seconds
            } else {
                if (autoRefreshInterval) {
                    clearInterval(autoRefreshInterval);
                    autoRefreshInterval = null;
                }
            }
        }
        
        // Escape HTML to prevent XSS
        function escapeHtml(text) {
            const div = document.createElement('div');
            div.textContent = text;
            return div.innerHTML;
        }
        
        // Initial load – ensure checkbox property is TRUE and start auto refresh without manual flush
    window.addEventListener("load", () => {
    const cb = document.getElementById("autoRefresh");
    cb.checked = true;
    toggleAutoRefresh();
});
    </script>
</body>
</html>
)rawliteral";