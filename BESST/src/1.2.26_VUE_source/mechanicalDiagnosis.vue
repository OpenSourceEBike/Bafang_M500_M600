<template>
    <div class="mechanical-diagnosis">
        <div class="content">
            <el-button class="report" type="success" @click="createReportSave">
                {{ $t('createReport') }}
            </el-button>
        </div>
        <div class="clear-10"></div>
        <div class="content" >
            <el-form ref="form" class="mechanical-handle">
                <el-form-item>
                    <!--<div>-->
                    <label>{{ $t('defectDes') }}</label>
                    <el-input
                        type="textarea"
                        :rows="3"
                        resize="none"
                        :placeholder="$t('defectLength')"
                        :maxlength="600"
                        v-model="problemDescription"></el-input>
                    <!--</div>-->
                </el-form-item>

                <el-form-item :label="$t('image')">
                    <el-upload
                        :show-file-list="false"
                        :action="upload.url"
                        :on-success="handleSuccess"
                        :file-list="upload.photographList"
                        :data="upload.data">
                        <div>
                            <img src="../../assets/images/diagnosis/icon-upload.png"/>
                            <span class="ml-10" style="color: #FFFFFF">{{ $t('defectDes') }}</span>
                        </div>
                    </el-upload>
                    <el-row v-if="fileList.img.length>0">
                        <div v-for="(item, index) in fileList.img" class="img-holder">
                            <img :src="linkPre + item" class="img-content" @click="handlePreview(linkPre + item)"/>
                            <div class="close-btn" @click="removeImg(index)"></div>
                        </div>
                    </el-row>
                </el-form-item>

                <el-form-item :label="$t('video')">
                    <el-upload
                        class="avatar-uploader"
                        :action="upload.url"
                        :show-file-list="false"
                        :on-success="handleAvatarSuccess"
                        :before-upload="beforeAvatarUpload"
                        :on-progress="uploadVideoProcess"
                        :data="upload.data"
                        accept="video/*">
                        <div>
                            <img src="../../assets/images/diagnosis/icon-upload.png"/>
                            <span class="ml-10" style="color: #FFFFFF">{{ $t('op.upload') }}</span>
                        </div>
                    </el-upload>
                    <el-row v-if="fileList.video.length>0">
                        <div v-for="(item, index) in fileList.video" class="video-holder">
                            <video :src="linkPre + item" controls="controls" class="videoItem"/>
                            <div class="close-btn" @click="removeVideo(index)"></div>
                        </div>
                    </el-row>
                    <span>{{ $t('videoMaxSize') }}</span>
                </el-form-item>
            </el-form>
        </div>
        <div class="content">
            <el-form class="defect-list">
                <el-form-item v-for="(item, index) in defectOptionList" :key="index">
                    <h3 style="color: #E26829; font-size: 20px;text-indent: 18px;">{{item.model}}</h3>
                    <h3 class="serial-number">
                        <span>{{ $t('cNam.sn') }}:</span>
                        <el-input v-model="SNList[item.type]" class="input input-sn" :maxlength="40"></el-input>
                    </h3>
                    <el-checkbox-group v-model="selectedList[item.type]" class="error-details">
                        <el-checkbox v-for="select in item.data" :key="select.name"
                                    :label="select.label" @change="changeRecord()"></el-checkbox>
                    </el-checkbox-group>
                </el-form-item>
            </el-form>
        </div>
        <div class="content">
            <el-form class="error-list">
                <h3 style="color: #E26829; font-size: 20px;text-indent: 18px;">{{errorCodeOptionList.model}}</h3>
                <el-checkbox-group v-model="errorCodeList" class="error-details" @change="changeCheckBox">
                    <el-checkbox v-for="error in errorCodeOptionList.data" :key="error.label"
                                 :label="error.label"></el-checkbox>
                </el-checkbox-group>
            </el-form>
        </div>

        <!--图片预览图-->
        <el-dialog v-model="preDialogVisible"  custom-class="pre-dialog" top="20vh" size="large">
            <div style="text-align: center; background: #FFFFFF">
                <img class="pre-img" :src="dialogImageUrl" alt="">
            </div>
        </el-dialog>
    </div>
</template>
<script>
    import Core from 'core/core';
    import ZH from 'src/assets/lang/zh';
    import EN from 'src/assets/lang/en';
    import DE from 'src/assets/lang/de';
    import NL from 'src/assets/lang/nl';

    export default {
        name: "diagnosisMechanical",
        props: {
            defectList: Object,
            errorList: Array,
            fileAll: Object,
            reportInfo: Object,
            problemText: String,
        },
        data(){
            return{
                Core: Core,
                videoFlag: false,
                videoUploadPercent: 0,
                showVideoPath: '',
                upload: {
                    url: Core.Const.NET.FILE_UPLOAD_END_POINT,
                    photographList: [],
                    data: {
                        token: Core.Data.getToken(),
                    }
                },
                imageUrl: '',
                defectSelectVisible: false,
                defectOptionList: Core.Const.OptionList,

                errorCodeOptionList:{
                    type: 'errorCode',
                    saveData: 'errorCodeList',
                    model: '报错代码',
                    data: []
                },
                isConnected: false,
                selectedList: {},
                selectedDefectList: [],
                SNList: {
                    hmi: '',
                    controller: '',
                    battery: '',
                    sensor: '',
                    motor: '',
                    bike: '',
                    other: ''
                },
                errorCodeList: [],
                // 扫描输入
                isScanner: false,
                lastInputTime: 0,
                scannerTimeout: '',
                reportData: {},

                linkPre: Core.Const.NET.FILE_URL_PREFIX,

                fileList: {
                    video:[],
                    img:[],
                },
                problemDescription: '',
                dialogImageUrl: '',
                preDialogVisible: false,

            }
        },
        i18n: {
            messages: {
                en: EN.Components.Diagnosis.diagnosis3rd,
                zh: ZH.Components.Diagnosis.diagnosis3rd,
                nl: NL.Components.Diagnosis.diagnosis3rd,
                de: DE.Components.Diagnosis.diagnosis3rd
            }
        },
        mounted() {
//            console.log(this.defectOptionList);
//            console.log(this.errorList);
            console.log(this.reportInfo);

            this.SNList.hmi = this.reportInfo.device.hmi.sn;
            this.SNList.controller = this.reportInfo.device.controller.sn;
            this.SNList.battery = this.reportInfo.device.battery.sn;
            this.SNList.sensor = this.reportInfo.device.pedalSensor.sn;
            this.SNList.motor = this.reportInfo.device.motor.sn;
            this.SNList.bike = this.reportInfo.device.vehicle.sn;

            this.reportData = this.reportInfo;
//            console.log( this.reportData );

            if(this.errorList.length>0){
                this.errorCodeList = this.errorList;
            }
            if(this.defectList.hasOwnProperty('HMIList')){
                this.selectedList = this.defectList;
            }
            if(this.problemText !== ''){
                this.problemDescription = this.problemText;
            }
            if(this.fileAll){
                this.fileList = this.fileAll;
            }

//            this.fileList = {
//                video:['6dbfea85063ae089ace219520d8f1efa1fdbb8ed1faee893bb64a682ab6d5c9f.mp4','6dbfea85063ae089ace219520d8f1efa1fdbb8ed1faee893bb64a682ab6d5c9f.mp4'],
//                img:['dd5de5b7d2401fbdd5f845b96cdd6157b63da21505d9a926071a1fc7bf5642a7.png','dd5de5b7d2401fbdd5f845b96cdd6157b63da21505d9a926071a1fc7bf5642a7.png']
//            }
            this.initDefectOptionList();
            this.initErrorCode();
        },
        methods: {
            changeRecord(){

//                let keys = Object.keys(this.selectedList);
//                console.log(keys,"keys");
//                console.log("keys", this.selectedList);
//                this.selectedDefectList = [{
//                    'hmi':[],
//                    'battery':[],
//                    'bike':[],
//                    'charger':[],
//                    'motor':[],
//                    'other':[],
//                    'sensor':[],
//                }];
//                for(let i=0; i<keys.length; i++){
//                    for(let j=0; j<this.selectedList[keys[i]].length; j++){
//
//                    }
//                }
//                console.log("selectedDefectList", this.selectedDefectList);

            },

            initDefectOptionList() {
                this.errorCodeOptionList['model'] = this.$t("cNam.errorCode");
                this.defectOptionList[0]['model'] = this.$t("cNam.bike");
                this.defectOptionList[1]['model'] = this.$t("cNam.motor");
                this.defectOptionList[2]['model'] = this.$t("cNam.hmi");
                this.defectOptionList[3]['model'] = this.$t("cNam.controller");
                this.defectOptionList[4]['model'] = this.$t("cNam.battery");
                this.defectOptionList[5]['model'] = this.$t("cNam.sensor");
                this.defectOptionList[0]['data'][0].label = this.$t("cNam.bikeLabel_1");
                this.defectOptionList[0]['data'][1].label = this.$t("cNam.bikeLabel_2");
                this.defectOptionList[0]['data'][2].label = this.$t("cNam.bikeLabel_3");
                this.defectOptionList[0]['data'][3].label = this.$t("cNam.bikeLabel_4");
                this.defectOptionList[0]['data'][4].label = this.$t("cNam.bikeLabel_5");
                this.defectOptionList[0]['data'][5].label = this.$t("cNam.bikeLabel_6");
                this.defectOptionList[0]['data'][6].label = this.$t("cNam.bikeLabel_7");
                this.defectOptionList[0]['data'][7].label = this.$t("cNam.bikeLabel_8");
                this.defectOptionList[0]['data'][8].label = this.$t("cNam.bikeLabel_9");
                this.defectOptionList[0]['data'][9].label = this.$t("cNam.bikeLabel_10");
                this.defectOptionList[0]['data'][10].label = this.$t("cNam.bikeLabel_11");
                this.defectOptionList[0]['data'][11].label = this.$t("cNam.bikeLabel_12");
                this.defectOptionList[1]['data'][0].label = this.$t("cNam.motorLabel_1");
                this.defectOptionList[1]['data'][1].label = this.$t("cNam.motorLabel_2");
                this.defectOptionList[1]['data'][2].label = this.$t("cNam.motorLabel_3");
                this.defectOptionList[1]['data'][3].label = this.$t("cNam.motorLabel_4");
                this.defectOptionList[1]['data'][4].label = this.$t("cNam.motorLabel_5");
                this.defectOptionList[1]['data'][5].label = this.$t("cNam.motorLabel_6");
                this.defectOptionList[1]['data'][6].label = this.$t("cNam.motorLabel_7");
                this.defectOptionList[1]['data'][7].label = this.$t("cNam.motorLabel_8");
                this.defectOptionList[1]['data'][8].label = this.$t("cNam.motorLabel_9");
                this.defectOptionList[1]['data'][9].label = this.$t("cNam.motorLabel_10");
                this.defectOptionList[2]['data'][0].label = this.$t("cNam.hmiLabel_1");
                this.defectOptionList[2]['data'][1].label = this.$t("cNam.hmiLabel_2");
                this.defectOptionList[2]['data'][2].label = this.$t("cNam.hmiLabel_3");
                this.defectOptionList[2]['data'][3].label = this.$t("cNam.hmiLabel_4");
                this.defectOptionList[2]['data'][4].label = this.$t("cNam.hmiLabel_5");
                this.defectOptionList[2]['data'][5].label = this.$t("cNam.hmiLabel_6");
                this.defectOptionList[2]['data'][6].label = this.$t("cNam.hmiLabel_7");
                this.defectOptionList[2]['data'][7].label = this.$t("cNam.hmiLabel_8");
                this.defectOptionList[2]['data'][8].label = this.$t("cNam.hmiLabel_9");
                this.defectOptionList[3]['data'][0].label = this.$t("cNam.ctlLabel_1");
                this.defectOptionList[3]['data'][1].label = this.$t("cNam.ctlLabel_2");
                this.defectOptionList[3]['data'][2].label = this.$t("cNam.ctlLabel_3");
                this.defectOptionList[3]['data'][3].label = this.$t("cNam.ctlLabel_4");
                this.defectOptionList[4]['data'][0].label = this.$t("cNam.batLabel_1");
                this.defectOptionList[4]['data'][1].label = this.$t("cNam.batLabel_2");
                this.defectOptionList[4]['data'][2].label = this.$t("cNam.batLabel_3");
                this.defectOptionList[4]['data'][3].label = this.$t("cNam.batLabel_4");
                this.defectOptionList[4]['data'][4].label = this.$t("cNam.batLabel_5");
                this.defectOptionList[4]['data'][5].label = this.$t("cNam.batLabel_6");
                this.defectOptionList[4]['data'][6].label = this.$t("cNam.batLabel_7");
                this.defectOptionList[4]['data'][7].label = this.$t("cNam.batLabel_8");
                this.defectOptionList[4]['data'][8].label = this.$t("cNam.batLabel_9");
                this.defectOptionList[4]['data'][9].label = this.$t("cNam.batLabel_10");
                this.defectOptionList[4]['data'][10].label = this.$t("cNam.batLabel_11");
                this.defectOptionList[5]['data'][0].label = this.$t("cNam.sensor_1");
                this.defectOptionList[5]['data'][1].label = this.$t("cNam.sensor_2");
                this.defectOptionList[5]['data'][2].label = this.$t("cNam.sensor_3");
                this.defectOptionList[5]['data'][3].label = this.$t("cNam.sensor_4");
                this.defectOptionList[5]['data'][4].label = this.$t("cNam.sensor_5");
                this.defectOptionList[5]['data'][5].label = this.$t("cNam.sensor_6");
                this.defectOptionList[5]['data'][6].label = this.$t("cNam.sensor_7");
            },
            initErrorCode() {
                let errObject = Core.Const.TEST_ERROR_CODE;
                let keys = Object.keys(errObject);
                keys.forEach((item)=>{
                    let obj = {value:item,label:item};
                    this.errorCodeOptionList.data.push(obj);
                });
//                console.log(this.errorCodeOptionList.data,"errorCodeOptionList");
            },
            handlePreview(url) {
                this.dialogImageUrl = url;
                this.preDialogVisible = true;
            },
            removeImg(index) {
//                console.log(index);
                this.fileList.img.splice(index, 1);
            },
            removeVideo(index) {
//                console.log(index);
                this.fileList.video.splice(index, 1);
            },
            handleSuccess(file, fileList) {
                if(this.fileList.video.length + this.fileList.img.length>4){
                    this.$message({
                        message: 'Including video and image, maximum 5 files',
                        type: 'warning'
                    });
                    return;
                }
//                console.log(file, "file")
//                code: 0
//                data:
//                    file:
//                        attr_def_id: 0
//                ext: "png"
//                hash: "dd5de5b7d2401fbdd5f845b96cdd6157b63da21505d9a926071a1fc7bf5642a7"
//                id: 1019
//                mime_type: "image/png"
//                name: "HAS been used.png"
//                size: 154381
//                console.log(fileList, 'fileList')
                let targetFile = file.data.file.hash  + '.' +  file.data.file.ext;
//                console.log(targetFile)
                this.fileList.img.push(targetFile);
            },
            handleAvatarSuccess(res, file) {
//                console.log(res);
//                file:
//                    attr_def_id: 0
//                ext: "mp4"
//                hash: "6dbfea85063ae089ace219520d8f1efa1fdbb8ed1faee893bb64a682ab6d5c9f"
//                id: 1002
//                mime_type: "video/mp4"
//                name: "1b14ba4110476085695fb8485dcac6d3.mp4"
//                size: 10133119
//                console.log(file);

                if(this.fileList.video.length + this.fileList.img.length>4){
                    this.$message({
                        message: 'Including video and image, maximum 5 files',
                        type: 'warning'
                    });
                    return;
                }
                if(file.raw.type.indexOf('video')>-1){
//                    this.showVideoPath = URL.createObjectURL(file.raw);
                    let targetFile = file.data.file.hash  + '.' +  file.data.file.ext;
                    this.fileList.video.push(targetFile);
                }
//                console.log(this.fileList);
//                if (this.imgList.length > 0) {
//                    img = JSON.stringify(this.imgList);
//                }
//
//                if (this.fileList.length > 0) {
//                    video = this.fileList[0].response.data.file.hash + '.' + this.fileList[0].response.data.file.ext;
//                }

            },
            beforeAvatarUpload(file) {
//                console.log(file);
                //    const isLt10M = file.size / 1024 / 1024  < 10;
                // if (['video/mp4', 'video/ogg', 'video/flv','video/avi','video/wmv','video/rmvb'].indexOf(file.type) == -1) {
                //     this.$message.error('请上传正确的视频格式');
                //     return false;
                // }
                // if (!isLt10M) {
                //     this.$message.error('上传视频大小不能超过10MB哦!');
                //     return false;
                // }
                const isLt10M = file.size / 1024 / 1024  < 10;
                if (!isLt10M) {
                    this.$message.error(this.$t('videoMaxSize'));
                    return false;
                }
            },

            uploadVideoProcess(event, file, fileList){
                this.videoFlag = true;
                //this.videoUploadPercent = file.percentage.toFixed(0);
                this.videoUploadPercent = file.percentage;
            },
            complete(file, status, xhr) {
                if (status === 'success') {
                    let res = JSON.parse(xhr.response);
                    if (res.code === 0 && res.data.file) {
                        this.fileList.img.push({"img":res.data.file.hash + '.' + res.data.file.ext});
                    }
                }
            },
//            cancelSelectDefect(){
//                this.defectSelectVisible = false;
//                for(var item in this.selectedList){
//                    this.selectedList[item] = [];
//                }
//            },
//            selectItemDefect(){
//                this.defectSelectVisible = false;
//                this.defectOptionList.forEach((item)=>{
//                    for(var selectedItem in this.selectedList){
//                        if(item.saveData == selectedItem){
//                            this.selectedList[selectedItem].forEach((x,y)=>{
//                                item.data.forEach((j)=>{
//                                    if(x == j.label){
//                                        var data = item.type+'-'+j.value;
//                                        this.selectedList[selectedItem].splice(y,1,data);
//                                    }
//                                })
//                            })
//                        }
//                    }
//                })
//                console.log(this.selectedList);
//            },
//            showDefectDialog(){
//                this.defectSelectVisible = true;
//                this.defectOptionList.forEach((item)=>{
//                    for(var selectedItem in this.selectedList){
//                        if(item.saveData == selectedItem){
//                            this.selectedList[selectedItem].forEach((x,y)=>{
//                                item.data.forEach((j)=>{
//                                    var data = item.type+'-'+j.value;
//                                    if(x == data){
//                                        this.selectedList[selectedItem].splice(y,1,j.label);
//                                    }
//                                })
//                            })
//                        }
//                    }
//                })
//            },
//            getVideo(){
//                this.$http.get('http://192.168.0.140:9898/file/fileVoidByHash?token='+Core.Data.getToken()
//                ).then(res=>{
//                    console.log(res);
//                    if(!res.data){
//                        return false;
//                    }
//                    this.showVideoPath = 'http://static.besst.bafang-service.com/besst/file/'+res.data.fileHash+'.mp4'
//                }).catch(err=>{
//                    console.log(err);
//                })
//            },
            createReportSave(){
                this.$emit('skipShow');
                console.log("skipShow");
            },

//            goToSaveReport(){
//
//                let report = {};
//
//                if(this.reportData.result.hmi !== '' && this.reportData.result.controller !==''){
//                    report = this.reportData;
//                }else {
//                    report = {
//                        device: {
//                            vehicle: {
//                                id: '',
//                                sn: this.SNList.bike,
//                                itemName: '',
//                                secretKey: ''
//                            },
//                            hmi: {'sn': this.SNList.HMI},
//                            controller: {'sn': this.SNList.controller},
//                            battery: {'sn': this.SNList.battery},
//                            motor: {'sn': this.SNList.motor},
//                            pedalSensor: {'sn': this.SNList.sensor},
//                            rpmSensor: {}
//                        },
//                        can: undefined,
//                        test: {},
//                        result: [],
//                        existComList: [],
//                        bomList: [],
//                        connectList: [],
//                        testList: [],
//                        allConnectList: [],
//                        allTestList: []
//                    };
//                }
//
//                this.$router.push({
//                    path: 'result',
//                    query: {report: report, defect: this.selectedDefectList, file: JSON.stringify(this.fileList), remark: this.problemDescription}
//                })
//            },
            changeCheckBox() {
//                console.log( this.errorCodeList );
            }
        },
        created(){
            this.defectOptionList.forEach((item)=>{
                this.$set(this.selectedList, item.type, []);
                this.$set(this.SNList, item.type, '');
            });
//            console.log(this.selectedList);
//            console.log(this.SNList);
//            this.getVideo();
//            this.showVideoPath = 'http://static.besst.bafang-service.com/besst/file/6dbfea85063ae089ace219520d8f1efa1fdbb8ed1faee893bb64a682ab6d5c9f.mp4'

        },
        watch: {
            'SNList.HMI': {
                handler: function (newValue, oldValue) {
                    let vm = this;
                    vm.SNList.HMI = newValue.replace(/\s+/g, "");

                    if (newValue === oldValue || newValue === '') {
                        return;
                    }
                    if (typeof oldValue !== 'undefined') {
                        let time = new Date().getTime();
                        vm.isScanner = time - vm.lastInputTime <= 100; // 间隔小于50ms的就认为是扫描枪输入
                        vm.lastInputTime = time;
                    } else {
                        vm.lastInputTime = new Date().getTime();
                    }
                    if (!vm.scannerTimeout) {
                        vm.scannerTimeout = setTimeout(function () {
                            if (vm.isScanner) {
                                let value = vm.SNList.HMI;
                                // 如果序列号中匹配到特定字符，自动显示在对应的输入框中
                                if (value.indexOf('DP') > -1) {
                                    let array = value.split('DP');
                                    if (array.length > 1) {
                                        value = 'DP' + array[array.length - 1];
                                    }
                                    vm.SNList.HMI = value;
                                }
                            }
                            clearTimeout(vm.scannerTimeout);
                            vm.scannerTimeout = undefined;
                        }, 1000)
                    }
                },
                deep: true
            },
            'SNList.controller': {
                handler: function (newValue, oldValue) {
                    this.SNList.controller = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            'SNList.battery': {
                handler: function (newValue, oldValue) {
                    this.SNList.battery = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            'SNList.motor': {
                handler: function (newValue, oldValue) {
                    this.SNList.motor = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
            'SNList.sensor': {
                handler: function (newValue, oldValue) {
                    let vm = this;
                    vm.SNList.sensor = newValue.replace(/\s+/g, "");

                    if (newValue === oldValue || newValue === '') {
                        return;
                    }
                    if (typeof oldValue !== 'undefined') {
                        let time = new Date().getTime();
                        vm.isScanner = time - vm.lastInputTime <= 100; // 间隔小于50ms的就认为是扫描枪输入
                        vm.lastInputTime = time;
                    } else {
                        vm.lastInputTime = new Date().getTime();
                    }
                    if (!vm.scannerTimeout) {
                        vm.scannerTimeout = setTimeout(function () {
                            if (vm.isScanner) {
                                let value = vm.SNList.sensor;
                                // 如果序列号中匹配到特定字符，自动显示在对应的输入框中
                                if (value.indexOf('SR') > -1) {
                                    let array = value.split('SR');
                                    if (array.length > 1) {
                                        value = 'SR' + array[array.length - 1];
                                    }
                                    vm.SNList.sensor = value;
                                }
                            }
                            clearTimeout(vm.scannerTimeout);
                            vm.scannerTimeout = undefined;
                        }, 1000)
                    }
                    console.log(vm.SNList.sensor);

                },
                deep: true
            },
            'SNList.bike': {
                handler: function (newValue, oldValue) {
                    this.SNList.bike = newValue.replace(/\s+/g, "");
                },
                deep: true
            },
        }
    }
</script>
<style lang="scss">

    .mechanical-diagnosis{
        overflow: auto;
        width: 100%;
        height: 100%;
        .content{
            margin-right: 35px;
        }
        .report {
            margin: 10px 0px 0 0;
            padding: 5px 26px;
            align-self: center;
            float: right;
            span{
                font-size: 20px;
            }
        }
        .el-form {
            .auth-title {
                color: #fff;
                padding: 10px 0;
                font-weight: bold;
            }
            .el-checkbox-group {
                line-height: 30px;
                overflow: hidden;
                .el-checkbox + .el-checkbox {
                    margin: 0;
                }
                .el-checkbox__label{
                    display: inline-block;
                    max-width: 300px;
                    text-overflow: ellipsis;
                }
            }
            .img-holder {
                position: relative;
                width: 160px;
                display: inline-block;
                background-color: transparent;
                margin-right: 10px;
                .img-content {
                    width: 160px;
                }
                .close-btn {
                    width: 15px;
                    height: 15px;
                    top: 0;
                    right: 0;
                    position: absolute;
                    z-index: 9;
                    background: url("../../assets/images/icon-close.png");
                    &:hover {
                        background: url("../../assets/images/icon-close-red.png");
                    }
                }
            }
            .video-holder{
                position: relative;
                display:inline-block;
                margin-right:10px;
                .videoItem{
                    width:160px;
                }
                .close-btn {
                    width: 15px;
                    height: 15px;
                    top: 0;
                    right: 0;
                    position: absolute;
                    z-index: 9;
                    background: url("../../assets/images/icon-close.png");
                    &:hover {
                        background: url("../../assets/images/icon-close-red.png");
                    }
                }
            }
        }
        .mechanical-handle{
            .el-form-item{
                margin-left: 32px;
                margin-bottom: 8px;
                .error-code-description{
                    width: 160px!important;
                }
            }
        }
        .defect-list{
            padding: 10px;
            display: flex;
            flex-wrap:wrap;
            .el-form-item{
                flex: 0 0 33%;
                .serial-number{
                    margin-left: 20px;
                    .el-input{
                        width: 200px;
                        margin-left: 10px;
                    }
                    .input-sn:after{
                        top: 10px;
                    }
                }
                .el-checkbox {
                    width: 100%;
                    padding-left: 20px;
                }
            }
        }
        .error-list{
            padding: 10px;
            display: flex;
            flex-wrap:wrap;
            .el-checkbox {
                width: 25%;
                padding-left: 20px;
            }
        }
        .pre-dialog {
            .el-dialog__header {
                display: none;
            }

            .el-dialog__body {
                padding: 0;
                >div{
                    font-size: 0;
                }
            }
            .pre-img {
                min-width: 100px;
                max-width: 500px;
            }
        }
        /*.error-details .el-checkbox{*/
            /*margin-left: 15px!important;*/
        /*}*/
    }

</style>



// WEBPACK FOOTER //
// mechanicalDiagnosis.vue?0aefad15