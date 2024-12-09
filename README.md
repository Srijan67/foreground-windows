# Foreground Windows (A great alternative to find active-windows using nodejs)

Nodejs package to get active/foreground window. This will generally be useful to all our Electronjs folks.

## Pros

1. This package also gives you event so that you do not need to hit same function again and again in a cronjob to monitor accurate windows at that time.
2. Supports for cjs(require) and mjs(import).

## Foreground Event

To hook and listen to event, there are two functions startListening() and startDetailListening() functions. From the name its pretty clear that the former one is used for listening the app process changes while the latter is used to listen the tab changes too along with app process changes which means now you can listen to changes in windows title as well so that chrome.exe tab switching doesn't get missed out.

Example:

    // const foreground = require('foreground-windows');
    import foreground from 'foreground-windows';
    foreground.startListening(); // normal process listening

    // foreground.startDetailListening(); //  to listen to changes in windows title
    foreground.on('change', (data) => {
        console.log(data)
    })

    foreground.stopListening(); // use this function when you want to stop the foreground listening event.

## Foreground (Async/Await)

foreground.getForegroundWindow() function is a asynchronous function which gives currently foreground windows details.
Example:

    // const foreground = require('foreground-windows');
    import  foreground  from  'foreground-windows';
    const  currentForegroundWin  =  async () => {
    const  data  =  await foreground.getForegroundWindow()
    console.log('Foreground Window detail:  ',data)
    }

    currentForegroundWin();

## Output

Output comes in javascript object. Example:

    {
      processId: '20220',
      processName: 'Code.exe',
      windowTitle: 'index.js - foreground - Visual Studio Code'
    }

## Features

Since you might have seen in output it gives application process ID, process Name, and windows title in javascript object format.
