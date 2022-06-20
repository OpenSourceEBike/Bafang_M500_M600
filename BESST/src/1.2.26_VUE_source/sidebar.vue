<template>
    <div class="sidebar Noprint">
        <div class="user-info">
            <div class="logo-div">
                <img class="user-avatar" src="../../assets/images/main/img-default-avatar.png"
                     :src="org.avatar | getImgUrl">
                <div class="user-name-box"  v-if="org.type == Core.Const.ORG_TYPE.COMPONENT || org.type == Core.Const.ORG_TYPE.SERVICE_CENTER">
                    <img class="user-icon" src="../../assets/images/main/icon-user.png">
                    <span class="user-name">{{ org.name }}</span>
                </div>
                <div class="brand-info"  v-else>
                    <div class="user-name-box" @click="tapUserName">
                        <img class="brand-icon" src="../../assets/images/main/icon-brand.png">
                        <span class="user-name" v-if="org.type == Core.Const.ORG_TYPE.BRAND">{{ org.name }}</span>
                        <span class="user-name" v-else>{{ $t('brand') }}</span>
                        <!--<span class="user-name" v-if="brand !== null && brand">{{ brand.name }}</span>-->
                        <img class="arrow-icon" src="../../assets/images/main/arrow-right.png">
                    </div>
                </div>
            </div>
        </div>

        <div class="menu-container">
            <el-menu :default-active="$route.path" class="el-menu-vertical-demo" theme="dark" :default-openeds="defaultOpenList" :unique-opened="true">
                <div v-if="org.type == 1 && !offline" class="new">
                    <el-menu-item index="/brand/brand/list" v-if="authList['brand.menu']">
                        <a class="a-link" @click="$router.push('/brand/brand/list')">
                            <img class="icon" src="../../assets/images/icon/menu/brand.png"/>{{ $t('brand') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/brand/model/list" v-if="authList['item.menu']">
                        <a class="a-link"
                           @click="$router.push({ path: '/brand/model/list', query: { page: 1, index: 0 }})">
                            <img class="icon" src="../../assets/images/icon/menu/product.png"/>{{ $t('model') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/brand/vehicle/list" v-if="authList['object.menu']">
                        <a class="a-link" @click="$router.push('/brand/vehicle/list')">
                            <img class="icon" src="../../assets/images/icon/menu/model.png"/>{{ $t('vehicle') }}
                        </a>
                    </el-menu-item>
                    <!--<el-menu-item index="/brand/error/shooting" v-if="authList['error_shooting.menu']">-->
                    <!--<a class="a-link" @click="$router.push('/brand/error/shooting')">-->
                    <!--<img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>{{ $t('errorCode') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-submenu index="/brand/diagnosis" v-if="authList['diagnose.menu']">-->
                    <!--<template slot="title">-->
                    <!--<img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}-->
                    <!--</template>-->
                    <!--<el-menu-item index="/diagnosis">-->
                    <!--<a class="a-link" @click="$router.push('/brand/diagnosis/3rd-diagnosis')">{{ $t('diagnosis') }}</a>-->
                    <!--&lt;!&ndash;    <a class="a-link" @click="$router.push('/brand/diagnosis/diagnosis')">{{ $t('diagnosis') }}</a>  &ndash;&gt;-->

                    <!--</el-menu-item>-->
                    <!--<el-menu-item index="/report-list">-->
                    <!--<a class="a-link" @click="$router.push('/brand/diagnosis/report-list')">{{ $t('report') }}</a>-->
                    <!--</el-menu-item>-->
                    <!--</el-submenu>-->
                    <el-submenu index="/brand/partner/dealer" v-if="authList['dealer.menu']">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/dealer.png"/>{{ $t('dealer') }}
                        </template>
                        <el-menu-item index="/brand/partner/dealer/list">
                            <a class="a-link" @click="$router.push('/brand/partner/dealer/list')">
                                {{ $t('dealerList') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/partner/dealer/invite" v-if="authList['dealer.invitation']">
                            <a class="a-link" @click="$router.push('/brand/partner/dealer/invite')">
                                {{ $t('invitation') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-submenu index="/brand/partner/oem" v-if="authList['oem.menu']">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/oem.png"/>{{ $t('oem') }}
                        </template>
                        <el-menu-item index="/brand/partner/oem/list">
                            <a class="a-link" @click="$router.push('/brand/partner/oem/list')">
                                {{ $t('oemList') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/partner/oem/invite" v-if="authList['oem.invitation']">
                            <a class="a-link" @click="$router.push('/brand/partner/oem/invite')">
                                {{ $t('invitation') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!--<el-submenu index="/brand/partner/component">-->
                    <!--<template slot="title">-->
                    <!--<img class="icon" src="../../assets/images/icon/menu/component.png"/>{{ $t('compon') }}-->
                    <!--</template>-->
                    <!--<el-menu-item index="/component-list">-->
                    <!--<a class="a-link" @click="$router.push('partner/component/list')">-->
                    <!--{{ $t('componentList') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--</el-submenu>-->
                    <el-menu-item index="/brand/partner/component/list" v-if="authList['item.menu']">
                        <a class="a-link"
                           @click="$router.push({ path: '/brand/partner/component/list', query: { page: 1, index: 0 }})">
                            <img class="icon" src="../../assets/images/icon/menu/component.png"/>{{ $t('componentList') }}
                        </a>
                    </el-menu-item>
                    <!--<el-submenu index="/brand/service-center" v-if="authList['ticket.menu']">-->
                    <!--<template slot="title">-->
                    <!--<img class="icon" src="../../assets/images/icon/menu/service.png"/>{{ $t('serviceCenter') }}-->
                    <!--</template>-->
                    <!--</el-submenu>-->
                    <el-menu-item index="/brand/account/list" v-if="authList['account.menu']">
                        <a class="a-link" @click="$router.push('/brand/account/list')">
                            <img class="icon" src="../../assets/images/icon/menu/account.png"/>{{ $t('account') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/service">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/service.png"/>{{ $t('serviceCenter') }}
                        </template>
                        <el-menu-item index="/brand/service-center/ticket/service-center-list" v-if="authList['ticket.service_center']">
                            <a class="a-link" @click="$router.push('/brand/service-center/ticket/service-center-list')">
                                {{ $t('serviceCenters') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/service-center/ticket/center-ticket-list" v-if="authList['ticket.complaint']">
                            <a class="a-link" @click="$router.push('/brand/service-center/ticket/center-ticket-list')">
                                {{ $t('complaints') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/service-center/ticket/my-ticket-list" v-if="authList['ticket.task']">
                            <a class="a-link" @click="$router.push('/brand/service-center/ticket/my-ticket-list')">
                                {{ $t('tasks') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/diagnosis/3rd-diagnosis" v-if="authList['diagnose.menu']">
                            <a class="a-link" @click="$router.push('/brand/diagnosis/3rd-diagnosis')">
                                <img src="../../assets/images/icon/menu/diagnosis.png" class="icon">{{ $t('diagnosis') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/diagnosis/report-list">
                            <a class="a-link" @click="$router.push('/brand/diagnosis/report-list')">
                                <img src="../../assets/images/icon/menu/order.png" class="icon">{{ $t('report') }}
                            </a>
                        </el-menu-item>
                        <el-submenu index="/tool" v-if="authList['tool.menu']">
                            <template slot="title"><img class="icon" src="../../assets/images/icon/menu/tool.png"/><span class="titleSec">{{ $t('tool') }}</span></template>
                            <el-menu-item index="/brand/tool/hmi">
                                <a class="a-link" @click="$router.push('/brand/tool/hmi')">
                                    <img src="../../assets/images/icon/icon-hmi.png" class="icon">{{ $t('hmi') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/brand/tool/controller">
                                <a class="a-link" @click="$router.push('/brand/tool/controller')">
                                    <img src="../../assets/images/icon/icon-controller.png" class="icon">{{ $t('controller') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/brand/tool/battery">
                                <a class="a-link" @click="$router.push('/brand/tool/battery')">
                                    <img src="../../assets/images/icon/icon-battery.png" class="icon">{{ $t('battery') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/component/tool/sensor">
                                <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                    <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/brand/tool/update">
                                <a class="a-link" @click="$router.push('/brand/tool/update')">
                                    <img src="../../assets/images/icon/icon-update.png" class="icon">{{ $t('update') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/brand/tool/record">
                                <a class="a-link" @click="$router.push('/brand/tool/record')">
                                    {{ $t('record') }}
                                </a>
                            </el-menu-item>
                        </el-submenu>
                        <el-submenu index="/error/shooting">
                            <template slot="title">
                                <img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>{{ $t('faq') }}
                            </template>
                            <el-menu-item index="/error/shooting" v-if="authList['error_shooting.menu']">
                                <a class="a-link" @click="$router.push('/brand/error/shooting')">
                                    {{ $t('errorCode') }}
                                </a>
                            </el-menu-item>
                        </el-submenu>
                    </el-submenu>
                    <!--<el-menu-item index="/brand/account/list" v-if="authList['account.menu']">-->
                    <!--<a class="a-link" @click="$router.push('/brand/account/list')">-->
                    <!--<img class="icon" src="../../assets/images/icon/menu/account.png"/>{{ $t('account') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-submenu index="/dealer/tool" v-if="authList['tool.menu']">-->
                    <!--<template slot="title">-->
                    <!--<img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}-->
                    <!--</template>-->
                    <!--<el-menu-item index="/hmi">-->
                    <!--<a class="a-link" @click="$router.push('/brand/tool/hmi')">-->
                    <!--<img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item index="/controller">-->
                    <!--<a class="a-link" @click="$router.push('/brand/tool/controller')">-->
                    <!--<img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item index="/battery">-->
                    <!--<a class="a-link" @click="$router.push('/brand/tool/battery')">-->
                    <!--<img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item index="/sensor">-->
                    <!--<a class="a-link" @click="$router.push('/component/tool/sensor')">-->
                    <!--<img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item index="/update">-->
                    <!--<a class="a-link" @click="$router.push('/brand/tool/update')">-->
                    <!--<img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--&lt;!&ndash;<el-menu-item index="/auth">&ndash;&gt;-->
                    <!--&lt;!&ndash;<a class="a-link" @click="$router.push('/brand/tool/auth')">&ndash;&gt;-->
                    <!--&lt;!&ndash;{{ $t('auth') }}&ndash;&gt;-->
                    <!--&lt;!&ndash;</a>&ndash;&gt;-->
                    <!--&lt;!&ndash;</el-menu-item>&ndash;&gt;-->
                    <!--<el-menu-item index="/record">-->
                    <!--<a class="a-link" @click="$router.push('/brand/tool/record')">-->
                    <!--{{ $t('record') }}-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--</el-submenu>-->
                </div>

                <div v-else-if="org.type == 1 && offline">
                    <el-menu-item index="/brand/diagnosis/3rd-diagnosis">
                        <a class="a-link" @click="$router.push('/brand/diagnosis/3rd-diagnosis')">
                            <img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/component/tool">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}
                        </template>
                        <el-menu-item index="/brand/tool/hmi">
                            <a class="a-link" @click="$router.push('/brand/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/tool/controller">
                            <a class="a-link" @click="$router.push('/brand/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/tool/battery">
                            <a class="a-link" @click="$router.push('/brand/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/sensor">
                            <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/brand/tool/update">
                            <a class="a-link" @click="$router.push('/brand/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>
                <!--
                <div v-else-if="org.type == 9 && !offline">
                    <el-submenu index="/dealer/diagnosis" v-if="authList['diagnose.menu']">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}
                        </template>
                        <el-menu-item index="/mechanic-detect">
                            <a class="a-link" @click="$router.push('/dealer/mechanic-detect')">
                                Mechanic Detect
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/diagnosis">
                            <a class="a-link" @click="$router.push('/dealer/diagnosis/3rd-diagnosis')">
                                {{ $t('diagnosis') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/report-list">
                            <a class="a-link" @click="$router.push('/dealer/diagnosis/report-list')">
                                {{ $t('report') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-menu-item index="/dealer/sale/list" v-if="authList['order.menu']">
                        <a class="a-link " @click="$router.push('/dealer/sale/list')">
                            <img class="icon" src="../../assets/images/icon/menu/order.png"/>{{ $t('order') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/dealer/vehicle/search" v-if="authList['object.menu']">
                        <a class="a-link" @click="$router.push('/dealer/vehicle/search')">
                            <img class="icon" src="../../assets/images/icon/menu/model.png"/>{{ $t('vehicle') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/dealer/ticket/list" v-if="authList['ticket.menu']">
                        <a class="a-link" @click="$router.push('/dealer/ticket/list')">
                            <img class="icon" src="../../assets/images/icon/menu/service.png"/>{{ $t('serviceCenter') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/dealer/account/list" v-if="authList['account.menu']">
                        <a class="a-link" @click="$router.push('/dealer/account/list')">
                            <img class="icon" src="../../assets/images/icon/menu/account.png"/>{{ $t('account') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/dealer/error/shooting" v-if="authList['error_shooting.menu']">
                        <a class="a-link" @click="$router.push('/dealer/error/shooting')">
                            <img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>{{ $t('errorCode') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/dealer/tool" v-if="authList['tool.menu']">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}
                        </template>
                        <el-menu-item index="/hmi">
                            <a class="a-link" @click="$router.push('/dealer/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/controller">
                            <a class="a-link" @click="$router.push('/dealer/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/battery">
                            <a class="a-link" @click="$router.push('/dealer/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/sensor">
                            <a class="a-link" @click="$router.push('/dealer/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/update">
                            <a class="a-link" @click="$router.push('/dealer/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>
                -->
                <div v-else-if="org.type == 2 && offline">
                    <el-menu-item index="/dealer/diagnosis/3rd-diagnosis">
                        <a class="a-link" @click="$router.push('/dealer/diagnosis/3rd-diagnosis')">
                            <img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/component/tool">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}
                        </template>
                        <el-menu-item index="/dealer/tool/hmi">
                            <a class="a-link" @click="$router.push('/dealer/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/tool/controller">
                            <a class="a-link" @click="$router.push('/dealer/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/tool/battery">
                            <a class="a-link" @click="$router.push('/dealer/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/sensor">
                            <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/tool/update">
                            <a class="a-link" @click="$router.push('/dealer/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>

                <!-- -->
                <div v-else-if="org.type == 2" class="new">
                    <el-menu-item index="/dealer/partner/list" v-if="authList['object.menu']">
                        <a class="a-link" @click="$router.push('/dealer/partner/list')">
                            <img src="../../assets/images/icon/menu/product.png" class="icon">{{ $t('bikeModel') }}</a>
                    </el-menu-item>
                    <el-menu-item index="/dealer/sale/list" v-if="authList['object.menu']">
                        <a class="a-link" @click="$router.push('/dealer/sale/list')">
                            <img src="../../assets/images/icon/menu/order.png" class="icon">{{ $t('bikeSales') }}</a>
                    </el-menu-item>
                    <el-menu-item index="/vehicle/search" v-if="authList['object.menu']">
                        <a class="a-link" @click="$router.push('/dealer/vehicle/search')">
                            <img src="../../assets/images/icon/menu/model.png" class="icon">{{ $t('bikeInfo') }}</a>
                    </el-menu-item>
                    <el-menu-item index="/dealer/account/list" v-if="authList['account.menu']">
                        <a class="a-link" @click="$router.push('/dealer/account/list')">
                            <img src="../../assets/images/icon/menu/account.png" class="icon">{{ $t('account') }}</a>
                    </el-menu-item>
                    <!--<el-menu-item index="1-4">-->
                    <!--<a class="a-link" @click="$router.push('')">-->
                    <!--<img src="" class="icon">{{ $t('author') }}</a>-->
                    <!--</el-menu-item>-->
                    <el-submenu index="/dealer/service">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/service.png"/>{{ $t('serviceCenter') }}
                        </template>
                        <el-menu-item index="/dealer/ticket/list" v-if="authList['ticket.menu']">
                            <a class="a-link" @click="$router.push('/dealer/ticket/list')">
                                <img src="../../assets/images/icon/menu/service.png" class="icon">{{ $t('serviceCenter') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/diagnosis/3rd-diagnosis" v-if="authList['diagnose.menu']">
                            <a class="a-link" @click="$router.push('/dealer/diagnosis/3rd-diagnosis')">
                                <img src="../../assets/images/icon/menu/diagnosis.png" class="icon">{{ $t('diagnosis') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/diagnosis/report-list">
                            <a class="a-link" @click="$router.push('/dealer/diagnosis/report-list')">
                                <img src="../../assets/images/icon/menu/order.png" class="icon">{{ $t('report') }}
                            </a>
                        </el-menu-item>
                        <el-submenu index="/tool" v-if="authList['tool.menu']">
                            <template slot="title"><span class="titleSec">{{ $t('tool') }}</span></template>
                            <el-menu-item index="/dealer/tool/hmi">
                                <a class="a-link" @click="$router.push('/dealer/tool/hmi')">
                                    <img src="../../assets/images/icon/icon-hmi.png" class="icon">{{ $t('hmi') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/dealer/tool/controller">
                                <a class="a-link" @click="$router.push('/dealer/tool/controller')">
                                    <img src="../../assets/images/icon/icon-controller.png" class="icon">{{ $t('controller') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/dealer/tool/battery">
                                <a class="a-link" @click="$router.push('/dealer/tool/battery')">
                                    <img src="../../assets/images/icon/icon-battery.png" class="icon">{{ $t('battery') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/component/tool/sensor">
                                <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                    <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                                </a>
                            </el-menu-item>
                            <el-menu-item index="/dealer/tool/update">
                                <a class="a-link" @click="$router.push('/dealer/tool/update')">
                                    <img src="../../assets/images/icon/icon-update.png" class="icon">{{ $t('update') }}
                                </a>
                            </el-menu-item>
                        </el-submenu>
                        <el-submenu index="/error/shooting">
                            <template slot="title">
                                <img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>FAQ
                            </template>
                            <el-menu-item index="/dealer/error/shooting">
                                <a class="a-link" @click="$router.push('/dealer/error/shooting')">
                                    {{ $t('errorCode') }}
                                </a>
                            </el-menu-item>
                        </el-submenu>
                    </el-submenu>
                </div>

                <div v-else-if="org.type == 3">
                    <el-menu-item index="/assembler/order/list" v-if="authList['production.menu']">
                        <a class="a-link" @click="$router.push('/assembler/order/list')">
                            <img class="icon" src="../../assets/images/icon/menu/order.png"/>{{ $t('order') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/assembler/vehicle/list" v-if="authList['object.menu']">
                        <a class="a-link" @click="$router.push('/assembler/vehicle/list')">
                            <img class="icon" src="../../assets/images/icon/menu/model.png"/>{{ $t('vehicle') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/assembler/diagnosis">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}
                        </template>
                        <el-menu-item index="/assembler/diagnosis/3rd-diagnosis">
                            <a class="a-link" @click="$router.push('/assembler/diagnosis/3rd-diagnosis')">{{ $t('diagnosis') }}</a>
                        </el-menu-item>
                        <el-menu-item index="/assembler/diagnosis/report-list">
                            <a class="a-link" @click="$router.push('/assembler/diagnosis/report-list')">{{ $t('report') }}</a>
                        </el-menu-item>
                    </el-submenu>
                    <el-menu-item index="/assembler/account/list" v-if="authList['account.menu']">
                        <a class="a-link" @click="$router.push('/assembler/account/list')">
                            <img class="icon" src="../../assets/images/icon/menu/account.png"/>{{ $t('account') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/error/shooting">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>{{ $t('faq') }}
                        </template>
                        <el-menu-item index="/assembler/error/shooting">
                            <a class="a-link" @click="$router.push('/assembler/error/shooting')">
                                {{ $t('errorCode') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-submenu index="/component/tool" v-if="authList['tool.menu']">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}
                        </template>
                        <el-menu-item index="/component/tool/hmi">
                            <a class="a-link" @click="$router.push('/component/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/controller">
                            <a class="a-link" @click="$router.push('/component/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/battery">
                            <a class="a-link" @click="$router.push('/component/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/sensor">
                            <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/update">
                            <a class="a-link" @click="$router.push('/component/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>

                <div v-else-if="org.type == 4 && !offline">
                    <el-menu-item index="/component/product/list" v-if="authList['product.menu']">
                        <a class="a-link" @click="$router.push('/component/product/list')">
                            <img class="icon" src="../../assets/images/icon/menu/product.png"/>{{ $t('product') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/component/param" v-if="authList['product.menu'] && org.id == 25 ">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/param.png"/>{{ $t('tt.param') }}
                        </template>
                        <el-menu-item index="/component/project/">
                            <a class="a-link" @click="$router.push('/component/project/index')">
                                <img class="icon" src="../../assets/images/icon/menu/project.png">
                                {{ $t('tt.system') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/testPlan/testPlan">
                            <a class="a-link" @click="$router.push('/component/testPlan/testPlan')">
                                <img class="icon" src="../../assets/images/icon/list.png">
                                {{ $t('tt.testPlan') }}
                            </a>
                        </el-menu-item>

                        <el-menu-item index="/component/createParam/">
                            <a class="a-link" @click="$router.push('/component/createParam/index')">
                                <img class="icon" src="../../assets/images/icon/menu/param.png">
                                {{ $t('tt.param') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/headList/">
                            <a class="a-link" @click="checkHeadList">
                                <img class="icon" src="../../assets/images/icon/list.png">
                                {{ $t('tt.headList') }}
                            </a>
                        </el-menu-item>
                        <!--<el-menu-item index="/component/client/">-->
                            <!--<a class="a-link" @click="$router.push('/component/client/index')">-->
                                <!--<img class="icon" src="../../assets/images/icon/list.png">-->
                                <!--{{ $t('tt.client') }}-->
                            <!--</a>-->
                        <!--</el-menu-item>-->
                        <el-menu-item index="/component/dictionary/detail">
                            <a class="a-link" @click="$router.push('/component/dictionary/detail')">
                                <img class="icon" src="../../assets/images/icon/dictionary.png">
                                {{ $t('tt.dictionary') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-menu-item index="/component/component/list" v-if="authList['object.menu']">
                        <a class="a-link" @click="$router.push('/component/component/list')">
                            <img class="icon" src="../../assets/images/icon/menu/component.png"/>{{ $t('component') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/component/order/list" v-if="authList['production.menu']">
                        <a class="a-link" @click="$router.push('/component/order/list')">
                            <img class="icon" src="../../assets/images/icon/menu/order.png"/>{{ $t('order') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/component/service-center" v-if="authList['ticket.menu']">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/service.png"/>{{ $t('serviceCenter') }}
                        </template>
                        <el-menu-item index="/ticket/service-center-list" v-if="authList['ticket.service_center']">
                            <a class="a-link" @click="$router.push({name: 'componentServiceCenterAssignList'})">
                                {{ $t('serviceCenters') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/ticket/complaint" v-if="authList['ticket.complaint']">
                            <a class="a-link" @click="$router.push({name: 'componentServiceCenterTicketList'})">
                                {{ $t('complaints') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/ticket/task" v-if="authList['ticket.task']">
                            <a class="a-link" @click="$router.push({name: 'componentMyTicketList'})">
                                {{ $t('tasks') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/ticket/statistics">
                            <a class="a-link" @click="$router.push({name: 'componentTicketStatistics'})">
                                {{ $t('statistics') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-menu-item index="/component/account/list" v-if="authList['account.menu']">
                        <a class="a-link" @click="$router.push('/component/account/list')">
                            <img class="icon" src="../../assets/images/icon/menu/account.png"/>{{ $t('account') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/error/shooting">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>FAQ
                        </template>
                        <el-menu-item index="/component/error/shooting">
                            <a class="a-link" @click="$router.push('/component/error/shooting')">
                                {{ $t('errorCode') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-submenu index="/component/diagnosis">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}
                        </template>
                        <el-menu-item index="/component/diagnosis/3rd-diagnosis">
                            <a class="a-link" @click="$router.push('/component/diagnosis/3rd-diagnosis')">
                                {{ $t('diagnosis') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/diagnosis/report-list">
                            <a class="a-link" @click="$router.push('/component/diagnosis/report-list')">
                                {{ $t('report') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-submenu index="/component/audit-apply/apply" v-show="org.id == 25 ">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/order.png"/>
                                Application record
                        </template>
                        <el-menu-item index="/component/audit-apply/apply" v-if="accountType_2">
                            <a class="a-link" @click="$router.push('/component/audit-apply/apply')">
                                Apply BESST Account
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/audit-apply/audit" v-if="accountType_3">
                            <a class="a-link" @click="$router.push('/component/audit-apply/audit')">
                                Audit BESST Account
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/audit-apply/ip">
                            <a class="a-link" @click="$router.push('/component/audit-apply/ip')">
                                Apply IP Verification
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <el-menu-item index="/component/material/list" v-show="org.id == 25 ">
                        <a class="a-link" @click="$router.push('/component/material/list')">
                            <img class="icon" src="../../assets/images/icon/menu/order.png"/>{{ $t('material') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/component/feedback-center/list" v-show="org.id == 25 ">
                        <a class="a-link" @click="$router.push('/component/feedback-center/list')">
                            <img class="icon" src="../../assets/images/icon/menu/order.png"/>{{ $t('testFeedback') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/component/tool" v-if="authList['tool.menu']">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}
                        </template>
                        <el-menu-item index="/component/tool/hmi">
                            <a class="a-link" @click="$router.push('/component/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/controller">
                            <a class="a-link" @click="$router.push('/component/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/battery">
                            <a class="a-link" @click="$router.push('/component/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/sensor">
                            <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/update">
                            <a class="a-link" @click="$router.push('/component/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/record" v-show="org.id == 25">
                            <a class="a-link" @click="$router.push('/component/tool/record')">
                                {{ $t('tool') }} {{ $t('record') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/firmware-delete-record" v-show="org.id == 25">
                            <a class="a-link" @click="$router.push('/component/tool/operationTraceRecord')">
                                {{ $t('component') }} {{ $t('record') }}
                            </a>
                        </el-menu-item>
                        <!--<el-menu-item index="/component/tool/basic-params" v-show="org.id == 25">
                            <a class="a-link" @click="$router.push('/component/tool/basic-params')">
                                Battery Basic Params
                            </a>
                        </el-menu-item>-->
                        <el-menu-item index="/component/tool/save" v-show="org.id == 25">
                            <a class="a-link" @click="$router.push('/component/tool/save')">
                                Component Save
                            </a>
                        </el-menu-item>
                        <!--<el-menu-item index="/record">-->
                        <!--<a class="a-link" @click="$router.push('/test')">-->
                        <!--Test-->
                        <!--</a>-->
                        <!--</el-menu-item>-->
                    </el-submenu>
                </div>

                <div v-else-if="org.type == 4 && offline">
                    <el-menu-item index="/component/diagnosis/3rd-diagnosis">
                        <a class="a-link" @click="$router.push('/component/diagnosis/3rd-diagnosis')">
                            <img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/component/tool">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}
                        </template>
                        <el-menu-item index="/component/tool/hmi">
                            <a class="a-link" @click="$router.push('/component/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/controller">
                            <a class="a-link" @click="$router.push('/component/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/battery">
                            <a class="a-link" @click="$router.push('/component/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/sensor">
                            <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/update">
                            <a class="a-link" @click="$router.push('/component/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>

                <div v-else-if="org.type == 5 && !isSpecial">
                    <el-menu-item index="/service-center/center-ticket-lit">
                        <a class="a-link" @click="$router.push({name: 'centerTicketList'})">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('centerTicket') }}
                        </a>
                    </el-menu-item>
                    <el-menu-item index="/service-center/my-ticket-list">
                        <a class="a-link" @click="$router.push('/service-center/my-ticket-list')">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('myTicket') }}
                        </a>
                    </el-menu-item>
                    <el-submenu index="/error/shooting">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>FAQ
                        </template>
                        <el-menu-item index="/service-center/error/shooting">
                            <a class="a-link" @click="$router.push('/service-center/error/shooting')">
                                {{ $t('errorCode') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>
                <div v-else-if="org.type == 5 && isSpecial">
                    <!-- <el-menu-item index="/service-center/center-ticket-lit">
                                  <a class="a-link" @click="$router.push({name: 'centerTicketList'})">
                                      <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('centerTicket') }}
                                  </a>
                              </el-menu-item>
                              <el-menu-item index="/service-center/my-ticket-list">
                                  <a class="a-link" @click="$router.push('/service-center/my-ticket-list')">
                                      <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('myTicket') }}
                                  </a>
                              </el-menu-item>
                              <el-submenu index="/error/shooting">
                                  <template slot="title">
                                      <img class="icon" src="../../assets/images/icon/menu/error-shooting.png"/>FAQ
                                  </template>
                                  <el-menu-item index="/error/shooting">
                                      <a class="a-link" @click="$router.push('/service-center/error/shooting')">
                                          {{ $t('errorCode') }}
                                      </a>
                                  </el-menu-item>
                    </el-submenu>-->
                    <!--<el-menu-item index="/service-center/after-index">-->
                    <!--<a class="a-link" @click="$router.push('/service-center/after-index')">-->
                    <!--<img class="icon" src="../../assets/images/icon/menu/account.png"/>After Sales-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!-- warehouse -->
                    <el-submenu index="/warehouse">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/warehouse.svg">
                            Warehouse
                        </template>
                        <el-menu-item index="/warehouse/list" @click="$router.push({ path: '/warehouse/list'})">
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/receipt.svg">
                                Warehouse
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/warehouse/area" @click="$router.push({ path: '/warehouse/area'})">
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/shipment.svg">
                                Area
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            index="/warehouse/location"
                            @click="$router.push({ path: '/warehouse/location'})"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/receipt.svg">
                                Location
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- products -->
                    <el-menu-item index="/products/index" @click="$router.push({ name:'productsIndex' })">
                        <a class="a-link">
                            <img class="icon" src="../../assets/images/icon/menu/product.png">
                            Products
                        </a>
                    </el-menu-item>
                    <!-- inventory -->
                    <el-menu-item index="/inventory/index" @click="$router.push({ name:'inventoryIndex' })">
                        <a class="a-link">
                            <img class="icon" src="../../assets/images/warehouse/inventory.svg">
                            Inventory
                        </a>
                    </el-menu-item>
                    <!-- stocks -->
                    <el-submenu index="/stocks">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/warehouse.svg">
                            Stocks
                        </template>
                        <!-- <el-menu-item index="/stocks/index" @click="$router.push({path:'/stocks/index'})">
                          <a class="a-link">
                            <img class="icon" src="../../assets/images/warehouse/inventory.svg" />
                            All
                          </a>
                        </el-menu-item>-->
                        <el-menu-item index="/stocks/in" @click="$router.push({name:'stockIn'})">
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/receipt.svg">
                                Stock In
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/stocks/out" @click="$router.push({path:'/stocks/out'})">
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/shipment.svg">
                                Stock Out
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- tool -->
                    <el-submenu index="/dealer/tool">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png">
                            {{ $t('tool') }}
                        </template>
                        <el-menu-item index="/hmi">
                            <a class="a-link" @click="$router.push('/dealer/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png">
                                {{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/controller">
                            <a class="a-link" @click="$router.push('/dealer/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png">
                                {{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/battery">
                            <a class="a-link" @click="$router.push('/dealer/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png">
                                {{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/sensor">
                            <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png">
                                {{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/update">
                            <a class="a-link" @click="$router.push('/dealer/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png">
                                {{ $t('update') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- after sales -->
                    <el-submenu
                        index="/after-sales"
                        v-if="serviceCenter|| parcelInformation ||customerServiceRecord ||technicalService"
                    >
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/after-sales.svg">
                            After Sales
                        </template>
                        <el-menu-item
                            v-ilpermission="'service-center'"
                            index="/after-sales/service-center"
                            @click="$router.push('/after-sales/service-center')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/service-center.svg">
                                Service Center
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            index="/after-sales/parcel-information"
                            @click="$router.push('/after-sales/parcel-information')"
                            v-ilpermission="'parcel-information'"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/package.svg">
                                Parcel Info
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'customer-service-record'"
                            index="/after-sales/customer-service-record"
                            @click="$router.push('/after-sales/customer-service-record')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/record.svg">
                                CS Record
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'technical-service'"
                            index="/after-sales/technical-service"
                            @click="$router.push('/after-sales/technical-service')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/technical.svg">
                                Technical
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- customer service -->
                    <el-submenu index="/customer-service" v-if=" customerServiceUser||technicalServiceUser">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/service.svg">
                            CSD
                        </template>
                        <el-menu-item
                            v-ilpermission="'customer-service-user'"
                            index="/customer-service/customer-service-user"
                            @click="$router.push('/customer-service/customer-service-user')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/user.svg">
                                Service User
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'technical-service-user'"
                            index="/customer-service/technical-service-user"
                            @click="$router.push('/customer-service/technical-service-user')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/technology.svg">
                                Technical User
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            index="/customer-service/carrier-order"
                            @click="$router.push('/customer-service/carrier-order')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/cleanRecords.svg">
                                Carrier Order
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- clean manage -->
                    <el-submenu index="/clean-manage" v-if=" commercialInvoice||cleanRecord">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/management.svg">
                            Clean Manage
                        </template>
                        <el-menu-item
                            v-ilpermission="'commerial-invoice'"
                            index="/clean-manage/commerial-invoice"
                            @click="$router.push('/clean-manage/commerial-invoice')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/ci.svg">
                                CI
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'clean-record'"
                            index="/clean-manage/clean-record"
                            @click="$router.push('/clean-manage/clean-record')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/cleanRecords.svg">
                                Clean Record
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!--&lt;!&ndash; warehouse &ndash;&gt;-->
                    <!--<el-submenu-->
                    <!--index="/warehouse"-->
                    <!--class="warehouse"-->
                    <!--v-if="warehouseList ||-->
                    <!--warehouseRegion||-->
                    <!--warehouseArea  ||-->
                    <!--warehouseAreaType||-->
                    <!--warehouseInventory ||-->
                    <!--warehouseShipment ||-->
                    <!--warehouseReceipt ||-->
                    <!--warehousePickList"-->
                    <!--&gt;-->
                    <!--<template slot="title">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/warehouse.svg">-->
                    <!--Warehouse-->
                    <!--</template>-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-region'"-->
                    <!--index="/warehouse/warehouse-region"-->
                    <!--@click="$router.push('/warehouse/warehouse-region')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/region.svg">-->
                    <!--Region-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-list'"-->
                    <!--index="/warehouse/warehouse-list"-->
                    <!--@click="$router.push('/warehouse/warehouse-list')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/list.svg">-->
                    <!--List-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-area'"-->
                    <!--index="/warehouse/warehouse-area"-->
                    <!--@click="$router.push('/warehouse/warehouse-area')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/area.svg">-->
                    <!--Warehouse-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-area-type'"-->
                    <!--index="/warehouse/warehouse-area-type"-->
                    <!--@click="$router.push('/warehouse/warehouse-area-type')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/category.svg">-->
                    <!--Area Type-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-inventory'"-->
                    <!--index="/warehouse/warehouse-inventory"-->
                    <!--@click="$router.push('/warehouse/warehouse-inventory')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/inventory.svg">-->
                    <!--Inventory-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-shipment'"-->
                    <!--index="/warehouse/shipment-order-list"-->
                    <!--@click="$router.push('/warehouse/shipment-order-list')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/shipment.svg">-->
                    <!--Stock Out-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--&lt;!&ndash; <el-submenu-->
                    <!--v-ilpermission="'warehouse-shipment'"-->
                    <!--index="/warehouse/shipment-order-list"-->
                    <!--&gt;-->
                    <!--<template slot="title">-->
                    <!--<a class="a-link">-->
                    <!--<img-->
                    <!--class="icon"-->
                    <!--src="../../assets/images/warehouse/shipment.svg"-->
                    <!--&gt;-->
                    <!--Stock Out-->
                    <!--</a>-->
                    <!--</template>-->
                    <!--<el-menu-item-->
                    <!--index="/warehouse/shipment-order-list"-->
                    <!--@click="$router.push('/warehouse/shipment-order-list')"-->
                    <!--&gt;purchase</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--index="/warehouse/shipment-order-list?1"-->
                    <!--@click="$router.push('/warehouse/shipment-order-list?1')"-->
                    <!--&gt;sale</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--index="/warehouse/shipment-order-list?2"-->
                    <!--@click="$router.push('/warehouse/shipment-order-list?2')"-->
                    <!--&gt;show</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--index="/warehouse/shipment-order-list?3"-->
                    <!--@click="$router.push('/warehouse/shipment-order-list?3')"-->
                    <!--&gt;other</el-menu-item>-->
                    <!--</el-submenu>&ndash;&gt;-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-receipt'"-->
                    <!--index="/warehouse/receipt-order-list"-->
                    <!--@click="$router.push('/warehouse/receipt-order-list')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/receipt.svg">-->
                    <!--Stock In-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--&lt;!&ndash; <el-submenu-->
                    <!--v-ilpermission="'warehouse-receipt'"-->
                    <!--index="/warehouse/receipt-order-list"-->
                    <!--&gt;-->
                    <!--<template slot="title">-->
                    <!--<a class="a-link">-->
                    <!--<img-->
                    <!--class="icon"-->
                    <!--src="../../assets/images/warehouse/receipt.svg"-->
                    <!--&gt;-->
                    <!--Stock In-->
                    <!--</a>-->
                    <!--</template>-->
                    <!--<el-menu-item-->
                    <!--index="/warehouse/shipment-order-list"-->
                    <!--@click="$router.push('/warehouse/receipt-order-list')"-->
                    <!--&gt;sale</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--index="/warehouse/shipment-order-list?1"-->
                    <!--@click="$router.push('/warehouse/receipt-order-list?1')"-->
                    <!--&gt;show</el-menu-item>-->
                    <!--<el-menu-item-->
                    <!--index="/warehouse/shipment-order-list?2"-->
                    <!--@click="$router.push('/warehouse/receipt-order-list?2')"-->
                    <!--&gt;other</el-menu-item>-->
                    <!--</el-submenu>&ndash;&gt;-->
                    <!--<el-menu-item-->
                    <!--v-ilpermission="'warehouse-pick-list'"-->
                    <!--index="/warehouse/pick-list"-->
                    <!--@click="$router.push('/warehouse/pick-list')"-->
                    <!--&gt;-->
                    <!--<a class="a-link">-->
                    <!--<img class="icon" src="../../assets/images/warehouse/receipt.svg">-->
                    <!--Pick List-->
                    <!--</a>-->
                    <!--</el-menu-item>-->
                    <!--</el-submenu>-->
                    <!-- finance -->
                    <el-submenu index="/finance" v-if="invoiceList || cashierList || financeReceptionList">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/finance.svg">
                            Finance
                        </template>
                        <el-menu-item
                            v-ilpermission="'invoice-list'"
                            index="/finance/invoice-list"
                            @click="$router.push('/finance/invoice-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/invoice.svg">
                                Invoice
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'cashier-list'"
                            index="/finance/cashier"
                            @click="$router.push('/finance/cashier')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/cashier.svg">
                                Cashier
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'finance-reception-list'"
                            index="/finance/reception"
                            @click="$router.push('/finance/reception')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/process.svg">
                                Reception
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- purchase -->
                    <el-submenu index="/purchase" v-if="purchaseList">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/purchase.svg">
                            Purchase
                        </template>
                        <el-menu-item
                            v-ilpermission="'purchase-list'"
                            index="/purchase/purchase-list"
                            @click="$router.push('/purchase/purchase-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/list.svg">
                                List
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- sales -->
                    <el-submenu index="/sales" v-if="proformaInvoiceList || deliveryList">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/sales.svg">
                            Sales
                        </template>
                        <el-menu-item
                            v-ilpermission="'proforma-invoice-list'"
                            index="/sales/proforma-invoice"
                            @click="$router.push('/sales/proforma-invoice')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/PI.svg">
                                PI
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'delivery-list'"
                            index="/sales/delivery-list"
                            @click="$router.push('/sales/delivery-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/delivery.svg">
                                Delivery
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- customer -->
                    <el-submenu index="/customer" v-if="true">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/customer.svg">
                            Customer
                        </template>
                        <el-menu-item
                            index="/customer/customer-list"
                            @click="$router.push('/customer/customer-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/list.svg">
                                List
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- department -->
                    <el-submenu index="/department" v-if="departmentList">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/department.svg">
                            Department
                        </template>
                        <el-menu-item
                            v-ilpermission="'department-list'"
                            index="/department/department-list"
                            @click="$router.push('/department/department-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/list.svg">
                                List
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- role -->
                    <el-submenu index="/role" v-if="roleList || roleSetting">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/role.svg">
                            Role
                        </template>
                        <el-menu-item
                            v-ilpermission="'role-list'"
                            index="/role/role-list"
                            @click="$router.push('/role/role-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/list.svg">
                                List
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'role-setting'"
                            index="/role/role-setting"
                            @click="$router.push('/role/role-setting')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/role-setting.svg">
                                Setting
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- user -->
                    <el-submenu index="/user" v-if="userNL">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/role.svg">
                            User
                        </template>
                        <el-menu-item
                            v-ilpermission="'user-nl'"
                            index="/user/user-list"
                            @click="$router.push('/user/user-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/role-setting.svg">
                                List
                            </a>
                        </el-menu-item>
                    </el-submenu>
                    <!-- permission -->
                    <el-submenu index="/permission" v-if="permissionList || permissionCategory">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/warehouse/permission.svg">
                            Permission
                        </template>
                        <el-menu-item
                            v-ilpermission="'permission-list'"
                            index="/permission/permission-list"
                            @click="$router.push('/permission/permission-list')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/list.svg">
                                List
                            </a>
                        </el-menu-item>
                        <el-menu-item
                            v-ilpermission="'permission-category'"
                            index="/permission/permission-category"
                            @click="$router.push('/permission/permission-category')"
                        >
                            <a class="a-link">
                                <img class="icon" src="../../assets/images/warehouse/category.svg">
                                Category
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>
                <div v-else-if="org.type == 6">
                    <el-submenu index="/component/diagnosis/3rd-diagnosis" v-show="org.id != 1549">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/diagnosis.png"/>{{ $t('diagnosis') }}
                        </template>
                        <el-menu-item index="/component/diagnosis/3rd-diagnosis">
                            <a class="a-link" @click="$router.push('/component/diagnosis/3rd-diagnosis')">
                                {{ $t('diagnosis') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/diagnosis/report-list">
                            <a class="a-link" @click="$router.push('/component/diagnosis/report-list')">
                                {{ $t('report') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>

                    <el-submenu index="/dealer/tool">
                        <template slot="title">
                            <img class="icon" src="../../assets/images/icon/menu/tool.png"/>{{ $t('tool') }}
                        </template>
                        <el-menu-item index="/dealer/tool/hmi" v-show="org.id != 1549">
                            <a class="a-link" @click="$router.push('/dealer/tool/hmi')">
                                <img class="icon" src="../../assets/images/icon/icon-hmi.png"/>{{ $t('hmi') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/tool/controller" v-show="org.id != 1549">
                            <a class="a-link" @click="$router.push('/dealer/tool/controller')">
                                <img class="icon" src="../../assets/images/icon/icon-controller.png"/>{{ $t('controller') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/tool/battery" v-show="org.id != 1549">
                            <a class="a-link" @click="$router.push('/dealer/tool/battery')">
                                <img class="icon" src="../../assets/images/icon/icon-battery.png"/>{{ $t('battery') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/component/tool/sensor" v-show="org.id != 1549">
                            <a class="a-link" @click="$router.push('/component/tool/sensor')">
                                <img class="icon" src="../../assets/images/icon/icon-sensor.png"/>{{ $t('sensor') }}
                            </a>
                        </el-menu-item>
                        <el-menu-item index="/dealer/tool/update">
                            <a class="a-link" @click="$router.push('/dealer/tool/update')">
                                <img class="icon" src="../../assets/images/icon/icon-update.png"/>{{ $t('update') }}
                            </a>
                        </el-menu-item>
                    </el-submenu>
                </div>
            </el-menu>

            <div class="menu-footer">
                <div class="menu">
                    <div class="menu-item" v-show="this.org.type !== 6">
                        <el-tooltip placement="top" effect="light">
                            <div slot="content">{{ $t('help') }}</div>
                            <a :href="helpUrl" title="Bafang" target="_blank">
                                <img src="../../assets/images/icon/icon-help.png" class="icon"/>
                            </a>
                        </el-tooltip>
                    </div>
                    <div
                        class="menu-item"
                        @click="setting"
                        v-show="!offline && this.org.type !== 6"
                    >
                        <el-tooltip placement="top" effect="light">
                            <div slot="content">{{ $t('setting') }}</div>
                            <img src="../../assets/images/icon/icon-setting.png" class="icon"/>
                        </el-tooltip>
                    </div>
                    <div class="menu-item" @click="logout">
                        <el-tooltip placement="top" effect="light">
                            <div slot="content">{{ $t('logout') }}</div>
                            <img src="../../assets/images/icon/icon-logout.png" class="icon"/>
                        </el-tooltip>
                    </div>
                </div>
                <div class="copyright" v-show="this.org.type !== 6">
                    <span class="power-by">Powered by</span>
                    <a href="http://www.bafang-e.com" title="Bafang" class="logo" target="_blank">
                        <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" id="logo" x="0px" y="0px" width="170.574px" height="48px" viewBox="0 0 170.574 48" enable-background="new 0 0 170.574 48" xml:space="preserve" class="svg replaced-svg">
                            <g>
                                <path fill="#D07327" d="M146.573,0H24C10.745,0,0,10.745,0,24s10.745,24,24,24h122.573c13.255,0,24.001-10.745,24.001-24   S159.828,0,146.573,0z"></path>
                                <path fill="#FFFFFF" d="M36.609,23.264c1.255-0.893,2.218-2.266,2.218-3.88c0-3.415-2.63-5.773-6.298-5.773H22.157v20.325   l10.708-0.001c3.52,0,6.54-2.578,6.54-6.096C39.404,25.743,38.312,24.2,36.609,23.264z M25.869,16.923h6.467   c1.535,0,2.713,1.111,2.713,2.582c0,1.51-1.089,2.571-2.773,2.571l-6.407,0.003V16.923z M32.731,30.622h-6.869v-5.396l6.929-0.005   c1.759,0,2.836,1.066,2.836,2.635C35.628,29.417,34.413,30.622,32.731,30.622z M53.04,13.057c-5.101,0-9.306,4.136-9.306,9.237   v10.301c0,1.045,0.847,1.893,1.893,1.893s1.91-0.848,1.91-1.893v-5.861h10.87v5.861c0,1.045,0.848,1.893,1.894,1.893   c1.045,0,1.909-0.848,1.909-1.893V22.294C62.208,17.193,58.142,13.057,53.04,13.057z M58.406,23.417h-10.87v-1.061   c0-3.159,2.013-5.72,5.435-5.72c3.42,0,5.435,2.561,5.435,5.72V23.417z M80.2,17.136h-9.053v5.935c0,0,6.923,0,7.896,0   s1.661,0.745,1.661,1.66c0,0.916-0.742,1.658-1.658,1.658h-7.899v6.207c0,1.045-0.864,1.893-1.909,1.893   c-1.046,0-1.893-0.848-1.893-1.893h0v-18.99l12.813,0.005c1.063,0,1.806,0.788,1.806,1.761   C81.964,16.347,81.174,17.136,80.2,17.136z M93.977,13.057c-5.103,0-9.308,4.136-9.308,9.237v10.301   c0,1.045,0.85,1.893,1.894,1.893c1.047,0,1.91-0.848,1.91-1.893v-5.861h10.87v5.861c0,1.045,0.848,1.893,1.893,1.893   s1.91-0.848,1.91-1.893V22.294C103.146,17.193,99.077,13.057,93.977,13.057z M99.343,23.417h-10.87v-1.061   c0-3.159,2.014-5.72,5.436-5.72c3.42,0,5.435,2.561,5.435,5.72V23.417z M125.836,32.596c0,1.045-0.934,1.893-1.979,1.893   c-1.046,0-1.962-0.848-1.962-1.893v-10.58c0-2.825-1.944-5.208-4.85-5.208s-4.851,2.383-4.851,5.208v10.58   c0,1.045-0.932,1.893-1.979,1.893c-1.045,0-1.961-0.848-1.961-1.893v-10.78c0-4.861,3.997-8.759,8.858-8.759   c4.86,0,8.722,3.897,8.722,8.759V32.596z M140.908,34.488c-6.103,0-10.783-4.557-10.783-10.716c0-5.918,4.845-10.715,10.854-10.715   c2.916,0,5.08,0.734,7.011,2.248c0.83,0.652,0.962,1.827,0.365,2.609c-0.621,0.814-1.652,1.006-2.459,0.445   c-1.533-1.067-2.916-1.726-4.989-1.726c-3.823,0-6.926,3.07-6.926,7.014c0,4.285,3.042,7.266,6.986,7.266   c1.563,0,2.952-0.412,4.043-1.109v-4.11h-3.419c0,0-0.002,0-0.004,0c-0.925,0-1.675-0.75-1.675-1.676   c0-0.926,0.75-1.677,1.675-1.677h7.222c0,0,0.002,6.841,0.002,9.222C147.046,33.094,144.241,34.488,140.908,34.488z"></path>
                            </g>
                        </svg>
                    </a>
                </div>
            </div>
        </div>
    </div>
</template>


<script type="text/ecmascript-6">
    import Core from 'core/core'
    import ZH from '../../../src/assets/lang/zh';
    import EN from '../../../src/assets/lang/en';
    import DE from '../../../src/assets/lang/de';
    import NL from '../../../src/assets/lang/nl';

    export default {
        name: 'sidebar',
        data() {
            return {
                Core: Core,
                org: Core.Data.getOrg() ? Core.Data.getOrg() : '',
                brand: '',
                authList: {},
                defaultActiveIndex: "3",
                helpUrl: "http://v.bafang-service.com/?t=" + Core.Util.time(),

                routeIndex: "",
                from: "",
                defaultOpenList: [],
                offline: Core.Data.getOffline() ? Core.Data.getOrg() : false,

                // 权限
                departmentList: false,
                roleList: false,
                roleSetting: false,
                warehouseList: false,
                warehouseRegion: false,
                warehouseArea: false,
                warehouseAreaType: false,
                warehouseInventory: false,
                warehouseShipment: false,
                warehouseReceipt: false,
                warehousePickList: false,
                permissionList: false,
                permissionCategory: false,
                invoiceList: false,
                cashierList: false,
                financeReceptionList: false,
                purchaseList: false,
                proformaInvoiceList: false,
                customerList: false,
                deliveryList: false,
                commercialInvoice: false,
                cleanRecord: false,
                //售后
                serviceCenter: false,
                parcelInformation: false,
                customerServiceRecord: false,
                technicalService: false,

                //客服
                customerServiceUser: false,
                technicalServiceUser: false,
                //用户
                userNL: false,

                //特殊的ServiceCenter
                isSpecial: false,

                sn:'',
                hardware_version:'',
                soft_version:'',
                count: 0,

                updateToolDialog: true,
                accountType: Core.Data.get("accountType"),
                accountType_2: Core.Data.get("accountType") && Core.Data.get("accountType").indexOf(2)>-1,
                accountType_3: Core.Data.get("accountType") && Core.Data.get("accountType").indexOf(3)>-1,
            }
        },
        i18n: {
            messages: {
                en: EN.Layout.Sidebar,
                zh: ZH.Layout.Sidebar,
                de: DE.Layout.Sidebar,
                nl: NL.Layout.Sidebar
            }
        },
        watch:{
            $route(){
                this.routeIndex = this.$router.currentRoute.path
            }
        },
        methods: {
            checkHeadList() {
                if (this.$route.name == "headList") {
                    this.$store.state.getParamHead();
                } else {
                    this.$router.push({
                    path: "/component/headList/index"
                    });
                }
            },
            logout() {
                if(this.offline){
                    Core.Data.setOffline(true);
                    this.$router.push({name: 'login'});
                }else {
                    Core.Api.User.logout().then(res => {
                        Core.Data.setOffline(true);
                        Core.Data.setToken(' ');
                        Core.Data.setAuth('');
                        this.$router.push({name: 'login'})
                    }).catch(() => {
                        Core.Data.setOffline(true);
                        this.$router.push({name: 'login'})
                    });
                }

            },

            setting() {
                this.$emit('settingDialog');
            },

            tapUserName() {
                if(this.offline) {
                    return;
                }

                let orgType = Core.Const.ORG_TYPE;
                switch (this.org.type) {
                    case orgType.BRAND:
                        this.$router.push({name: 'brandIndex'});
                        break;
                    case orgType.DEALER:
                        this.$router.push({name: 'dealerBrandInvitation'});
                        break;
                    case orgType.OEM:
                        this.$router.push({name: 'assemblerPartner'});
                        break;
                    case orgType.SPECIAL_DEAlER:
                        this.$router.push({name: 'dealerBrandList'});
                        break;
                    default:
                        break;
                }
            },

            getBrandPartner() {
                if (this.org) {
                    this.brand = Core.Data.getBrandPartner(this.org.id);
                }
            },

            getAuth(){
                if(this.offline){
                    return;
                }
                let $this = this;
                Core.Api.Auth.authDetail()
                    .then(res => {
                        $this.authList = res.auth;
//                        Core.Data.setAuth(res.auth);
                        $this.$store.commit("currentAuth", $this.authList);
                        let authFlag = false;

                        if(this.from == "login"){
                            switch( $this.org.type ){
                                case 2:
                                    if(!$this.authList['object.menu']){
                                        $this.$router.replace({ name: "dealerErrorShooting" })
                                    }
                                    break;
                                case 3:
                                    if(!$this.authList['production.menu']){
                                        $this.$router.replace({ name: "assemblerErrorShooting" })
                                    }
                                    break;
                                case 4:
                                    if(!$this.authList['product.menu']){
                                        $this.$router.replace({ name: "componentErrorShooting" })
                                    }
                                    break;
                            }
                        }

                        for (let i in $this.authList) {
                            if ($this.authList[i]) {
                                authFlag = true;
                                break;
                            }
                        }

                        if (!authFlag) {
                            $this.$root.eventHub.$emit(Core.Const.EVENT_HUB.NO_AUTH);
                        }
                    });
            },
            // 检查权限
            checkPermission(userPermissions) {
                const vm = this;
                userPermissions.forEach(ele => {
                    const { resourceId, permissioned } = ele;
                    switch (resourceId) {
                        case "department-list":
                            vm.departmentList = permissioned;
                            break;
                        case "role-list":
                            vm.roleList = permissioned;
                            break;
                        case "role-setting":
                            vm.roleSetting = permissioned;
                            break;
                        case "warehouse-list":
                            vm.warehouseList = permissioned;
                            break;
                        case "warehouse-region":
                            vm.warehouseRegion = permissioned;
                            break;
                        case "warehouse-area":
                            vm.warehouseArea = permissioned;
                            break;
                        case "warehouse-area-type":
                            vm.warehouseAreaType = permissioned;
                            break;
                        case "warehouse-inventory":
                            vm.warehouseInventory = permissioned;
                            break;
                        case "warehouse-shipment":
                            vm.warehouseShipment = permissioned;
                            break;
                        case "warehouse-receipt":
                            vm.warehouseReceipt = permissioned;
                            break;
                        case "warehouse-pick-list":
                            vm.warehousePickList = permissioned;
                            break;
                        case "permission-list":
                            vm.permissionList = permissioned;
                            break;
                        case "permission-category":
                            vm.permissionCategory = permissioned;
                            break;
                        case "invoice-list":
                            vm.invoiceList = permissioned;
                            break;
                        case "cashier-list":
                            vm.cashierList = permissioned;
                            break;
                        case "finance-reception-list":
                            vm.financeReceptionList = permissioned;
                            break;
                        case "purchase-list":
                            vm.purchaseList = permissioned;
                            break;
                        case "proforma-invoice-list":
                            vm.proformaInvoiceList = permissioned;
                            break;
                        case "customer-list":
                            vm.customerList = permissioned;
                            break;
                        case "delivery-list":
                            vm.deliveryList = permissioned;
                            break;
                        case "commerial-invoice":
                            vm.commercialInvoice = permissioned;
                            break;
                        case "clean-record":
                            vm.cleanRecord = permissioned;
                            break;
                        case "service-center":
                            vm.serviceCenter = permissioned;
                            break;
                        case "parcel-information":
                            vm.parcelInformation = permissioned;
                            break;
                        case "customer-service-record":
                            vm.customerServiceRecord = permissioned;
                            break;
                        case "customer-service-user":
                            vm.customerServiceUser = permissioned;
                            break;
                        case "technical-service-user":
                            vm.technicalServiceUser = permissioned;
                            break;
                        case "technical-service":
                            vm.technicalService = permissioned;
                            break;
                        case "user-nl":
                            vm.userNL = permissioned;
                            break;
                    }
                });
            }
        },

        mounted: function () {
//            $this.$root.eventHub.$on(Core.Const.EVENT_HUB.REFRESH_PARTNER_BRAND_ICON, (data) => {
//                this.brand = data;
//            });
            this.$root.eventHub.$on(Core.Const.EVENT_HUB.OPEN_MENU, (data) => {
                this.defaultOpenList = [data]
            });
            this.$root.eventHub.$on(Core.Const.EVENT_HUB.  ORG_CHANGE, (data) => {
                this.org = Core.Data.getOrg();
            });
            this.from = this.$route.query.from;
            if(this.org.id === 582 ||
                this.org.id === 1406 ||
                this.org.id === 390 ||
                this.org.id === 379 ||
                this.org.id === 711 ||
                this.org.id === 1453 ||
                this.org.id === 1454 ||
                this.org.id === 1504
            ){
                this.isSpecial = true;
            }else{
                this.isSpecial = false;
            }
            if (this.org && this.org.type === 5) {
                return;
            }

            this.getBrandPartner();
            // 获取用户权限
            this.getAuth();

        },
        created: function() {
            const userPermissions = this.$store.state.userPermissions;
            this.checkPermission(userPermissions);
        }
    }
</script>

<style lang="scss" rel="stylesheet/scss" scoped>
    .sidebar {
        overflow: auto;
        position: fixed;
        width: 208px;
        height: calc(100% - 50px);
        background-color: #272B37;
        .user-info {
            height: 268px;
            width: 100%;
            position: relative;
            .logo-div{
                width: 100%;
                height: 208px;
                .user-avatar {
                    width: 100%;
                    height: auto;
                    position: absolute;
                    top: 104px;
                    transform: translate(0, -50%);
                }
            }
            .brand-info {
                .user-name-box {
                    cursor: pointer;
                }
            }
            .user-name-box {
                position: absolute;
                bottom: 0;
                width: 100%;
                height: 60px;
                z-index: 100;
                background-color: #98B42C;
                line-height: 60px;
                overflow: hidden;
                text-overflow: ellipsis;
                white-space: nowrap;
                padding: 0 20px;
                .user-icon {
                    position: absolute;
                    top: 50%;
                    margin-top: -11px;
                    width: 15px;
                    height: 22px;
                }
                .user-name {
                    font-size: 14px;
                    font-weight: bold;
                    padding-left: 35px;
                }
                .brand-icon {
                    position: absolute;
                    top: 50%;
                    margin-top: -11px;
                    width: 20px;
                    height: 20px;
                }
                .arrow-icon {
                    position: absolute;
                    top: 50%;
                    right: 10%;
                    margin-top: -6px;
                    width: 10px;
                    height: 10px;
                }
            }
        }
        .el-menu-vertical-demo {
            width: 100%;
            height: auto;
            .a-link {
                font-size: 14px;
                font-weight: bold;
                display: inline-block;
                width: 100%;
                height: 100%;
                img.icon {
                    width: 16px;
                    margin-right: 20px;
                    vertical-align: middle;
                }
            }
            .a-link:hover {
                color: white;
            }
            .titleSec{
                padding-left: 0;
            }
        }
        .menu-container {
            display: -webkit-box;
            display: -webkit-flex;
            display: -moz-box;
            display: -ms-flexbox;
            display: flex;
            -webkit-box-orient: vertical;
            -webkit-flex-direction: column;
            -ms-flex-direction: column;
            flex-direction: column;
            -webkit-box-pack: justify;
            -webkit-justify-content: space-between;
            -ms-flex-pack: justify;
            justify-content: space-between;
            height: calc(100% - 268px);
            .menu-footer {
                height: auto;
                margin-top: 10px;
                margin-bottom: 10px;
                .menu {
                    display: flex;
                    align-items: center;
                    justify-content: center;
                    .menu-item {
                        cursor: pointer;
                        width: 60px;
                        height: 30px;
                        display: flex;
                        align-items: center;
                        justify-content: center;
                        >a {
                            height: 18px;
                        }
                        .icon {
                            height: 18px;
                            width: 18px;
                        }
                    }
                }
            }
            .copyright {
                text-align: center;
                padding: 10px 0;
                .power-by {
                    position: relative;
                    top: -6px;
                    padding-right: 4px;
                    font-size: 10px;
                    color: #A1A5B1;
                }
                #logo {
                    width: 60px;
                    height:20px;
                }
            }
            .new{
                .el-menu-item{
                    padding-left:0;
                }
                /*内联样式优先级不及行内样式*/
                /*.el-submenu{*/
                /*.el-menu{*/
                /*.el-submenu{*/
                /*.el-menu-item{*/
                /*padding-left:0;*/
                /*}*/
                /*}*/
                /*}*/
                /*}*/

            }
        }
        .color-orange {
            color: #E26829;
        }
    }
</style>



// WEBPACK FOOTER //
// sidebar.vue?13eb5ef2