#ifndef INDEX_H
#define INDEX_H

const char PAGE_MAIN[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<style>
  :root { --bg: #0f172a; --card: #1e293b; --primary: #3b82f6; --secondary: #10b981; --danger: #ef4444; --text: #f8fafc; }
  body { font-family: 'Segoe UI', sans-serif; background: var(--bg); color: var(--text); margin: 0; padding: 20px; text-align:center; }
  .container { max-width: 400px; margin: auto; }
  .card { background: var(--card); padding: 20px; border-radius: 15px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); margin-bottom: 15px; text-align: left; }
  .label { font-size: 0.8rem; text-transform: uppercase; color: #94a3b8; margin-bottom: 8px; display: block; }
  .btn { width: 100%; padding: 12px; border-radius: 8px; border: none; font-weight: bold; cursor: pointer; transition: 0.3s; margin-top: 10px; }
  .btn-primary { background: var(--primary); color: white; }
  .btn-sec { background: var(--secondary); color: white; }
  .btn-off { background: var(--danger); color: white; }
  input { width: 100%; background: #334155; border: 1px solid #475569; color: white; padding: 10px; border-radius: 5px; box-sizing: border-box; margin-bottom: 10px; }
  .val-display { float: right; color: var(--primary); font-weight: bold; display: flex; align-items: center; }
  
  /* SIGNAL BARS CSS */
  .rssi-container { display: inline-flex; align-items: flex-end; gap: 2px; height: 20px; margin-left: 10px; vertical-align: middle; }
  .bar { width: 4px; background: #475569; border-radius: 1px; transition: 0.3s; }
  .bar.active { background: var(--secondary); }
  .b1 { height: 30%; } .b2 { height: 50%; } .b3 { height: 75%; } .b4 { height: 100%; }
  #no-bt { color: var(--danger); font-size: 1.2rem; display: none; }

  .spinner { width: 12px; height: 12px; border: 2px solid rgba(255,255,255,0.3); border-radius: 50%; border-top-color: var(--primary); animation: spin 0.8s linear infinite; display: none; margin-left: 8px; }
  @keyframes spin { to { transform: rotate(360deg); } }
</style>
</head>
<body>
  <div class='container'>
    <h2 style="display:flex; justify-content:center; align-items:center; gap:10px;">
      🖱️ Jiggler Pro 
      <div id="no-bt">❌</div>
      <div class="rssi-container" id="rssiIcon">
        <div class="bar b1"></div><div class="bar b2"></div>
        <div class="bar b3"></div><div class="bar b4"></div>
      </div>
    </h2>

    <div class='card'>
      <span class='label'>Status</span>
      <div id='statusTxt' style='font-size:1.2rem; font-weight:bold;'>Syncing...</div>
      <div id='timerDisplay' style='color:var(--primary); margin-top:5px; display:none;'></div>
      <button id='mainBtn' class='btn' onclick='toggleJiggler()'>...</button>
    </div>

    <div class='card'>
      <span class='label'>Timer (min)</span>
      <input type='number' id='timerInput' placeholder='Minutes' min='1'>
      <button onclick='setTimer()' class='btn btn-primary'>START TIMER</button>
    </div>

    <div class='card'>
    <div class='card'>
      <span class='label'>Control Settings</span>
      <span class='label'>Distance <span class='val-display'><span id='distVal'>-</span><div id='s_dist' class='spinner'></div></span></span>
      <input type='range' id='distSlider' min='1' max='50' oninput='updateLabel("distVal", this.value + "px")' onchange='sendData("/setMove?px=", this.value, "s_dist")'>
      <br><br>
      <span class='label'>Interval <span class='val-display'><span id='intVal'>-</span><div id='s_int' class='spinner'></div></span></span>
      <input type='range' id='intSlider' min='1' max='60' oninput='updateLabel("intVal", this.value + "s")' onchange='sendData("/setInterval?sec=", this.value, "s_int")'>
      <br><br>
      <span class='label'>Brightness <span class='val-display'><span id='brightVal'>-</span><div id='s_bright' class='spinner'></div></span></span>
      <input type='range' id='brightSlider' min='0' max='255' oninput='updateLabel("brightVal", this.value)' onchange='sendData("/setBrightness?val=", this.value, "s_bright")'>
    </div>

    <div class='card' style="border-top: 2px solid var(--primary);">
      <span class='label'>Home WiFi Settings (Station)</span>
      <input type='text' id='staSSIDInput' placeholder='Home WiFi Name'>
      <input type='password' id='staPassInput' placeholder='Home Password'>
      
      <span class='label'>Local AP Settings (Direct Connect)</span>
      <input type='text' id='apSSIDInput' placeholder='AP Name'>
      <input type='password' id='apPassInput' placeholder='AP Password'>

      <span class='label'>Bluetooth Name</span>
      <input type='text' id='btInput'>
      <button onclick='saveConfig()' class='btn btn-primary' style="background: #6366f1;">UPDATE & RESTART</button>
    </div>
  </div>

  <script>
    let active = false; let timerEnd = 0;

    function updateLabel(id, val) { document.getElementById(id).innerText = val; }
    function updateValue(id, val) { document.getElementById(id).value = val; }

    function updateRSSI(dbm) {
      const bars = document.querySelectorAll('.bar');
      const noBt = document.getElementById('no-bt');
      const icon = document.getElementById('rssiIcon');
      if (dbm === 0) { noBt.style.display = 'block'; icon.style.display = 'none'; return; }
      noBt.style.display = 'none'; icon.style.display = 'inline-flex';
      let level = (dbm > -60) ? 4 : (dbm > -75) ? 3 : (dbm > -90) ? 2 : 1;
      bars.forEach((bar, i) => { bar.classList.toggle('active', i < level); });
    }

    function sendData(path, val, spinnerId) {
      document.getElementById(spinnerId).style.display = 'inline-block';
      fetch(path + val).then(() => { setTimeout(() => { document.getElementById(spinnerId).style.display = 'none'; syncStatus();}, 300); });
      
    }

    function saveConfig() {
      const staSSID = document.getElementById('staSSIDInput').value;
      const staPass = document.getElementById('staPassInput').value;
      const apSSID = document.getElementById('apSSIDInput').value;
      const apPass = document.getElementById('apPassInput').value;
      const bt = document.getElementById('btInput').value;
      
      fetch(`/updateConfig?staSSID=${staSSID}&staPass=${staPass}&apSSID=${apSSID}&apPass=${apPass}&bt=${bt}`)
        .then(() => alert("Restarting..."));
    }

    function toggleJiggler() { active = !active; fetch('/toggle').then(() => updateUI()); }
    
    function setTimer() { 
      const min = document.getElementById('timerInput').value;
      if(min > 0) { fetch('/setTimer?min=' + min).then(() => { active = true; timerEnd = Date.now() + min * 60000; updateUI(); }); } 
    }

    function updateUI() {
      const btn = document.getElementById('mainBtn');
      const st = document.getElementById('statusTxt');
      st.innerText = active ? 'ACTIVE' : 'IDLE';
      st.style.color = active ? '#10b981' : '#ef4444';
      btn.innerText = active ? 'STOP' : 'START MANUAL';
      btn.className = 'btn ' + (active ? 'btn-off' : 'btn-sec');
    }

    function syncStatus() {
      fetch('/status').then(r => r.json()).then(data => {
        active = data.active;
        updateRSSI(data.rssi);

        updateValue('distSlider',  data.move);
        updateLabel('distVal', data.move + 'px');
        updateValue('intSlider',  data.interval);
        updateLabel('intVal', data.interval + 's');
        updateValue('brightSlider',  data.bright);
        updateLabel('brightVal', data.bright);

        updateValue('staSSIDInput',  data.staSSID);
        updateValue('staPassInput',  data.staPass);
        updateValue('apSSIDInput',  data.apSSID);
        updateValue('apPassInput',  data.apPass);
        updateValue('btInput',  data.btname);
        

        if(data.timerRemaining > 0) { timerEnd = Date.now() + data.timerRemaining; }
        updateUI();
      });
    }

    function keepSync() {
      fetch('/status').then(r => r.json()).then(data => {
        active = data.active;
        updateRSSI(data.rssi);
        updateLabel('brightVal', data.bright);
        if(data.timerRemaining > 0) { timerEnd = Date.now() + data.timerRemaining; }
        updateUI();
      });

    }

    // Update timer display every second
    setInterval(() => {
      if(active && timerEnd > Date.now()) {
        let diff = Math.round((timerEnd - Date.now()) / 1000);
        let m = Math.floor(diff/60); let s = diff%60;
        document.getElementById('timerDisplay').style.display = 'block';
        document.getElementById('timerDisplay').innerText = 'Remaining: ' + m + 'm ' + s + 's';
      } else {
        document.getElementById('timerDisplay').style.display = 'none';
      }
    }, 1000);

    // Sync RSSI and Settings every 3 seconds
    setInterval(keepSync, 3000);
    window.onload = syncStatus;
  </script>
</body></html>
)=====";

#endif