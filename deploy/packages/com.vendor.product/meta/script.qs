function Component()
{
    // constructor
    installer.installationFinished.connect(this, Component.prototype.installVCRedist);
}

Component.prototype.installVCRedist = function()
{
    var registryVC2019x86 = installer.execute("reg", new Array("QUERY", "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x86", "/v", "Installed"))[0];
    if (registryVC2019x86)
    {
        QMessageBox.information("vcRedist.install", "Install VS Redistributables", "The application requires Visual Studio 2015-2019 Redistributables. Please follow the steps to install it now.", QMessageBox.OK);
        var dir = installer.value("TargetDir");
        installer.execute(dir + "/vc_redist.x86.exe", "/norestart", "/passive");
    }
}

Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
    } catch (e) {
        console.log(e);
    }
	
    if (systemInfo.productType === "windows") {
            component.addOperation("CreateShortcut", "@TargetDir@/webphoneID.exe", "@StartMenuDir@/webphoneID.lnk",
        "workingDirectory=@TargetDir@");
            component.addOperation("CreateShortcut", "@TargetDir@/webphoneID.exe", "@HomeDir@/Desktop/webphoneID.lnk");
    }
}