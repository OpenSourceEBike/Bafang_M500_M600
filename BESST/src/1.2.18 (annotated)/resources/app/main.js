const electron = require('electron');
const app = electron.app; // Module to control application life.
const BrowserWindow = electron.BrowserWindow; // Module to create native browser window.
const dialog = require('electron').dialog;
const updater = require('./updater');
const path = require('path');
const url = require('url');

const log = require('electron-log');
const { autoUpdater } = require("electron-updater");

autoUpdater.logger = log;
autoUpdater.logger.transports.file.level = 'debug';
log.info('App starting...');

handleUpdateEvent();

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
var mainWindow = null;

// Quit when all windows are closed.
app.on('window-all-closed', function() {
    // On OS X it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform != 'darwin') {
        app.quit();
    }
});

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
app.on('ready', function() {

    var protocol = electron.protocol;
    protocol.interceptFileProtocol('file', function(req, callback) {
            p = path.normalize(req.url.substr(8));
            p = decodeURI(p);
            if (p.indexOf(__dirname) < 0) {
                p = path.normalize(__dirname + '/' + req.url.substr(10))
            }
            callback({ path: p })
        }, function(error) {
            if (error)
                console.error('Failed to register protocol')
        })
        // 

    // Create the browser window.
    // mainWindow = new BrowserWindow({width: 1024, height: 650, minWidth:1024, minHeight:650);
    // mainWindow = new BrowserWindow({fullscreen:true});
    mainWindow = new BrowserWindow({ 
	width: 1024, 
	height: 650, 
	minWidth: 1024, 
	minHeight: 650,
	webPreferences:{
		backgroundThrottling:false
	}
    });
    mainWindow.maximize();

    // and load the index.html of the app.
    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'index.html'),
        protocol: 'file:',
        slashes: true
    }));
    // mainWindow.loadURL('http://www.baidu.com');

    // Open the DevTools. 打开调试窗口
    mainWindow.webContents.openDevTools();

    // Emitted when the window is closed.
    mainWindow.on('closed', function() {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null;
    });

    autoUpdater.checkForUpdates();
});








function sendStatusToWindow(text) {
    log.info(text);
    // mainWindow.webContents.send('auto-update', text);
}

function handleUpdateEvent() {
    autoUpdater.on('checking-for-update', () => {
        sendStatusToWindow('Checking for update...');
    })
    autoUpdater.on('update-available', (info) => {
        sendStatusToWindow('Update available.');
    })
    autoUpdater.on('update-not-available', (info) => {
        sendStatusToWindow('Update not available.');
    })
    autoUpdater.on('error', (err) => {
        sendStatusToWindow('Error in auto-updater.');
    })
    autoUpdater.on('download-progress', (progressObj) => {
        let message = "Download speed: " + progressObj.bytesPerSecond;
        message = message + ' - Downloaded ' + progressObj.percent + '%';
        message = message + ' (' + progressObj.transferred + "/" + progressObj.total + ')';
        sendStatusToWindow(message);
    })
    autoUpdater.on('update-downloaded', (info) => {
        sendStatusToWindow('Update downloaded; will install in 3 seconds');
        dialog.showMessageBox({
            title: 'Update',
            message: 'A new version available and could be updated in one minute.' ,
            buttons: ['Update after closing BESST ', 'Update now'],
        }, function(index) {
            if (index == 1) {
                autoUpdater.quitAndInstall();
            }
            sendStatusToWindow(index)
        });
        // setTimeout(function() {
        //     autoUpdater.quitAndInstall();
        // }, 3000);
    });
}