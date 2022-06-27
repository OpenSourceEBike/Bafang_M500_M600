<template>
    <div>
        <div style="border: solid 2px #999fff; float: left; padding: 5px; margin-bottom: 5px;">
            <div style="text-align: center;">
                <span style="font-weight: bold;">Contains content that I want to print</span>
                <br>
                This is just a sample page to demonstrate this plugin.
            </div>
            <div class="PrintArea area1 all" id="Retain">
                <div>Print Area 1</div>
                <span style="color: #000777;">print me</span>
                <span class="test">xyz</span>
                <br>
                <input name="chkTest1" value="1" type="checkbox">
                <input name="chkTest2" value="2" type="checkbox">
                <br>
                <input value="3" name="rdoTest3" type="radio">
                <input value="4" name="rdoTest3" type="radio">
                <br>
                <input value="" name="textTest4" type="">
                <input value="" name="textTest5" type="text">
                <br>
                <select name="selTest6">
                    <option value="A">A</option>
                    <option value="B">B</option>
                    <option value="C">C</option>
                </select>
                <select name="selTest7" multiple="">
                    <option value="a">a</option>
                    <option value="b">b</option>
                    <option value="c">c</option>
                </select>
                <br>
                <textarea name="textareaTest8"></textarea>
                <br>
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
            </div>

            <div style="border: solid 1px #999; margin: 20px 5px; padding: 20px;">
                I don't want this to print.
            </div>

            <div class="PrintArea area2 all" style="border-color: #999;">
                <div>Print Area 2</div>
                <span style="color: #000777;">print me</span>
                <span class="test">xyz</span>
                <br>
                <input name="chkTest3" value="1" type="checkbox">
                <input name="chkTest4" value="2" type="checkbox">
                <br>
                <input value="3" name="rdoTest4" type="radio">
                <input value="4" name="rdoTest4" type="radio">
                <br>
                <input value="" name="textTest6" type="text">
                <br>
                <select name="selTest7">
                    <option value="A">A</option>
                    <option value="B">B</option>
                    <option value="C">C</option>
                </select>
                <select name="selTest8" multiple="">
                    <option value="a">a</option>
                    <option value="b">b</option>
                    <option value="c">c</option>
                </select>
                <br>
                <textarea name="textareaTest9"></textarea>
                <br>
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
                <img src="FoesEnd.jpg" id="img2" alt="test" width="200">
            </div>
        </div>

        <div class="testDialog">
            <div class="PrintArea area3 all">
                This is Print Area 3
            </div>
        </div>

        <div style="border: solid 2px #999fff; float: left; margin-left: 20px;" class="SettingsBox">
            <div class="button b1">Print</div>
        </div>
    </div>
</template>

<script>
    import printArea from 'printArea'
    export default {
        name: 'hello',
        data () {
            return {
                msg: 'Welcome to Your Vue.js App'
            }
        },
        mounted () {
            $(document).ready(function(){
                $("div.b1").click(function(){
                    let print = "div.PrintArea.area1, div.PrintArea.area2, div.PrintArea.area3";

                    let extraCss = "";
                    let mode = "iframe";
                    let close = false;
                    let retainAttr = ['class', 'id', 'style', 'on'];
                    let headElements = '<meta charset="utf-8" />,<meta http-equiv="X-UA-Compatible" content="IE=edge"/>';

                    let options = { mode : mode, popClose : close, extraCss : extraCss, retainAttr : retainAttr, extraHead : headElements };
                    $(print).printArea(options);
                });
            });
        }
    }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    .buttonBar {
        height: 30px;
    }

    .buttonBar div {
        display: inline-block;
        float: left;
        margin-right: 10px;
        padding: 3px 12px;
        cursor: pointer;
    }

    .button {
        background: none repeat scroll 0 0 #999999;
        border: 1px solid blue;
        color: #EEEEEE;
        text-align: center;
    }

    div.settingName {
        display: inline-block;
        vertical-align: top;
    }

    div.settingVals {
        display: inline-block;
        padding-left: 10px;
    }

    span.test {
        color: red;
    }

    #Retain.PrintArea.area1 {
        border: 1px solid #999;
        background: red;
    }

</style>



// WEBPACK FOOTER //
// print.vue?f56c258c