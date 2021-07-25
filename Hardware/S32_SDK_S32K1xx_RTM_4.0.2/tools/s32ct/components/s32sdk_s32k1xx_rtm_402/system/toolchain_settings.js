/*
 * Copyright 2018 NXP
 * To be used with UCT Tools. See Software License Agreement of UCT Tools.
 */

scriptApi.requireScript("codegeneratorUtils.js");

function coreUsed(functionalGroups, coreId) {
    for (functionalGroup in functionalGroups) {
        if (functionalGroups[functionalGroup].getCore() == coreId) {
            return true;
        }
    }
    return false;
}

function main() {
    var profile = scriptApi.getProfile();
    var functionalGroups = profile.getFunctionalGroups();
    var coresList = JSON.parse(profile.getMcuInfo().getCoresList());

    /* coresList is an array containing list of coreIds of each MCU cores, e. g.: ['core0', 'core1'] */
    var coresIds = Object.keys(coresList);

    for (var core = 0; core < coresIds.length; core++) {
        if (coreUsed(functionalGroups, coresIds[core])) {
            updateToolchainSettingsPerComp(profile, functionalGroups, coresIds[core]);
            addComponetsToProject(profile, functionalGroups, coresIds[core]);
        }
    }
  }

// add componets to project
function addComponetsToProject(profile, functionalGroups, coreId){
    for (var functionalGroupsInd in functionalGroups) {
        if (functionalGroups[functionalGroupsInd].getCore() == coreId) {
            var fnPrefix = functionalGroups[functionalGroupsInd].getIdPrefix();
            var componentInstancesConfigs = functionalGroups[functionalGroupsInd].getComponentInstances();
            var isComponent = componentInstancesConfigs.length > 0;
            if (isComponent) {
                componentExists = true;
                
				var compIds = [];
                for (var j = 0; j < componentInstancesConfigs.length; j++) {
                    var swComponents = componentInstancesConfigs[j].getComponent().getComponents();
                    for (var swComp in swComponents) {
                        var componentName = swComponents[swComp].getName();
						compIds.push(componentName)
                    }
                }
				profile.addAllComponents(compIds,true,true);
            }
        }
    }
}

// update project settings per functional group per component
function updateToolchainSettingsPerComp(profile, functionalGroups, coreId) {
    var initFunctionsString = "";
    var initFunctionsGrString = "";
    var componentExists = false;
    var componentCfgs = profile.getComponentConfigurations();

    for (var functionalGroupsInd in functionalGroups) {
        if (functionalGroups[functionalGroupsInd].getCore() == coreId) {
            var fnGroupName = functionalGroups[functionalGroupsInd].getName();
            var fnPrefix = functionalGroups[functionalGroupsInd].getIdPrefix();
            var componentInstancesConfigs = functionalGroups[functionalGroupsInd].getComponentInstances();
            var isComponent = componentInstancesConfigs.length > 0;
            if (isComponent) {
                componentExists = true;
                scriptApi.logWarning(componentInstancesConfigs);
                // Iterate through all components and insert generated settings
                for (var j = 0; j < componentInstancesConfigs.length; j++) {
                    //scriptApi.logWarning(componentInstancesConfigs[j]);
                    //scriptApi.logWarning(componentInstancesConfigs[j].getName());
                    loadComponentSettings(componentInstancesConfigs, fnGroupName, j);
                }
            }
        }
    }
}

function isComponentPresent(id, instancesConfigs) {
    for (var j = 0; j < instancesConfigs.length; j++) {
        var instancesConfig = instancesConfigs[j];
        if (instancesConfig.getError() == null) {
            var componentSettings = instancesConfig.getChildren();
            for (var indComponentSetting in componentSettings) {
                var configSet = componentSettings[indComponentSetting];
                if(configSet.getName() == id) {
                    return true;
                }
            }
        }
    }
    return false;
}

function loadComponentSettings(instancesConfigs, fnGroupName, index) {
    var instanceName = instancesConfigs[index].getName();

    if (instancesConfigs[index].getError() == null) {
      var componentSettings = instancesConfigs[index].getChildren();
      
      var isComponentPresentAnonFunction = function(id) { return isComponentPresent(id, instancesConfigs); };

      for (var indComponentSetting in componentSettings) {
          var componentSettingEmitter = componentSettings[indComponentSetting].getCodeEmitter();

          try{
                var configSets = componentSettings[indComponentSetting];
                // Get js file which contains code generation for each component.
                var settingsGenFile = getSettingsValue(configSets, configSets.getName() + ".toolchain_settings_file");
                if(settingsGenFile != null){
                    // Load required script.
                    scriptApi.requireScript(settingsGenFile);
                    // Get code generation class.
                    var className = getSettingsValue(configSets, configSets.getName() + ".toolchain_settings_class");
                    var settingsClass = (Function('return new ' + className))()
                    // Call constructor.
                    scriptApi.logInfo("Class name " + className);
                    settingsClass.constructor.apply(settingsClass, new Array(configSets));
                    // Add component specific info in header files sections e.g. defines, includes.
                    settingsClass.applyToolchainSettings(scriptApi.getProfile(), configSets, isComponentPresentAnonFunction);
                }
            }
            catch(err)
            {
                scriptApi.logInfo(err);
            }
        
      }

    }
    else {
        scriptApi.logWarning("Configuration of the component " + instanceName + " of functional group " + fnGroupName + " is not valid.");
    }
    return;
}

main();