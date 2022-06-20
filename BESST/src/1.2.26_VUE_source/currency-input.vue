<template>
    <div class="el-input">
        <input type="text" v-model="displayValue" class="el-input__inner"/>
    </div>
</template>
<script type="text/ecmascript-6">
    import Core from 'core/core';

    export default {
        props: {
            value: Number
        },
        data() {
            return {
            };
        },
        computed: {
            displayValue: {
                get: function() {
                    return '' + (this.value / 100)
                },
                set: function(modifiedValue) {
                    let newValue = parseInt(modifiedValue.replace(/[^\d\.]/g, ""));
                    if (isNaN(newValue)) {
                        newValue = 0
                    }
                    this.$emit('input', newValue * 100)
                }
            }
        }
    };
</script>

<style scoped lang="scss">

</style>



// WEBPACK FOOTER //
// currency-input.vue?1351524c