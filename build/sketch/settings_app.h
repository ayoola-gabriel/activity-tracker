#line 1 "C:\\Users\\Ayoola\\Documents\\Newness Files\\projects\\Activity Tracker\\activity_tracker_esp32_code\\settings_app.h"
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>antitheft</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        :root {
            --primary: #312651;
            --secondary: #444262;
            --tertiary: #FF7754;

            --gray: #6b6b85;
            --gray2: #C1C0C8;

            --white: #F3F4F8;
            --lightWhite: #FAFAFC;

            --xxxSmall: 0.8px;
            --xxSmall: 2px;
            --xSmall: 10px;
            --small: 12px;
            --medium: 14px;
            --large: 20px;
            --xLarge: 24px;
            --xxLarge: 32px;
        }

        * {
            padding: 0px;
            margin: 0px;
            box-sizing: border-box;
        }

        body {
            background-color: #f8f8f8;
            display: flex;
            justify-content: center;
            align-items: center;
        }


        .main-container {
            /* margin: auto; */
            padding: 0px;
            max-width: 400px;
        }

        header {
            padding: 10px;
            margin-bottom: var(--large);
            align-items: center;
        }


        .infoUI {
            margin-bottom: 20px;
            box-shadow: 0px 0px 5px 1px rgb(129, 129, 129);
        }

        .conState {
            margin-bottom: var(--medium);
            font-weight: bold;
            color: var(--gray);
        }

        .pwelcome {
            font-size: var(--xxLarge);
            /* margin-bottom: var(--large); */
            color: var(--primary)
        }

        .pstate,
        .pconnected,
        .connectButton {
            font-size: var(--medium);
        }

        .pconnected {
            color: var(--secondary);
            font-weight: bold;
        }

        .connectButton {
            background-color: var(--tertiary);
            border: none;
            color: var(--lightWhite);
            width: 100px;
            border-radius: var(--large);
            font-weight: bold;
        }

        .settings {
            width: 100%;
            background-color: #e9e9e9;
            border-radius: 5px 5px 0px 0px;
        }

        .carrierSIM,
        .namecon {
            padding: var(--medium)
        }

        select,
        input {
            width: 100%;
            font-size: medium;
            padding: 10px;
            border-radius: 5px;
            border: 1px solid var(--gray);
            outline: none;
            letter-spacing: 1.5px;

        }

        .carrierSIM select {
            color: var(--primary);
        }

        label {
            color: var(--gray);
            font-weight: bold;
            margin-bottom: var(--small);
            width: 80%;
        }

        .btn-container {
            margin: 20px;
        }

        .btn {
            width: 100px;
            border-radius: var(--large);
            border: none;
            height: 40px;
            font-weight: bold;
            color: var(--lightWhite);
            transition: background-color 0.2s ease-in-out;
        }

        .syncBtn {
            background-color:var(--secondary)
        }

        .savebtn {
            background-color: green;
        }

        .cancelbtn {
            background-color: var(--tertiary)
        }

        .cancelbtn:hover {
            background-color: #fd5a32;
        }

        .savebtn:hover {
            background-color: rgb(3, 155, 3);
        }

        .syncBtn:hover {
            background-color: #312651;
        }

        footer {
            display: flex;
            margin-top: var(--xxLarge);
            align-items: center;
            justify-content: center;
        }

        footer p {
            color: var(--gray);
        }


        .even {
            justify-content: space-evenly;
        }

        .infoUI {
            padding: 10px;
        }



        p,
        label,
        button {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }

        label {
            /* font-size: medium; */
            display: block;
        }

        .dflex {
            display: flex;
        }

        .jcol {
            flex-direction: column;
            -ms-flex-direction: column;
        }

        .jcol-reverse {
            flex-direction: column-reverse;
            -ms-flex-direction: column-reverse;
        }

        .wrap {
            flex-wrap: wrap;
            -ms-flex-wrap: wrap;
        }

        .nowrap {
            flex-wrap: nowrap;
            -ms-flex-wrap: nowrap;
        }

        .jrow {
            flex-direction: row;
            -ms-flex-direction: row;
        }

        .sb {
            justify-content: space-between;
        }

        .end {
            align-items: flex-end;
            -ms-flex-item-align: flex-end;
        }

        .center {
            align-items: center;
            -ms-flex-item-align: center;
        }

        .jcenter {
            justify-content: center;
        }

        .hidden {
            display: none;
        }
    </style>

</head>

<body class="center">
    <div class="main-container dflex sb jcol">
        <header class="dflex jrow sb center">
            <p class="pwelcome">Settings</p>
            <button class="btn syncBtn" id="syncBtn">Sync Time</button>
        </header>

        <section id="connectionsUI" class="infoUI dflex jcol">
            <div class="carrierSIM dflex sb jcol">
                <label for="carr">Select the carrier network of the SIM card inserted into the device</label>
                <select id="carr" aria-placeholder="Select Carrier Network">
                    <option>MTN</option>
                    <option>GLO</option>
                    <option>AIRTEL</option>
                    <option>ETISALAT</option>
                    <option>9MOBILE</option>
                </select>
            </div>

            <hr>

            <div class="namecon">
                <label for="sname">Please enter the number for theft alert SMS</label>
                <input id="sname" placeholder="xxxxxxxxxxx" type="tel">
            </div>

            <hr>

            <div class="btn-container dflex jrow even">
                <button class="btn cancelbtn" id="cancelBtn">Close</button>
                <button class="btn savebtn" id="saveBtn">Save</button>
            </div>

            <footer>
                <p>Newness Electronics, <span>&copy;</span>2024</p>
            </footer>
        </section>


    </div>

    <script>
        window.addEventListener("DOMContentLoaded", () => {

            async function getData(path) {
                const response = await fetch(`${path}`);
                const textData = await response.text();
                return textData;
            }

            syncBtn.addEventListener("click", () => {
                let d = new Date;
                let path = `${window.origin}/time?h=${d.getHours()}&m=${d.getMinutes()}&s=${d.getSeconds()}&dat=${d.getDate()}&mon=${d.getMonth()}&yr=${d.getFullYear()}`
                console.log(path)
                getData(path).then((r)=>{
                r!="Saved"?alert("Failed"):alert("Success");
                });
            })
                           
            saveBtn.addEventListener("click", () => {
                console.log(window.origin)
                let path = `${window.origin}/safeadmin?sos1=${sname.value}&net=${carr.value}`
                getData(path).then((r)=>{
                r!="Saved"?alert("Failed"):alert("Success");
                });
                
                })

            cancelBtn.addEventListener("click", () => {
                let path = `${window.origin}/close`
                getData(path).then((r)=>{
                r!="Done"?alert("Failed"):alert("Connection Closed! Kindly exit the page");
                });
                window.close();
            })
        });
    </script>
</body>
</html>
)rawliteral";