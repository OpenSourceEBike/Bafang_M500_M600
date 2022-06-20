<template>
    <div>
        <div style="width:300px;">
            <div class="item">
                <span>Ebike ID</span>
                <el-input v-model="ebikeID"></el-input>
            </div>
            <div class="item">
                <span>Battery SN</span>
                <el-input v-model="batterySN"></el-input>
            </div>
            <div class="item">
                <el-button @click="replace">替换</el-button>
            </div>
        </div>
        <div class="btn-group-inline">
            <div class="icons">
                <div class="icon-item replace" @click="replace"
                     title="replace"></div>
            </div>
        </div>
    </div>

</template>

<script>
    import areaCode from '../../../core/areaCode'
    export default {
        data() {
            return {
                ebikeID: '',
                batterySN: '',
            }
        },
        mounted() {

        },

        methods: {
            getAvgPrice(total, yearIntRate, month) {
                console.log(1)
                yearIntRate = 0.043;
                let monthIntRate = yearIntRate / 12;
                let x  =   Math.pow((1 + monthIntRate ), month);
                this.a = total * monthIntRate * x / (x - 1);
                console.log(a, 'xxxx');
            },
            replace(){
                let id= this.ebikeID.replace(/\s+/g, "");
                let sn= this.batterySN.replace(/\s+/g, "");

                this.$http.get('http://58.211.213.54:9898/client/1/object/battery-replace?vehicle_sn='+id+'&battery_sn='+sn+'&token=9e393b1ae594706896a9b037dc0dd9b9').then((response) => {
                        console.log( response );
                        let status = response.data.status;
                        console.log( status );
                        switch( status) {
                            case '1':
                                this.message('vehicle not exist traced by vehicle SN');
                                break;
                            case '2':
                                this.message('component not exist traced by component SN');
                                break;
                            case '3':
                                this.message('component not bound to any vehicle bom');
                                break;
                            case '4':
                                this.message('vehicle traced by component SN is not exist or deleted');
                                break;
                            case '5':
                                this.message('replace success');
                                break;
                            case '6':
                                this.message('bom of vehicle is empty');
                                break;
                        }
                })
            },



        }
    }
</script>



// WEBPACK FOOTER //
// test.vue?43729eff