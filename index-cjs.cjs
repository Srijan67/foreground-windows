const path = require('path')
const {EventEmitter} = require('events');
const {promisify} = require('util')
const { spawn, execFile } = require('child_process');
const foregroundListener = new EventEmitter();
const execAsync = promisify(execFile)
let child = null
if(process.platform !== 'win32'){
    console.error("This platform is not supported right now by the package!"); 
    process.exit(1)
}
const exefile= path.join(__dirname, "lib", "foreground.exe")
async function getForegroundWindow() {
    try {
        const {stdout, stderr} = await execAsync(exefile); 
        if(stdout && stdout.length > 0){
            const output = stdout.toString(); 
            const outputSplit = output.split('\r\n')
            let processName = outputSplit[2].split("Process Name: ")[1]
            let processId = outputSplit[1].split('Process ID: ')[1]
            let winTitle = outputSplit[0].split('Foreground Window Title: ')[1]
            let obj = {
                processId: processId,
                processName: processName,
                windowTitle: winTitle
            }
            return obj;
        }else if(stderr){
            return stderr.toString()
        }
    } catch (error) {
        console.log('Error: ', error)
        return error
        
    }
  
}
function startListening() { 
    if (child) { 
        console.log('Listener is already running.'); 
        return; 
    } 
    console.log('Starting foreground listener...'); 
    child = spawn(exefile, ['event']); 
    child.stdout.on('data', (data) => { 
        const output = data.toString(); 
        const outputSplit = output.split('\r\n')
        if(outputSplit.length > 2){

            let processName = outputSplit[2].split("Process Name: ")[1]
            let processId = outputSplit[1].split('Process ID: ')[1]
            let winTitle = outputSplit[0].split('Foreground Window Title: ')[1]
            let obj = {
                processId: processId,
                processName: processName,
                windowTitle: winTitle
            }
            
            foregroundListener.emit('change', obj); 
        }else {
            foregroundListener.emit('change', output); 

        }
    }); 
    child.stderr.on('data', (data) => { 
        const output = data.toString(); 
        console.error(`stderr: ${output}`); 
    }); 
    child.on('exit', (code) => { 
        console.log(`Foreground listener exited with code ${code}`); 
        child = null; 
    });
}
function startDetailListening() { 
    if (child) { 
        console.log('Listener is already running.'); 
        return; 
    } 
    console.log('Starting foreground listener...'); 
    child = spawn(exefile, ['detail-event']); 
    child.stdout.on('data', (data) => { 
        const output = data.toString(); 
        const outputSplit = output.split('\r\n')
        if(outputSplit.length > 2){

            let processName = outputSplit[2].split("Process Name: ")[1]
            let processId = outputSplit[1].split('Process ID: ')[1]
            let winTitle = outputSplit[0].split('Foreground Window Title: ')[1]
            let obj = {
                processId: processId,
                processName: processName,
                windowTitle: winTitle
            }
            
            foregroundListener.emit('change', obj); 
        }else {
            foregroundListener.emit('change', output); 

        }
    }); 
    child.stderr.on('data', (data) => { 
        const output = data.toString(); 
        console.error(`stderr: ${output}`); 
    }); 
    child.on('exit', (code) => { 
        console.log(`Foreground listener exited with code ${code}`); 
        child = null; 
    });
}
function stopListening() { 
    if (child) { 
        child.kill(); 
        child = null; 
        console.log('Stopped foreground listener.'); 
    } else { 
        console.log('Listener is not running.'); 
    }
}
const foreground = {
    startListening, startDetailListening, stopListening, on: (event, listener) => foregroundListener.on(event, listener), getForegroundWindow
}
module.exports = foreground;
