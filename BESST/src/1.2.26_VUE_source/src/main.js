// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'
import ElementUI from 'element-ui';
import filters from 'core/filters';
import VueI18n from 'vue-i18n'
import locale from 'element-ui/lib/locale'
import enLocale from 'element-ui/lib/locale/lang/en';
import deLocale from 'element-ui/lib/locale/lang/de';
import zhLocale from 'element-ui/lib/locale/lang/zh-CN';
import nlLocale from 'element-ui/lib/locale/lang/nl';
import VueClip from 'vue-clip'
import Vuex from 'vuex'
import VeLine from 'v-charts/lib/line'
import VueClipboard from 'vue-clipboard2'
import XLSX from 'xlsx/dist/xlsx.full.min'

import $ from 'jquery'

import Core from '../core/core'
//api
import WarehouseService from "src/services/warehouseService"
// css
// import 'element-ui/lib/theme-default/index.css'
import '../theme/index.css'
import '../src/assets/style/base.css';
import '../src/assets/style/index.scss';

//系统错误捕获
const errorHandler = (err, vm, info) => {
    // console.log(err.toString());
    // console.log(vm.$vnode.elm.className);
    // console.log(vm.$vnode.elm.baseURI);
    // console.log(info);
    // console.log(Core.Data.getUser() && Core.Data.getUser().email);
    // console.log(new Date().getTime());

    var a = {
        err:err.toString(),
        className:vm.$vnode.elm.className,
        url:vm.$vnode.elm.baseURI,
        info:info,
        user:Core.Data.getUser() && Core.Data.getUser().email,
        time:new Date().getTime()
    }

    // Core.Api.

}

//  main.js
window.myfunction = function(win) {
    console.log('in1')
}

Vue.prototype.mywindow = window

// vue 组件调用
// mounted(){
//     console.log(this.mywindow)
//     console.log(this.mywindow.myfunction)
//     this.mywindow.myfunction()
// }


Vue.config.errorHandler = errorHandler;
Vue.prototype.$throw = (error) => errorHandler(error, this);

Vue.config.productionTip = false;

filters(Vue);

Vue.use(VueI18n);
Vue.use(ElementUI);
Vue.use(VueClipboard)

let lang = Core.Data.getLang() || 'en';

switch (lang) {
    case 'en':
        locale.use(enLocale);
        break;
    case 'zh':
        locale.use(zhLocale);
        break;
    case 'de':
        locale.use(deLocale);
        break;
    case 'nl':
        locale.use(nlLocale);
        break;
    default:
        locale.use(enLocale);
        break;
}

Vue.use(VueClip);
Vue.use(Vuex);
Vue.component(VeLine.name, VeLine);

const i18n = new VueI18n({
    locale: lang,
    fallbackLocale: 'en',
    messages: []
});

const store = new Vuex.Store({
    state: {
        currentCategory: 0,
        changeFilter: {},
        currentBackStatus: false,
        userPermissions: [],
        userAuth: [],
        userAccount: "",
    },
    mutations: {
        currentCategory(state, payload) {
            state.currentCategory = payload.category;
        },
        setChangeFilter(state, changeFilter) {
            state.changeFilter = changeFilter;
        },
        changeTrueStatus(state) {
            state.currentBackStatus = true;
        },
        changeFalseStatus(state) {
            state.currentBackStatus = false;
        },
        currentAuth(state, payload) {
            state.userAuth = payload;
        },
        userAccount(state, payload) {
            state.userAccount = payload;
        },
        //更新权限
        changePermissions(state, data) {
            state.userPermissions = data;
        },
    },
    actions: {
        // 获取权限列表
        getPermission({ commit }) {
            // return new Promise((resolve, reject) => {
            //     WarehouseService.getAllPermission().then((res) => {
            //         // 存储权限列表
            //         console.log(res);
            //         if(res.data.length > 0){
            //             let userPermissions = res.data;
            //             commit('changePermissions', userPermissions);
            //             resolve(userPermissions)
            //         }
            //
            //     }).catch(() => {
            //         reject()
            //     })
            // })
        }
    }
});

// router.beforeEach((to, form, next) => {
//     const token = Core.Data.getToken();
//     const org = Core.Data.getOrg();
//     if (org && org.type === 5) {
//         if (to.path === '/login') {
//             next();
//         }
//         if (token && token.trim().length !== 0) {
//             if (to.path === '/') {
//                 router.replace('/login');
//             } else {
//                 if (store.state.userPermissions.length === 0) {
//                     store.dispatch('getPermission').then(res => {
//                         next(to.fullPath)
//                     }).catch(() => {
//                         router.replace('/')
//                     })
//                 } else {
//                     if (to.matched.length) {
//                         next()
//                     } else {
//                         router.replace('/')
//                     }
//                 }
//             }
//         } else {
//             router.replace('/login');
//         }
//     } else {
//         next();
//     }
// });

// 判断权限是否存在
Vue.prototype.$_has = function(permission) {
    const _userPermissions = store.state.userPermissions;
    const count = _userPermissions.filter(per => {
        return per.resourceId === permission && per.permissioned;
    });
    if (count.length > 0) {
        return true;
    }
    return false;
};

Vue.directive('ilpermission', {
    inserted: function(el, binding) {
        if (Vue.prototype.$_has && !Vue.prototype.$_has(binding.value)) {
            el.parentNode.removeChild(el);
        }
    }
})

//输入数字限制
const trigger = (el, type) => {
    const e = document.createEvent('HTMLEvents')
    e.initEvent(type, true, true)
    el.dispatchEvent(e)
}

Vue.directive('onlyNum', {
    bind: function(el) {
        const input = el.getElementsByTagName('input')[0]
        input.onkeyup = function(e) {
            if (input.value.length === 1) {
                input.value = input.value.replace(/[^1-9]/g, '')
            } else {
                input.value = input.value.replace(/[^\d]/g, '')
            }
            trigger(input, 'input')
        }
        input.onblur = function(e) {
            if (input.value.length === 1) {
                input.value = input.value.replace(/[^1-9]/g, '')
            } else {
                input.value = input.value.replace(/[^\d]/g, '')
            }
            trigger(input, 'input')
        }
    }
})

/* eslint-disable no-new */
new Vue({
    el: '#app',
    i18n,
    store,
    router,
    template: '<App/>',
    components: {
        App
    },
    data: {
        eventHub: new Vue()
    },
    mounted() {
        this.eventHub.$on(Core.Const.DATA.KEY_I18N, () => {
            i18n.locale = Core.Data.getLang() || 'en';
            this.eventHub.$emit(Core.Const.DATA.KEY_I18N_UPDATE);
        });
        // this.eventHub.$on(Core.Const.DATA.KEY_BATCH_RECORD, (data) => {
        //     console.log(data);
        //     if (data) {
        //         let list = Core.Data.getSavedRecordIndexList() || [];
        //         if (list.indexOf(data) < 0) {
        //             list.push(data);
        //             Core.Data.setSavedRecordIndexList(list);
        //         }
        //     }
        // })
    }
});



// WEBPACK FOOTER //
// ./src/main.js