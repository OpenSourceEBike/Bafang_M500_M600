const autoUpdater = require('electron').autoUpdater;
const dialog = require('electron').dialog;

function checkUpdate() {

}

var updater = {
    checkUpdate: checkUpdate
};

module.exports = updater;