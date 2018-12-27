import Vue from 'vue'
import App from './App.vue'
import VueRouter from 'vue-router'
Vue.use(VueRouter)
Vue.config.productionTip = false

import Index from './pages/Index.vue'
const routes = [
  { 
    path: '/', 
    name: 'index',
    component: Index,
    alias: ['home', '/index']
  },
];

const router = new VueRouter({
  routes,
  mode: 'hash'
});

new Vue({
  router,
  render: h => h(App),
}).$mount('#app')
